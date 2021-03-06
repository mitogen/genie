/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_FLOWGRAPH_H_
#define SRC_GENIE_CORE_FLOWGRAPH_H_

// ---------------------------------------------------------------------------------------------------------------------

#include "genie/core/stats/perf-stats.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

/**
 *
 */
class FlowGraph {
 public:
    /**
     *
     */
    virtual void run() = 0;

    /**
     *
     */
    virtual void stop(bool) {}

    /**
     *
     * @return
     */
    virtual core::stats::PerfStats getStats() = 0;

    /**
     *
     */
    virtual ~FlowGraph() = default;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_FLOWGRAPH_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
