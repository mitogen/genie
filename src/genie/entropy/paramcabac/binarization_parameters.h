/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_BINARIZATION_PARAMETERS_H
#define GENIE_BINARIZATION_PARAMETERS_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/util/bitreader.h>
#include <genie/util/bitwriter.h>
#include <genie/util/make-unique.h>
#include <boost/optional/optional.hpp>
#include <memory>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace entropy {
namespace paramcabac {

// ---------------------------------------------------------------------------------------------------------------------

/**
 * ISO 23092-2 Section 8.3.3.1 table 98
 */
class BinarizationParameters {
   private:
    boost::optional<uint8_t> cmax;             //!< : 8; Line 3
    boost::optional<uint8_t> cmax_teg;         //!< : 8; Line 5
    boost::optional<uint8_t> cmax_dtu;         //!< : 8; Line 7
    boost::optional<uint8_t> split_unit_size;  //!< : 4; Line 10
   public:
    enum class BinarizationId : uint8_t {
        BINARY_CODING = 0,
        TRUNCATED_UNARY = 1,
        EXPONENTIAL_GOLOMB = 2,
        SIGNED_EXPONENTIAL_GOMB = 3,
        TRUNCATED_EXPONENTIAL_GOLOMB = 4,
        SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB = 5,
        SPLIT_UNITWISE_TRUNCATED_UNARY = 6,
        SIGNED_SPLIT_UNIT_WISE_TRUNCATED_UNARY = 7,
        DOUBLE_TRUNCATED_UNARY = 8,
        SIGNED_DOUBLE_TRUNCATED_UNARY = 9,
    };

    uint8_t getParameter() const;

    virtual void write(util::BitWriter &writer) const;

    BinarizationParameters();

    BinarizationParameters(BinarizationId binID, util::BitReader &reader);

    BinarizationParameters(const BinarizationId &_binarization_id, uint8_t param);

    virtual ~BinarizationParameters() = default;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace paramcabac
}  // namespace entropy
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_BINARIZATION_PARAMETERS_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------