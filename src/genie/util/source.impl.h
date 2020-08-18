/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_SOURCE_IMPL_H
#define GENIE_SOURCE_IMPL_H

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace util {

// ---------------------------------------------------------------------------------------------------------------------

template <typename TYPE>
void Source<TYPE>::flowOut(TYPE&& t, const Section& id) {
    drain->flowIn(std::move(t), id);
}

// ---------------------------------------------------------------------------------------------------------------------

template <typename TYPE>
void Source<TYPE>::flushOut(uint64_t& pos) {
    drain->flushIn(pos);
}

// ---------------------------------------------------------------------------------------------------------------------

template <typename TYPE>
void Source<TYPE>::skipOut(const Section& id) {
    drain->skipIn(id);
}

// ---------------------------------------------------------------------------------------------------------------------

template <typename TYPE>
void Source<TYPE>::setDrain(Drain<TYPE>* d) {
    drain = d;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace util
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_SOURCE_IMPL_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
