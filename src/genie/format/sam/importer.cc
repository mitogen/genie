/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "importer.h"
#include <genie/core/record/alignment_split/same-rec.h>
#include <genie/util/ordered-section.h>
#include <list>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace sam {

// ---------------------------------------------------------------------------------------------------------------------

Importer::Importer(size_t _blockSize, std::istream &_file) : blockSize(_blockSize), samReader(_file), ref_counter(0) {}

// ---------------------------------------------------------------------------------------------------------------------

std::tuple<bool, uint8_t> Importer::convertFlags2Mpeg(uint16_t flags) {
    uint8_t flags_mpeg = 0;
    bool rcomp = false;
    flags_mpeg |= (flags & 1024u) >> 10u;  // PCR / duplicate
    flags_mpeg |= (flags & 512u) >> 8u;    // vendor quality check failed
    flags_mpeg |= (flags & 2u) << 1u;      // proper pair

    rcomp = flags & 16u;  // rcomp

    return std::make_tuple(rcomp, flags_mpeg);
}

// ---------------------------------------------------------------------------------------------------------------------

char Importer::convertCigar2ECigarChar(char token) {
    static const auto lut_loc = []() -> std::string {
        std::string lut(128, 0);
        lut['M'] = '=';
        lut['='] = '=';
        lut['X'] = '=';
        lut['I'] = '+';
        lut['D'] = '-';
        lut['N'] = '*';
        lut['S'] = ')';
        lut['H'] = ']';
        lut['P'] = ']';
        return lut;
    }();
    UTILS_DIE_IF(token < 0, "Invalid cigar token " + std::to_string((char)token));
    char ret = lut_loc[token];
    UTILS_DIE_IF(ret == 0, "Invalid cigar token " + std::string(1, token));
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

int Importer::stepSequence(char token) {
    static const auto lut_loc = []() -> std::string {
        std::string lut(128, 0);
        lut['M'] = 1;
        lut['='] = 1;
        lut['X'] = 1;
        lut['I'] = 1;
        lut['D'] = 0;
        lut['N'] = 0;
        lut['S'] = 1;
        lut['H'] = 0;
        lut['P'] = 0;
        return lut;
    }();
    UTILS_DIE_IF(token < 0, "Invalid cigar token " + std::to_string((char)token));
    return lut_loc[token];
}

// ---------------------------------------------------------------------------------------------------------------------

std::string Importer::convertCigar2ECigar(const std::string &cigar, const std::string &seq) {
    std::string ecigar;
    const size_t EXPECTED_ELONGATION = 4;  // Additional braces for softclips + hardclips
    ecigar.reserve(cigar.length() + EXPECTED_ELONGATION);
    size_t seq_pos = 0;
    std::string digits;
    for (const auto &a : cigar) {
        if (std::isdigit(a)) {
            digits += a;
            continue;
        }
        if (a == 'X') {
            size_t end = std::stoi(digits) + seq_pos;
            UTILS_DIE_IF(end > seq.length(), "CIGAR not valid for seq");
            for (; seq_pos < end; ++seq_pos) {
                ecigar += std::toupper(seq[seq_pos]);
            }
        } else {
            if (a == 'S') {
                ecigar += '(';
            } else if (a == 'H') {
                ecigar += '[';
            }
            char token = convertCigar2ECigarChar(a);
            seq_pos += stepSequence(a) * std::stoi(digits);
            ecigar += digits;
            ecigar += token;
        }
        digits.clear();
    }
    return ecigar;
}

// ---------------------------------------------------------------------------------------------------------------------

core::record::Record Importer::convert(uint16_t ref, sam::Record &&_r1, sam::Record *_r2) {
    sam::Record r1 = std::move(_r1);
    auto flag_tuple = convertFlags2Mpeg(r1.getFlags());
    core::record::Record ret(_r2 ? 2 : 1, core::record::ClassType::CLASS_I, r1.moveQname(), "Genie",
                             std::get<1>(flag_tuple));

    core::record::Alignment alignment(convertCigar2ECigar(r1.getCigar(), r1.getSeq()),
                                      r1.checkFlag(Record::FlagPos::SEQ_REVERSE));
    alignment.addMappingScore(r1.getMapQ());
    core::record::AlignmentBox alignmentContainer(r1.getPos(), std::move(alignment));

    core::record::Segment segment(r1.moveSeq());
    if (r1.getQual() != "*") {
        segment.addQualities(r1.moveQual());
    }
    ret.addSegment(std::move(segment));

    if (_r2) {
        sam::Record r2 = std::move(*_r2);

        auto ecigar = convertCigar2ECigar(r2.getCigar(), r2.getSeq());
        core::record::Alignment alignment2(std::move(ecigar), r2.checkFlag(Record::FlagPos::SEQ_REVERSE));
        alignment2.addMappingScore(r2.getMapQ());

        core::record::Segment segment2(r2.moveSeq());
        if (r2.getQual() != "*") {
            segment2.addQualities(r2.moveQual());
        }
        ret.addSegment(std::move(segment2));

        auto splitAlign =
            util::make_unique<core::record::alignment_split::SameRec>(r2.getPos() - r1.getPos(), std::move(alignment2));
        alignmentContainer.addAlignmentSplit(std::move(splitAlign));
    } else {
    }

    ret.addAlignment(ref, std::move(alignmentContainer));  // TODO
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

bool Importer::pumpRetrieve(genie::core::Classifier *_classifier) {
    util::Watch watch;
    core::stats::PerfStats stats;
    core::record::Chunk chunk;
    std::vector<sam::Record> s;
    std::list<sam::Record> samRecords;
    uint16_t local_ref_num = 0;
    {
        samReader.read(blockSize, s, stats);
        if (s.size() == 0) {
            return false;
        }
        auto it = refs.find(s.front().getRname());
        if (it == refs.end()) {
            local_ref_num = ref_counter;
            refs.insert(std::make_pair(s.front().getRname(), ref_counter++));
        } else {
            local_ref_num = it->second;
        }
    }
    std::copy(s.begin(), s.end(), std::back_inserter(samRecords));

    std::cout << "Read " << samRecords.size() << " SAM record(s) " << std::endl;
    while (!samRecords.empty()) {
        sam::Record samRecord = std::move(samRecords.front());
        samRecords.erase(samRecords.begin());
        // Search for mate
        const std::string &rnameSearchString =
            samRecord.getRnext() == "=" ? samRecord.getRname() : samRecord.getRnext();
        auto mate = samRecords.begin();
        mate = samRecords.end();  // Disable pairs for now TODO: implement
        if (samRecord.getPnext() == samRecord.getPos() && samRecord.getRname() == rnameSearchString) {
            mate = samRecords.end();
        }
        for (; mate != samRecords.end(); ++mate) {
            if (mate->getRname() == rnameSearchString && mate->getPos() == samRecord.getPnext()) {
                // LOG_TRACE << "Found mate";
                break;
            }
        }
        if (mate == samRecords.end()) {
            // LOG_TRACE << "Did not find mate";
            if ((samRecord.getFlags() & (1u << uint16_t(Record::FlagPos::SEGMENT_UNMAPPED))) ||
                samRecord.getCigar() == "*" || samRecord.getPos() == 0 || samRecord.getRname() == "*") {
                core::record::Record r(1, core::record::ClassType::CLASS_U,
                                       samRecord.getQname() == "*" ? "" : samRecord.moveQname(), "", 0);
                core::record::Segment seg(samRecord.moveSeq());
                if (!samRecord.getQual().empty() && samRecord.getQual() != "*") {
                    seg.addQualities(samRecord.moveQual());
                }
                r.addSegment(std::move(seg));
                chunk.getData().emplace_back(std::move(r));
            } else {
                chunk.getData().emplace_back(convert(local_ref_num, std::move(samRecord), nullptr));
            }
        } else {
            // TODO: note the filtering of unaligned reads above. Move this to the encoder
            chunk.getData().emplace_back(convert(local_ref_num, std::move(samRecord), &*mate));
            samRecords.erase(mate);
        }
    }
    if (!chunk.getData().empty()) {
        stats.addDouble("time-sam-import", watch.check());
        chunk.setStats(std::move(stats));
        _classifier->add(std::move(chunk));
    }

    // Break if less than blockSize records were read from the SAM file
    return !samReader.isEnd();
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace sam
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
