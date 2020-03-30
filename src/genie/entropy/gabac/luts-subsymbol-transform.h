/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GABAC_LUT_SUBSYMBOL_TRANSFORM_H_
#define GABAC_LUT_SUBSYMBOL_TRANSFORM_H_

#include "reader.h"
#include <genie/entropy/paramcabac/state_vars.h>

namespace genie {
namespace util {
class DataBlock;
}
}  // namespace genie

namespace genie {
namespace entropy {
namespace gabac {

struct LutEntry {
    uint64_t value;
    uint64_t index;
    uint64_t freq; // used only in encoder
};

struct LutRow {
    std::vector<LutEntry> entries;  // max size numAlphaSubsym
    uint64_t numMaxElems;           // max value == numAlphaSubsym
};

typedef std::vector<LutRow>    LutOrder1;
typedef std::vector<LutOrder1> LutOrder2;

class LUTsSubSymbolTransformation {

    public:
    void decodeLUTs(const paramcabac::SupportValues& supportVals, const paramcabac::StateVars& stateVars, Reader &reader);

    private:

    void setupLutsO1(uint8_t numSubsyms, uint64_t numAlphaSubsym);
    void setupLutsO2(uint8_t numSubsyms, uint64_t numAlphaSubsym);

    std::vector<LutOrder1> lutsO1;  // max size numSubsyms
    std::vector<LutOrder2> lutsO2;  // max size numSubsyms
};

}  // namespace gabac
}  // namespace entropy
}  // namespace genie
#endif  // GABAC_LUT_SUBSYMBOL_TRANSFORM_H_
