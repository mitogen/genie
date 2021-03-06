/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/format/sam/record.h"
#include <algorithm>
#include <regex>
#include <utility>
#include <vector>
#include "genie/core/constants.h"
#include "genie/format/sam/header/tag.h"
#include "genie/util/runtime-exception.h"
#include "genie/util/string-helpers.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace sam {

// ---------------------------------------------------------------------------------------------------------------------

const core::CigarFormatInfo& getSAMCigarInfo() {
    static const auto formatInfo = []() -> core::CigarFormatInfo {
        const auto ref_step2 = []() -> std::vector<uint8_t> {
            std::vector<uint8_t> lut(128, 0);
            lut['M'] = 1;
            lut['='] = 1;
            lut['X'] = 1;
            lut['I'] = 0;
            lut['D'] = 1;
            lut['N'] = 1;
            lut['S'] = 0;
            lut['H'] = 0;
            lut['P'] = 0;
            return lut;
        }();
        const auto seq_step2 = []() -> std::vector<uint8_t> {
            std::vector<uint8_t> lut(128, 0);
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
        return {ref_step2, seq_step2, {}, false};
    }();
    return formatInfo;
}

// ---------------------------------------------------------------------------------------------------------------------

Record::Record(std::string _qname, uint16_t _flag, std::string _rname, uint32_t _pos, uint8_t _mapq, std::string _cigar,
               std::string _rnext, uint32_t _pnext, int32_t _tlen, std::string _seq, std::string _qual)
    : qname(std::move(_qname)),
      flag(_flag),
      rname(std::move(_rname)),
      pos(_pos),
      mapq(_mapq),
      cigar(std::move(_cigar)),
      rnext(std::move(_rnext)),
      pnext(_pnext),
      tlen(_tlen),
      seq(std::move(_seq)),
      qual(std::move(_qual)) {
    checkValuesUsingRegex();
    checkValuesUsingCondition();
}

// ---------------------------------------------------------------------------------------------------------------------

Record::Record() : Record("", 0, "", 0, 0, "", "", 0, 0, "", "") {}

// ---------------------------------------------------------------------------------------------------------------------

Record::Record(const std::string& string) {
    auto tokens = util::tokenize(string, '\t');
    UTILS_DIE_IF(tokens.size() < 11, "Invalid SAM record");
    qname = tokens[0];
    flag = (uint16_t)std::stoi(tokens[1]);
    rname = tokens[2];
    pos = std::stoi(tokens[3]);
    mapq = (uint8_t)std::stoi(tokens[4]);
    cigar = tokens[5];
    rnext = tokens[6];
    pnext = std::stoi(tokens[7]);
    tlen = std::stoi(tokens[8]);
    seq = tokens[9];
    qual = tokens[10];

    checkValuesUsingRegex();
    checkValuesUsingCondition();
}

Record::Record(const std::string& _qname, const std::string& string) {
    auto tokens = util::tokenize(string, '\t');
    UTILS_DIE_IF(tokens.size() < 10, "Invalid SAM record");

    qname = _qname;
    flag = (uint16_t)std::stoi(tokens[0]);
    rname = tokens[1];
    pos = std::stoi(tokens[2]);
    mapq = (uint8_t)std::stoi(tokens[3]);
    cigar = tokens[4];
    rnext = tokens[5];
    pnext = std::stoi(tokens[6]);
    tlen = std::stoi(tokens[7]);
    seq = tokens[8];
    qual = tokens[9];

    checkValuesUsingRegex();
    checkValuesUsingCondition();
}

// ---------------------------------------------------------------------------------------------------------------------

void Record::checkValuesUsingRegex() const {
    static const std::regex REGEX_QNAME("[!-?A-~]{1,254}");
    static const std::regex REGEX_RNAME("\\*|" + header::getSAMRegex());
    static const std::regex REGEX_CIGAR("\\*|([0-9]+[MIDNSHPX=])+");
    static const std::regex REGEX_RNEXT("\\*|=|" + header::getSAMRegex());
    static const std::regex REGEX_SEQ("\\*|[A-Za-z=.]+");
    static const std::regex REGEX_QUAL("[!-~]+");
    UTILS_DIE_IF(!std::regex_match(qname, REGEX_QNAME) || !std::regex_match(rname, REGEX_RNAME) ||
                     !std::regex_match(cigar, REGEX_CIGAR) || !std::regex_match(rnext, REGEX_RNEXT) ||
                     !std::regex_match(seq, REGEX_SEQ) || !std::regex_match(qual, REGEX_QUAL),
                 "Invalid SAM record");
}

// ---------------------------------------------------------------------------------------------------------------------

void Record::checkValuesUsingCondition() const {
    // TODO(Yeremia): Check all conditions in SAM specification
    // Value of certain field may not be set to certain value because:
    //  > No assumption can be made does not mean default value
    //  > Round trip test may not possible

    // If read is pair and assigned as first and last segment at the same time
    UTILS_DIE_IF(checkFlag(FlagPos::MULTI_SEGMENT_TEMPLATE) && checkFlag(FlagPos::FIRST_SEGMENT) &&
                     checkFlag(FlagPos::LAST_SEGMENT),
                 "Invalid first & last segment flags");

    // True unmapped
    //   File : "simulation.1.homoINDELs.homoCEUsnps.reads2.fq.sam.samelength.sam"
    //    if (isUnmapped()) {
    //        rname = "*";
    //        pos = 0;
    //        cigar = "*";
    //        mapq = 255; // TODO(Yeremia): Check default value of mapq
    //
    //        flag &= ~(1u << uint16_t(Record::FlagPos::PROPERLY_ALIGNED));
    //        flag &= ~(1u << uint16_t(Record::FlagPos::SECONDARY_ALIGNMENT));
    //        flag &= ~(1u << uint16_t(Record::FlagPos::SUPPLEMENTARY_ALIGNMENT));
    //    }

    // TODO(Yeremia): Should the value of mapq set to default and unset flags just like using reliable unmapped
    // condition?
    //       Case can be found in "simulation.1.homoINDELs.homoCEUsnps.reads2.fq.sam.samelength.sam"
    // Secondary unmapped condition - no assumption can be made
    //    if (rname == "*" || pos == 0 || cigar == "*") {
    //        rname = "*";
    //        pos = 0;
    //        cigar = "*";
    //    }

    // Single-ended read
    //    if (!isMultiSeg()) {
    //        flag &= ~(1u << uint16_t(Record::FlagPos::PROPERLY_ALIGNED));       // 0x2
    //        flag &= ~(1u << uint16_t(Record::FlagPos::NEXT_SEGMENT_UNMAPPED));  // 0x8
    //        flag &= ~(1u << uint16_t(Record::FlagPos::NEXT_SEQ_REVERSE));       // 0x20
    //        flag &= ~(1u << uint16_t(Record::FlagPos::FIRST_SEGMENT));          // 0x40
    //        flag &= ~(1u << uint16_t(Record::FlagPos::LAST_SEGMENT));           // 0x80
    //    }

    // Unknown pair
    //    if (rnext == "*" || pnext == 0) {
    //        // No assumption can be made
    //        rnext = "*";
    //        pnext = 0;
    //        flag &= ~(1u << uint16_t(Record::FlagPos::NEXT_SEQ_REVERSE));
    //    }

    // Special character
}

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getQname() const { return qname; }

// ---------------------------------------------------------------------------------------------------------------------

uint16_t Record::getFlags() const { return flag; }

// ---------------------------------------------------------------------------------------------------------------------

bool Record::checkFlag(FlagPos f) const { return getFlags() & (1u << uint8_t(f)); }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getRname() const { return rname; }

// ---------------------------------------------------------------------------------------------------------------------

uint32_t Record::getPos() const { return pos; }

// ---------------------------------------------------------------------------------------------------------------------

uint8_t Record::getMapQ() const { return mapq; }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getCigar() const { return cigar; }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getRnext() const { return rnext; }

// ---------------------------------------------------------------------------------------------------------------------

uint32_t Record::getPnext() const { return pnext; }

// ---------------------------------------------------------------------------------------------------------------------

int32_t Record::getTlen() const { return tlen; }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getSeq() const { return seq; }

// ---------------------------------------------------------------------------------------------------------------------

const std::string& Record::getQual() const { return qual; }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveQname() { return std::move(qname); }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveRname() { return std::move(rname); }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveCigar() { return std::move(cigar); }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveRnext() { return std::move(rnext); }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveSeq() { return std::move(seq); }

// ---------------------------------------------------------------------------------------------------------------------

std::string&& Record::moveQual() { return std::move(qual); }

// ---------------------------------------------------------------------------------------------------------------------

std::string Record::toString() const {
    std::string ret;
    ret += qname + '\t' + std::to_string(flag) + '\t' + rname + '\t' + std::to_string(pos) + '\t';
    ret += std::to_string(mapq) + '\t' + cigar + '\t' + rnext + '\t' + std::to_string(pnext) + '\t';
    ret += std::to_string(tlen) + '\t' + seq + '\t' + qual;
    return ret;
}
// ---------------------------------------------------------------------------------------------------------------------
bool Record::isFirstRead() const { return checkFlag(FlagPos::FIRST_SEGMENT); }
// ---------------------------------------------------------------------------------------------------------------------
bool Record::isLastRead() const { return checkFlag(FlagPos::LAST_SEGMENT); }
// ---------------------------------------------------------------------------------------------------------------------
bool Record::isPairedEnd() const { return checkFlag(FlagPos::MULTI_SEGMENT_TEMPLATE); }
// ---------------------------------------------------------------------------------------------------------------------
bool Record::isPrimaryLine() const {
    return !(checkFlag(FlagPos::SECONDARY_ALIGNMENT) || checkFlag(FlagPos::SUPPLEMENTARY_ALIGNMENT));
}

// ---------------------------------------------------------------------------------------------------------------------

bool Record::isUnmapped() const {
    // FlagPos::SEGMENT_UNMAPPED is the only reliable way to tell whether a read is unmapped
    // Unmapped segment without coordinate has RNAME of '*'
    // Unmapped coordinate may have ordinary coordinate != 0
    return checkFlag(FlagPos::SEGMENT_UNMAPPED) || rname == "*";
}

// ---------------------------------------------------------------------------------------------------------------------

bool Record::isNextUnmapped() const {
    // See isUnmapped()
    return checkFlag(FlagPos::NEXT_SEGMENT_UNMAPPED) || rnext == "*";
}

// ---------------------------------------------------------------------------------------------------------------------

bool Record::isPairOf(Record& other) const {
    // If any read is not multi segments
    if (!(checkFlag(FlagPos::MULTI_SEGMENT_TEMPLATE) && other.checkFlag(FlagPos::MULTI_SEGMENT_TEMPLATE))) {
        return false;
    }

    // Check RNEXT and PNEXT can only be done if both are primary line
    // For secondary alignment, RNEXT and PNEXT point to RNAME and POS of primary line of the next template
    // See "Recommended Practice for SAM Format" in SAM Format documentation for more information
    if (isPrimaryLine() && other.isPrimaryLine()) {
        // If next segment is mapped or data of other is complete, check others fields
        if (!(isNextUnmapped() || other.isUnmapped())) {
            if (!((rnext == "=" ? rname : rnext) == other.rname && pnext == other.pos &&
                  checkFlag(FlagPos::NEXT_SEQ_REVERSE) == other.checkFlag(FlagPos::SEQ_REVERSE))) {
                return false;
            }
        }

        // If self is mapped or data for assumption if complete, check fields
        if (!(isUnmapped() || other.isNextUnmapped())) {
            if (!((other.rnext == "=" ? other.rname : other.rnext) == rname && other.pnext == pos &&
                  other.checkFlag(FlagPos::NEXT_SEQ_REVERSE) == checkFlag(FlagPos::SEQ_REVERSE))) {
                return false;
            }
        }
    }

    // Only one of it is the first segment and one of it is the last segment
    // Condition where both FirstSeg & LastSeg flags are set is already checked during construction
    return !((checkFlag(FlagPos::FIRST_SEGMENT) && other.checkFlag(FlagPos::FIRST_SEGMENT)) ||
             (checkFlag(FlagPos::LAST_SEGMENT) && other.checkFlag(FlagPos::LAST_SEGMENT)));
}

// const std::string &Record::getReverseSeq() const {
//    std::string revSeq = seq;
//    std::reverse(revSeq.begin(), revSeq.end())
//    return &revSeq;
//}

void Record::setSeq(const std::string& _seq) { seq = _seq; }

// ---------------------------------------------------------------------------------------------------------------------

ReadTemplate::ReadTemplate() : qname(), data(size_t(Index::TOTAL_TYPES)) {
    //    initializeData();
}

// ---------------------------------------------------------------------------------------------------------------------

ReadTemplate::ReadTemplate(Record&& rec) : qname(rec.getQname()), data(size_t(Index::TOTAL_TYPES)) {
    //    initializeData();
    addRecord(std::move(rec));
}

// ---------------------------------------------------------------------------------------------------------------------

const std::string& ReadTemplate::getQname() { return qname; }

// ---------------------------------------------------------------------------------------------------------------------

void ReadTemplate::initializeData() { data.resize(size_t(Index::TOTAL_TYPES)); }

void ReadTemplate::addRecord(Record&& rec) {
    if (qname != rec.getQname()) {
        if (qname.empty()) {
            qname = rec.getQname();
        } else {
            UTILS_DIE("Record with QNAME " + rec.getQname() + " was added to ReadTemplate with " + "QNAME " + qname);
        }
    }

    auto idx = Index::UNKNOWN;

    // Handles paired-end read / multi segments
    if (rec.checkFlag(Record::FlagPos::MULTI_SEGMENT_TEMPLATE)) {
        //        UTILS_DIE_IF(rec.checkFlag(Record::FlagPos::SEGMENT_UNMAPPED),
        //                     "Unmapped multi segment found for QNAME " + qname + " !");

        if (rec.checkFlag(Record::FlagPos::FIRST_SEGMENT)) {
            idx = Index::PAIR_FIRST;
        } else if (rec.checkFlag(Record::FlagPos::LAST_SEGMENT)) {
            idx = Index::PAIR_LAST;
        } else {
            UTILS_DIE("Neither first nor last segment for Template " + qname + " !");
        }

        // Handles single-end read / single segment
        // TODO(Yeremia): for linear template FIRST_SEGMENT and LAST_SEGMENT are set,
    } else if (!rec.checkFlag(Record::FlagPos::FIRST_SEGMENT) && !rec.checkFlag(Record::FlagPos::LAST_SEGMENT)) {
        idx = rec.checkFlag(Record::FlagPos::SEGMENT_UNMAPPED) ? Index::SINGLE_UNMAPPED : Index::SINGLE_MAPPED;

        // Should never be reached
    } else {
        UTILS_DIE("Not handled case found for QNAME " + qname + " !");
    }

    if (rec.isPrimaryLine()) {
        data[uint8_t(idx)].push_front(std::move(rec));
    } else {
        data[uint8_t(idx)].push_back(std::move(rec));
    }
}

bool ReadTemplate::isUnmapped() { return data[uint8_t(Index::SINGLE_UNMAPPED)].size() == 1; }

bool ReadTemplate::isSingle() {
    // Single non- and multiplie alignements
    return !data[uint8_t(Index::SINGLE_MAPPED)].empty() && data[uint8_t(Index::SINGLE_MAPPED)].front().isPrimaryLine();
}

bool ReadTemplate::isPair() {
    // Pair non- and multiple alignments
    //    return !data[uint8_t(Index::PAIR_FIRST)].empty() &&
    //           !data[uint8_t(Index::PAIR_LAST)].empty() &&
    //           data[uint8_t(Index::PAIR_FIRST)].front().isPrimaryLine() &&
    //           data[uint8_t(Index::PAIR_LAST)].front().isPrimaryLine() &&
    //           data[uint8_t(Index::PAIR_FIRST)].front().isPairOf(data[uint8_t(Index::PAIR_LAST)].front()) &&
    //           data[uint8_t(Index::PAIR_LAST)].front().isPairOf(data[uint8_t(Index::PAIR_FIRST)].front());

    // Allow unpaired reads
    return (!data[uint8_t(Index::PAIR_FIRST)].empty() && data[uint8_t(Index::PAIR_FIRST)].front().isPrimaryLine()) ||
           (!data[uint8_t(Index::PAIR_LAST)].empty() && data[uint8_t(Index::PAIR_LAST)].front().isPrimaryLine());
}

bool ReadTemplate::isUnknown() { return !data[uint8_t(Index::UNKNOWN)].empty(); }

bool ReadTemplate::isValid() {
    if (isUnknown() || data[uint8_t(Index::SINGLE_UNMAPPED)].size() > 1) {
        return false;
    } else {
        auto is_unmapped = isUnmapped();
        auto is_single = isSingle();
        auto is_pair = isPair();
        return (is_unmapped && !is_single && !is_pair) || (!is_unmapped && is_single && !is_pair) ||
               (!is_unmapped && !is_single && is_pair);
    }
}

bool ReadTemplate::getRecords(std::list<std::list<Record>>& sam_recs) {
    sam_recs.clear();
    if (isValid()) {
        if (isUnmapped()) {
            sam_recs.push_back(std::move(data[uint8_t(Index::SINGLE_UNMAPPED)]));
        } else if (isSingle()) {
            sam_recs.push_back(std::move(data[uint8_t(Index::SINGLE_MAPPED)]));
        } else if (isPair()) {
            sam_recs.push_back(std::move(data[uint8_t(Index::PAIR_FIRST)]));
            sam_recs.push_back(std::move(data[uint8_t(Index::PAIR_LAST)]));
        }
        return true;
    } else {
        return false;
    }
}

}  // namespace sam
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
