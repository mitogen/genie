#ifndef GENIE_EXTERNAL_ALIGNMENT_H
#define GENIE_EXTERNAL_ALIGNMENT_H

#include <cstdint>
#include <memory>

namespace util {
class BitWriter;

class BitReader;
}  // namespace util

namespace format {
namespace mpegg_rec {
class ExternalAlignment {
   public:
    enum class MoreAlignmentInfoType : uint8_t { NONE, OTHER_REC };

    explicit ExternalAlignment(MoreAlignmentInfoType _moreAlignmentInfoType);

    virtual ~ExternalAlignment() = default;

    virtual void write(util::BitWriter *writer) const;

    static std::unique_ptr<ExternalAlignment> factory(util::BitReader *reader);

   protected:
    MoreAlignmentInfoType moreAlignmentInfoType;
};
}  // namespace mpegg_rec
}  // namespace format

#endif  // GENIE_EXTERNAL_ALIGNMENT_H
