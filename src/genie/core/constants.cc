/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "constants.h"
#include <algorithm>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

// ---------------------------------------------------------------------------------------------------------------------

const GenSubIndex GenSub::POS_MAPPING_FIRST = std::make_pair(GenDesc::POS, 0);
const GenSubIndex GenSub::POS_MAPPING_ADDITIONAL = std::make_pair(GenDesc::POS, 1);

const GenSubIndex GenSub::RCOMP = std::make_pair(GenDesc::RCOMP, 0);

const GenSubIndex GenSub::FLAGS_PCR_DUPLICATE = std::make_pair(GenDesc::FLAGS, 0);
const GenSubIndex GenSub::FLAGS_QUALITY_FAIL = std::make_pair(GenDesc::FLAGS, 1);
const GenSubIndex GenSub::FLAGS_PROPER_PAIR = std::make_pair(GenDesc::FLAGS, 2);

const GenSubIndex GenSub::MMPOS_TERMINATOR = std::make_pair(GenDesc::MMPOS, 0);
const GenSubIndex GenSub::MMPOS_POSITION = std::make_pair(GenDesc::MMPOS, 1);

const GenSubIndex GenSub::MMTYPE_TYPE = std::make_pair(GenDesc::MMTYPE, 0);
const GenSubIndex GenSub::MMTYPE_SUBSTITUTION = std::make_pair(GenDesc::MMTYPE, 1);
const GenSubIndex GenSub::MMTYPE_INSERTION = std::make_pair(GenDesc::MMTYPE, 2);

const GenSubIndex GenSub::CLIPS_RECORD_ID = std::make_pair(GenDesc::CLIPS, 0);
const GenSubIndex GenSub::CLIPS_TYPE = std::make_pair(GenDesc::CLIPS, 1);
const GenSubIndex GenSub::CLIPS_SOFT_STRING = std::make_pair(GenDesc::CLIPS, 2);
const GenSubIndex GenSub::CLIPS_HARD_LENGTH = std::make_pair(GenDesc::CLIPS, 3);

const GenSubIndex GenSub::UREADS = std::make_pair(GenDesc::UREADS, 0);

const GenSubIndex GenSub::RLEN = std::make_pair(GenDesc::RLEN, 0);

const GenSubIndex GenSub::PAIR_DECODING_CASE = std::make_pair(GenDesc::PAIR, 0);
const GenSubIndex GenSub::PAIR_SAME_REC = std::make_pair(GenDesc::PAIR, 1);
const GenSubIndex GenSub::PAIR_R1_SPLIT = std::make_pair(GenDesc::PAIR, 2);
const GenSubIndex GenSub::PAIR_R2_SPLIT = std::make_pair(GenDesc::PAIR, 3);
const GenSubIndex GenSub::PAIR_R1_DIFF_SEQ = std::make_pair(GenDesc::PAIR, 4);
const GenSubIndex GenSub::PAIR_R2_DIFF_SEQ = std::make_pair(GenDesc::PAIR, 5);
const GenSubIndex GenSub::PAIR_R1_DIFF_POS = std::make_pair(GenDesc::PAIR, 6);
const GenSubIndex GenSub::PAIR_R2_DIFF_POS = std::make_pair(GenDesc::PAIR, 7);

const GenSubIndex GenSub::MSCORE = std::make_pair(GenDesc::MSCORE, 0);

const GenSubIndex GenSub::MMAP_NUMBER_ALIGN = std::make_pair(GenDesc::MMAP, 0);
const GenSubIndex GenSub::MMAP_RIGHT_ALIGN_ID = std::make_pair(GenDesc::MMAP, 1);
const GenSubIndex GenSub::MMAP_OTHER_REC_FLAG = std::make_pair(GenDesc::MMAP, 2);
const GenSubIndex GenSub::MMAP_REF_SEQ = std::make_pair(GenDesc::MMAP, 3);
const GenSubIndex GenSub::MMAP_REF_POS = std::make_pair(GenDesc::MMAP, 4);

const GenSubIndex GenSub::MSAR_CABAC_0 = std::make_pair(GenDesc::MSAR, 0);
const GenSubIndex GenSub::MSAR_CABAC_1 = std::make_pair(GenDesc::MSAR, 1);

const GenSubIndex GenSub::RTYPE = std::make_pair(GenDesc::RTYPE, 0);

const GenSubIndex GenSub::RGROUP = std::make_pair(GenDesc::RGROUP, 0);

const GenSubIndex GenSub::QV_PRESENT = std::make_pair(GenDesc::QV, 0);
const GenSubIndex GenSub::QV_CODEBOOK = std::make_pair(GenDesc::QV, 1);
const GenSubIndex GenSub::QV_STEPS_0 = std::make_pair(GenDesc::QV, 2);
const GenSubIndex GenSub::QV_STEPS_1 = std::make_pair(GenDesc::QV, 3);
const GenSubIndex GenSub::QV_STEPS_2 = std::make_pair(GenDesc::QV, 4);
const GenSubIndex GenSub::QV_STEPS_3 = std::make_pair(GenDesc::QV, 5);
const GenSubIndex GenSub::QV_STEPS_4 = std::make_pair(GenDesc::QV, 6);
const GenSubIndex GenSub::QV_STEPS_5 = std::make_pair(GenDesc::QV, 7);
const GenSubIndex GenSub::QV_STEPS_6 = std::make_pair(GenDesc::QV, 8);

const GenSubIndex GenSub::RNAME_CABAC_0 = std::make_pair(GenDesc::RNAME, 0);
const GenSubIndex GenSub::RNAME_CABAC_1 = std::make_pair(GenDesc::RNAME, 1);

const GenSubIndex GenSub::RFTP = std::make_pair(GenDesc::RFTP, 0);

const GenSubIndex GenSub::RFTT = std::make_pair(GenDesc::RFTT, 0);

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<GenomicDescriptorProperties> &getDescriptors() {
    static const std::vector<GenomicDescriptorProperties> prop = {
        {GenDesc::POS,
         "pos",
         false,
         {{GenSub::POS_MAPPING_FIRST, "first"}, {GenSub::POS_MAPPING_ADDITIONAL, "additional"}}},

        {GenDesc::RCOMP, "rcomp", false, {{GenSub::RCOMP, "rcomp"}}},

        {GenDesc::FLAGS,
         "flags",
         false,
         {{GenSub::FLAGS_PCR_DUPLICATE, "pcr_duplicate"},
          {GenSub::FLAGS_QUALITY_FAIL, "quality_flag"},
          {GenSub::FLAGS_PROPER_PAIR, "proper_pair"}}},

        {GenDesc::MMPOS,
         "mmpos",
         false,
         {{GenSub::MMPOS_TERMINATOR, "terminator"}, {GenSub::MMPOS_POSITION, "position"}}},

        {GenDesc::MMTYPE,
         "mmtype",
         false,
         {{GenSub::MMTYPE_TYPE, "type"},
          {GenSub::MMTYPE_SUBSTITUTION, "substitution"},
          {GenSub::MMTYPE_INSERTION, "insertion"}}},

        {GenDesc::CLIPS,
         "clips",
         false,
         {{GenSub::CLIPS_RECORD_ID, "record_id"},
          {GenSub::CLIPS_TYPE, "type"},
          {GenSub::CLIPS_SOFT_STRING, "soft_string"},
          {GenSub::CLIPS_HARD_LENGTH, "hard_length"}}},

        {GenDesc::UREADS, "ureads", false, {{GenSub::UREADS, "ureads"}}},

        {GenDesc::RLEN, "rlen", false, {{GenSub::RLEN, "rlen"}}},

        {GenDesc::PAIR,
         "pair",
         false,
         {{GenSub::PAIR_DECODING_CASE, "decoding_case"},
          {GenSub::PAIR_SAME_REC, "same_rec"},
          {GenSub::PAIR_R1_SPLIT, "r1_split"},
          {GenSub::PAIR_R2_SPLIT, "r2_split"},
          {GenSub::PAIR_R1_DIFF_SEQ, "r1_diff_seq"},
          {GenSub::PAIR_R2_DIFF_SEQ, "r2_diff_seq"},
          {GenSub::PAIR_R1_DIFF_POS, "r1_diff_pos"},
          {GenSub::PAIR_R2_DIFF_POS, "r2_diff_pos"}}},

        {GenDesc::MSCORE, "mscore", false, {{GenSub::MSCORE, "mscore"}}},

        {GenDesc::MMAP,
         "mmap",
         false,
         {{GenSub::MMAP_NUMBER_ALIGN, "number_alignments"},
          {GenSub::MMAP_RIGHT_ALIGN_ID, "right_alignment_id"},
          {GenSub::MMAP_OTHER_REC_FLAG, "other_rec_flag"},
          {GenSub::MMAP_REF_SEQ, "reference_seq"},
          {GenSub::MMAP_REF_POS, "reference_pos"}}},

        {GenDesc::MSAR, "msar", true, {{GenSub::MSAR_CABAC_0, "cabac_0"}, {GenSub::MSAR_CABAC_1, "cabac_1"}}},

        {GenDesc::RTYPE, "rtype", false, {{GenSub::RTYPE, "rtype"}}},

        {GenDesc::RGROUP, "rgroup", false, {{GenSub::RGROUP, "rgroup"}}},

        {GenDesc::QV,
         "qv",
         false,
         {{GenSub::QV_PRESENT, "present"},
          {GenSub::QV_CODEBOOK, "codebook"},
          {GenSub::QV_STEPS_0, "steps_0"},
          {GenSub::QV_STEPS_1, "steps_1"},
          {GenSub::QV_STEPS_2, "steps_2"},
          {GenSub::QV_STEPS_3, "steps_3"},
          {GenSub::QV_STEPS_4, "steps_4"},
          {GenSub::QV_STEPS_5, "steps_5"},
          {GenSub::QV_STEPS_6, "steps_6"}}},

        {GenDesc::RNAME, "rname", true, {{GenSub::RNAME_CABAC_0, "cabac0"}, {GenSub::RNAME_CABAC_1, "cabac1"}}},

        {GenDesc::RFTP, "rftp", false, {{GenSub::RFTP, "rftp"}}},

        {GenDesc::RFTT, "rftt", false, {{GenSub::RFTP, "rftt"}}},
    };

    return prop;
}

// ---------------------------------------------------------------------------------------------------------------------

const GenomicDescriptorProperties &getDescriptor(GenDesc desc) { return getDescriptors()[uint8_t(desc)]; }

// ---------------------------------------------------------------------------------------------------------------------

const GenomicSubDescriptorProperties &getSubsequence(GenSubIndex idx) {
    return getDescriptors()[uint8_t(idx.first)].subseqs[uint8_t(idx.second)];
}

// ---------------------------------------------------------------------------------------------------------------------

const Alphabet &getAlphabetProperties(AlphabetID id) {
    static const auto prop = []() -> std::vector<Alphabet> {
        std::vector<Alphabet> loc;
        loc.emplace_back();
        loc.back().lut = {'A', 'C', 'G', 'T', 'N'};
        loc.emplace_back();
        loc.back().lut = {'A', 'C', 'G', 'T', 'R', 'Y', 'S', 'W', 'K', 'M', 'B', 'D', 'H', 'V', 'N', '-'};
        for (auto &l : loc) {
            l.inverseLut = std::vector<char>(*std::max_element(l.lut.begin(), l.lut.end()) + 1, 0);
            for (size_t i = 0; i < l.lut.size(); ++i) {
                l.inverseLut[l.lut[i]] = static_cast<char>(i);
            }
        }
        return loc;
    }();
    return prop[uint8_t(id)];
}

// ---------------------------------------------------------------------------------------------------------------------

const GenomicSubDescriptorProperties &GenomicDescriptorProperties::getSubSeq(uint8_t sub) const {
    return subseqs[uint8_t(sub)];
}

// ---------------------------------------------------------------------------------------------------------------------

bool Alphabet::isIncluded(char c) const {
    for (const char &lc : lut) {
        if (lc == c) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

const CigarFormatInfo &getECigarInfo() {
    const static auto formatInfo = []() -> CigarFormatInfo {
        const auto ref_step2 = []() -> std::vector<uint8_t> {
            std::vector<uint8_t> lut(128, 0);
            lut['='] = 1;
            lut['+'] = 0;
            lut['-'] = 1;
            lut['*'] = 1;
            lut['/'] = 1;
            lut['%'] = 1;
            lut[')'] = 0;
            lut[']'] = 0;
            return lut;
        }();
        const auto seq_step2 = []() -> std::vector<uint8_t> {
            std::vector<uint8_t> lut(128, 0);
            lut['='] = 1;
            lut['+'] = 1;
            lut['-'] = 0;
            lut['%'] = 1;
            lut['/'] = 1;
            lut['*'] = 1;
            lut[')'] = 1;
            lut[']'] = 0;
            return lut;
        }();
        const auto ignore2 = []() -> std::vector<uint8_t> {
            std::vector<uint8_t> lut(128, 0);
            lut['('] = 1;
            lut['['] = 1;
            return lut;
        }();
        return {ref_step2, seq_step2, ignore2, true};
    }();
    return formatInfo;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
