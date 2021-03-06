/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_ENTROPY_GABAC_BINARY_ARITHMETIC_DECODER_H_
#define SRC_GENIE_ENTROPY_GABAC_BINARY_ARITHMETIC_DECODER_H_

// ---------------------------------------------------------------------------------------------------------------------

#include "genie/entropy/gabac/bit-input-stream.h"
#include "genie/entropy/gabac/context-model.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace gabac {

/**
 *
 */
class BinaryArithmeticDecoder {
 public:
    /**
     *
     * @param bitInputStream
     */
    explicit BinaryArithmeticDecoder(const BitInputStream& bitInputStream);

    /**
     *
     * @param contextModel
     * @return
     */
    unsigned int decodeBin(ContextModel* contextModel);

    /**
     *
     * @param numBins
     * @return
     */
    unsigned int decodeBinsEP(unsigned int numBins);

    /**
     *
     */
    void decodeBinTrm();

    /**
     *
     */
    void reset();

    /**
     *
     */
    void start();

    /**
     *
     * @return
     */
    size_t close();

 private:
    BitInputStream m_bitInputStream;  //!<
    int m_numBitsNeeded = 0;          //!<
    unsigned int m_range = 0;         //!<
    unsigned int m_value = 0;         //!<
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_ENTROPY_GABAC_BINARY_ARITHMETIC_DECODER_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
