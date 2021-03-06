/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_FLOWGRAPH_CONVERT_H_
#define SRC_GENIE_CORE_FLOWGRAPH_CONVERT_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <memory>
#include <vector>
#include "genie/core/flowgraph.h"
#include "genie/core/format-exporter.h"
#include "genie/core/format-importer.h"
#include "genie/util/selector.h"
#include "genie/util/thread-manager.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {

/**
 *
 */
class FlowGraphConvert : public FlowGraph {
    genie::util::ThreadManager mgr;                                       //!<
    std::vector<std::unique_ptr<genie::core::FormatImporter>> importers;  //!<
    std::unique_ptr<genie::core::Classifier> classifier;                  //!<
    std::unique_ptr<genie::core::ReferenceManager> refMgr;                //!<

    std::vector<std::unique_ptr<genie::core::FormatExporter>> exporters;     //!<
    genie::util::SelectorHead<genie::core::record::Chunk> exporterSelector;  //!<

 public:
    /**
     *
     * @param threads
     */
    explicit FlowGraphConvert(size_t threads);

    /**
     *
     * @return
     */
    ReferenceManager& getRefMgr();

    /**
     *
     * @param dat
     */
    void addImporter(std::unique_ptr<genie::core::FormatImporter> dat);

    /**
     *
     * @param dat
     * @param index
     */
    void setImporter(std::unique_ptr<genie::core::FormatImporter> dat, size_t index);

    /**
     *
     * @param dat
     */
    void addExporter(std::unique_ptr<genie::core::FormatExporter> dat);

    /**
     *
     * @param dat
     * @param index
     */
    void setExporter(std::unique_ptr<genie::core::FormatExporter> dat, size_t index);

    /**
     *
     * @param fun
     */
    void setExporterSelector(const std::function<size_t(const genie::core::record::Chunk&)>& fun);

    /**
     *
     * @param _classifier
     */
    void setClassifier(std::unique_ptr<genie::core::Classifier> _classifier);

    /**
     *
     */
    void run() override;

    /**
     *
     * @return
     */
    core::stats::PerfStats getStats() override;

    /**
     *
     * @param abort
     */
    void stop(bool abort) override;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_FLOWGRAPH_CONVERT_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
