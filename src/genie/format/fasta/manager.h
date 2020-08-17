/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_MANAGER_H
#define GENIE_MANAGER_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/core/reference-source.h>

#include <set>

#include "reader.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace fasta {

/**
 *
 */
class Manager : public core::ReferenceSource {
    FastaReader reader;
    std::mutex readerMutex;

   private:
    std::vector<std::unique_ptr<core::Reference>> generateRefHandles();

   public:
    /**
     *
     * @param fasta
     * @param fai
     * @param mgr
     */
    Manager(std::istream& fasta, std::istream& fai, core::ReferenceManager* mgr);

    /**
     *
     * @return
     */
    std::set<std::string> getSequences() const;

    /**
     *
     * @param seq
     * @return
     */
    uint64_t getLength(const std::string& seq) const;

    /**
     *
     * @param sequence
     * @param start
     * @param end
     * @return
     */
    std::string getRef(const std::string& sequence, uint64_t start, uint64_t end);
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace fasta
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_MANAGER_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------