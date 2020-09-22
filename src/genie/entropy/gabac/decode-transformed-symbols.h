/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GABAC_TRANSFORMED_SYMBOLS_DECODER_H
#define GABAC_TRANSFORMED_SYMBOLS_DECODER_H

// ---------------------------------------------------------------------------------------------------------------------

//#include <genie/util/data-block.h>
//#include "gabac.h"
//#include "reader.h"
#include "decode-transformed-subseq.h"
#include "context-selector.h"
#include "luts-subsymbol-transform.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace gabac {

/**
 *
 */
class TransformedSymbolsDecoder {
   public:

    /**
     *
     * @param
     */
    explicit TransformedSymbolsDecoder(util::DataBlock *bitstream,
                                       const paramcabac::TransformedSubSeq &trnsfSubseqConf,
                                       const unsigned int numEncodedSymbols);

    /**
     *
     * @param
     */
    ~TransformedSymbolsDecoder();

    /**
     * @brief
     * @param
     * @return
     */
    uint64_t decodeNextSymbol(uint64_t *depSymbol);

    /**
     * @brief
     * @return the number of symbols available.
     */
    size_t symbolsAvail() const;

   private:
    /**
     * @brief
     * @param
     * @return
     */
    uint64_t decodeNextSymbolOrder0();

    /**
     * @brief
     * @param
     * @return
     */
    uint64_t decodeNextSymbolOrder1(uint64_t *depSymbol);

    /**
     * @brief
     * @param
     * @return
     */
    uint64_t decodeNextSymbolOrder2();

    const unsigned int numEncodedSymbols;
    unsigned int numDecodedSymbols;
    const paramcabac::BinarizationParameters::BinarizationId binID;
    const paramcabac::StateVars &stateVars;

    Reader *reader;
    ContextSelector *ctxSelector;
    LUTsSubSymbolTransform *invLutsSubsymTrnsfm;

    binFunc binarizor;
    std::vector<unsigned int> binParams;
    bool diffEnabled;
    bool customCmaxTU;
    uint64_t defaultCmax;

    uint8_t outputSymbolSize;
    uint8_t codingSubsymSize;
    uint8_t codingOrder;
    uint64_t subsymMask;
    uint8_t numLuts;
    uint8_t numPrvs;

};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GABAC_TRANSFORMED_SYMBOLS_DECODER_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------