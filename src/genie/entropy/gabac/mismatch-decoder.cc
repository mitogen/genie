/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

// ---------------------------------------------------------------------------------------------------------------------

#include "mismatch-decoder.h"
#include <genie/util/make-unique.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace gabac {

// ---------------------------------------------------------------------------------------------------------------------

MismatchDecoder::MismatchDecoder(util::DataBlock &&d) : data(std::move(d)) {
}

// ---------------------------------------------------------------------------------------------------------------------

uint64_t MismatchDecoder::decodeMismatch(uint64_t ref) { return 0; }

// ---------------------------------------------------------------------------------------------------------------------

bool MismatchDecoder::dataLeft() const {
    return position < data.size();
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::MismatchDecoder> MismatchDecoder::copy() const {
    util::DataBlock d = data;
    auto ret = util::make_unique<MismatchDecoder>(std::move(d));
    ret->position = position;
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------