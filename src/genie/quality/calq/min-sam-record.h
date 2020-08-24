#ifndef CALQ_MIN_SAM_RECORD_H_
#define CALQ_MIN_SAM_RECORD_H_

namespace calq {

struct MinSamRecord {
    uint32_t posMin;
    uint32_t posMax;
    std::string reference;  // = RNAME
    std::string cigar;
    std::string sequence;
    std::string qvalues;
};

}  // namespace calq

#endif  // CALQ_MIN_SAM_RECORD_H_
