/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "exporter.h"

#include <genie/util/thread-manager.h>

#include "fasta-source.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace fasta {

// ---------------------------------------------------------------------------------------------------------------------

Exporter::Exporter(core::ReferenceManager* _refMgr, std::ostream* out, size_t _num_threads)
    : refMgr(_refMgr), outfile(out), num_threads(_num_threads) {}

// ---------------------------------------------------------------------------------------------------------------------

void Exporter::flushIn(size_t&) {
    util::ThreadManager mgr(num_threads);
    FastaSource source(outfile, refMgr);
    std::vector<util::OriginalSource*> vec;
    vec.push_back(&source);
    mgr.setSource(vec);
    mgr.run();
}

// ---------------------------------------------------------------------------------------------------------------------

void Exporter::flowIn(core::record::Chunk&&, const util::Section&) {}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace fasta
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------