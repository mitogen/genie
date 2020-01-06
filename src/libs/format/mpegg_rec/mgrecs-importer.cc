/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "mgrecs-importer.h"
#include <util/ordered-section.h>

// ---------------------------------------------------------------------------------------------------------------------

MgrecsImporter::MgrecsImporter(size_t _blockSize, std::istream &_file_1)
    : blockSize(_blockSize), reader(_file_1), record_counter(0) {}

// ---------------------------------------------------------------------------------------------------------------------

bool MgrecsImporter::pump(size_t id) {
    format::mpegg_rec::MpeggChunk chunk;
    {
        OrderedSection section(&lock, id);
        for (size_t i = 0; i < blockSize; ++i) {
            format::mpegg_rec::MpeggRecord rec(reader);
            if(!reader.isGood()) {
                flowOut(std::move(chunk), record_counter++);
                return false;
            }
            chunk.emplace_back(std::move(rec));
        }
    }
    flowOut(std::move(chunk), record_counter++);

    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void MgrecsImporter::dryIn() { dryOut(); }

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------