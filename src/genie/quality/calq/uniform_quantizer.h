#ifndef CALQ_UNIFORM_QUANTIZER_H_
#define CALQ_UNIFORM_QUANTIZER_H_

// -----------------------------------------------------------------------------

#include "quantizer.h"

// -----------------------------------------------------------------------------

namespace genie{
namespace quality{
namespace calq {

// -----------------------------------------------------------------------------

class UniformQuantizer : public Quantizer
{
 public:
    UniformQuantizer(const int& valueMin,
                     const int& valueMax,
                     const int& nrSteps
    );
    ~UniformQuantizer() override;
};

// -----------------------------------------------------------------------------

}  // namespace calq
}  // namespace quality
}  // namespace genie

// -----------------------------------------------------------------------------

#endif  // CALQ_UNIFORM_QUANTIZER_H_

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
