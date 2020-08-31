#ifndef CALQ_SAM_PILEUP_H_
#define CALQ_SAM_PILEUP_H_

// -----------------------------------------------------------------------------

#include <string>
#include <vector>

// -----------------------------------------------------------------------------

namespace genie{
namespace quality{
namespace calq {

// -----------------------------------------------------------------------------

class SAMPileup
{
 public:
    SAMPileup();
    ~SAMPileup();

    bool empty() const;
    void clear();
    void print() const;
    void printQual() const;
    void printSeq() const;

    uint32_t pos;  // 0-based position of this pileup
    std::string qual;
    std::string seq;
    char ref;

    uint16_t hq_softcounter;  // High quality softclips next to this position
};

// -----------------------------------------------------------------------------

}  // namespace calq
}  // namespace quality
}  // namespace genie

// -----------------------------------------------------------------------------

#endif  // CALQ_SAM_PILEUP_H_

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
