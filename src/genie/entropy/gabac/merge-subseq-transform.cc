/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include <algorithm>
#include <cassert>
#include <iostream>

#include <genie/util/block-stepper.h>
#include <genie/util/data-block.h>
#include "exceptions.h"
#include "merge-subseq-transform.h"

namespace genie {
namespace entropy {
namespace gabac {

void transformMergeCoding(const paramcabac::Subsequence& subseqCfg, std::vector<util::DataBlock> *const transformedSubseqs) {
    assert(transformedSubseqs != nullptr);
    const paramcabac::TransformedParameters& trnsfSubseqParams = subseqCfg.getTransformParameters();
    const uint16_t subseqCount = trnsfSubseqParams.getParam();
    const std::vector<uint8_t> subseqShiftSizes = trnsfSubseqParams.getMergeCodingShiftSize();
    const std::vector<paramcabac::TransformedSubSeq>& trnsfCfgs = subseqCfg.getTransformSubseqCfgs();

    if (subseqCount <= 0) {
        GABAC_DIE("invalid subseq_count for merge coding");
    }

    // Prepare internal and output data structures
    util::DataBlock symbols(0, 1);
    symbols.swap(&(*transformedSubseqs)[0]);
    transformedSubseqs->resize(subseqCount);

    const uint64_t symbolsCount = symbols.size();
    std::vector<uint64_t> trnsSubseqMasks(0, subseqCount);
    for (uint64_t ts = 0; ts < subseqCount; ts++) {
        const uint8_t codingSubsymSize = trnsfCfgs[ts].getSupportValues().getCodingSubsymSize();
        trnsSubseqMasks[ts] = (1u<<codingSubsymSize)-1;
        (*transformedSubseqs)[ts].setWordSize(paramcabac::StateVars::getLgWordSize(trnsfCfgs[ts].getSupportValues().getOutputSymbolSize()));
        (*transformedSubseqs)[ts].resize(symbolsCount);
    }

    // split
    for (uint64_t i = 0; i < symbolsCount; i++) {
        uint64_t symbolValue = symbols.get(i);
        for (uint64_t ts = 0; ts < subseqCount; ts++) {
            (*transformedSubseqs)[ts].set(i, (symbolValue>>subseqShiftSizes[ts]) & trnsSubseqMasks[ts]);
        }
    }

    symbols.clear();
}

void inverseTransformMergeCoding(const paramcabac::Subsequence& subseqCfg, std::vector<util::DataBlock> *const transformedSubseqs) {
    const paramcabac::TransformedParameters& trnsfSubseqParams = subseqCfg.getTransformParameters();
    const uint16_t subseqCount = trnsfSubseqParams.getParam();
    const std::vector<uint8_t> subseqShiftSizes = trnsfSubseqParams.getMergeCodingShiftSize();

    if (subseqCount <= 0 || subseqCount != (*transformedSubseqs).size()) {
        GABAC_DIE("invalid subseq_count for merge coding");
    }

    // Prepare the output data structure
    util::DataBlock symbols(0, 1);
    const uint64_t symbolsCount = (*transformedSubseqs)[0].size();
    symbols.resize(symbolsCount);
    symbols.setWordSize(paramcabac::StateVars::getLgWordSize(subseqShiftSizes[subseqCount-1] +
                                                              subseqCfg.getTransformSubseqCfg(subseqCount-1).getSupportValues().getOutputSymbolSize()));

    for (uint64_t i = 0; i < symbolsCount; i++) {
        uint64_t symbolValue = 0;
        for (uint64_t ts = 0; ts < subseqCount; ts++) {
            symbolValue = (symbolValue << subseqShiftSizes[ts]) | (*transformedSubseqs)[ts].get(i);
        }
        symbols.set(i, symbolValue);
    }

    (*transformedSubseqs).resize(1);
    (*transformedSubseqs)[0].clear();

    symbols.swap(&(*transformedSubseqs)[0]);
}
}  // namespace gabac
}  // namespace entropy
}  // namespace genie