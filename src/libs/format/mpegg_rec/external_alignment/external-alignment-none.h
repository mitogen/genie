#ifndef GENIE_EXTERNAL_ALIGNMENT_NONE_H
#define GENIE_EXTERNAL_ALIGNMENT_NONE_H

#include "external-alignment.h"

namespace util {
class BitWriter;
class BitReader;
}  // namespace util

namespace format {
namespace mpegg_rec {
class ExternalAlignmentNone : public ExternalAlignment {
   public:
    ExternalAlignmentNone();
    explicit ExternalAlignmentNone(util::BitReader *reader);

    void write(util::BitWriter *writer) const override;
};
}  // namespace mpegg_rec
}  // namespace format

#endif  // GENIE_EXTERNAL_ALIGNMENT_NONE_H
