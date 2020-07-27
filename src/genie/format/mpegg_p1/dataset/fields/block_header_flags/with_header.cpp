#include "with_header.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

WithHeader::WithHeader()
    : BlockHeader(true, false),
      CC_mode_flag(false){}

WithHeader::WithHeader(bool _mit_flag, bool _cc_mode_flag)
    : BlockHeader(true, _mit_flag),
      CC_mode_flag(_cc_mode_flag){}


uint64_t WithHeader::getLength() const {
    return BlockHeader::getLength() + 2; // block_header_flag, MIT_flag, CC_mode_flag
}

void WithHeader::write(genie::util::BitWriter& bit_writer) const {

    // block_header_flag u(1)
    BlockHeader::write(bit_writer);

    // MIT_flag u(1)
    bit_writer.write(MIT_flag, 1);

    // CC_mode_flag u(1)
    bit_writer.write(CC_mode_flag, 1);
}


}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie
