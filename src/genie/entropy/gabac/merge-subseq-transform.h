/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_ENTROPY_GABAC_MERGE_SUBSEQ_TRANSFORM_H_
#define SRC_GENIE_ENTROPY_GABAC_MERGE_SUBSEQ_TRANSFORM_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <vector>
#include "genie/entropy/paramcabac/subsequence.h"
#include "genie/util/data-block.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace gabac {

/**
 *
 * @param subseqCfg
 * @param transformedSubseqs
 */
void transformMergeCoding(const paramcabac::Subsequence& subseqCfg,
                          std::vector<util::DataBlock>* const transformedSubseqs);

/**
 *
 * @param subseqCfg
 * @param transformedSubseqs
 */
void inverseTransformMergeCoding(const paramcabac::Subsequence& subseqCfg,
                                 std::vector<util::DataBlock>* const transformedSubseqs);

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_ENTROPY_GABAC_MERGE_SUBSEQ_TRANSFORM_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
