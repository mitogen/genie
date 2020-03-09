/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_READ_ENCODER_H
#define GENIE_READ_ENCODER_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/util/drain.h>
#include <genie/util/source.h>
#include "access-unit-raw.h"
#include "qv-encoder.h"
#include "name-encoder.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

/**
 * @brief The basic interface for modules encoding the plain read data
 */
class ReadEncoder : public util::Drain<record::Chunk>, public util::Source<AccessUnitRaw> {
   protected:
    QVEncoder* qvcoder;
    NameEncoder* namecoder;

   public:
    explicit ReadEncoder(QVEncoder* coder, NameEncoder* ncoder) : qvcoder(coder), namecoder(ncoder) {}

    virtual void setQVCoder(QVEncoder* coder) { qvcoder = coder; }

    virtual void setNameCoder(NameEncoder* coder) { namecoder = coder; }

    /**
     * @Brief For polymorphic destruction
     */
    ~ReadEncoder() override = default;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_READ_ENCODER_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------