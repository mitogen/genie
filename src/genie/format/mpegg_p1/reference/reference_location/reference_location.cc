#include "reference_location.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

ReferenceLocation::ReferenceLocation()
    : external_ref_flag(ReferenceLocation::Flag::INTERNAL) {}

ReferenceLocation::ReferenceLocation(ReferenceLocation::Flag _flag)
    : external_ref_flag(_flag){}

bool ReferenceLocation::isExternal() const { return external_ref_flag == Flag::EXTERNAL; }

ReferenceLocation::Flag ReferenceLocation::getExternalRefFlag() const { return external_ref_flag; }

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie