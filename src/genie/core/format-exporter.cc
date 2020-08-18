/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "format-exporter.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

// ---------------------------------------------------------------------------------------------------------------------

stats::PerfStats& FormatExporter::getStats() { return stats; }

// ---------------------------------------------------------------------------------------------------------------------

void FormatExporter::skipIn(const util::Section&) {}

// ---------------------------------------------------------------------------------------------------------------------

void FormatExporter::flushIn(uint64_t&) {}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------