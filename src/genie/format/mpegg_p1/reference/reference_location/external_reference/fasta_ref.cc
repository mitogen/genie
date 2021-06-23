/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/format/mpegg_p1/reference/reference_location/external_reference/fasta_ref.h"
#include "genie/format/mpegg_p1/reference/reference_location/external_reference/checksum.h"
#include "genie/format/mpegg_p1/reference/reference_location/external_reference/md5.h"
#include "genie/format/mpegg_p1/reference/reference_location/external_reference/sha256.h"
#include "genie/util/exception.h"
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace mpegg_p1 {

// ---------------------------------------------------------------------------------------------------------------------

FastaReference::FastaReference() : ExternalReference(ExternalReference::Type::FASTA_REF) {}

// ---------------------------------------------------------------------------------------------------------------------

FastaReference::FastaReference(util::BitReader &reader, Checksum::Algo checksum_alg, uint16_t seq_count) {
    switch (checksum_alg) {
        case Checksum::Algo::MD5: {
            for (auto i = 0; i < seq_count; i++) {
                checksums.push_back(Md5(reader));
            }
            break;
        }
        case Checksum::Algo::SHA256: {
            for (auto i = 0; i < seq_count; i++) {
                checksums.push_back(Sha256(reader));
            }
            break;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

FastaReference::FastaReference(std::vector<Checksum> &&_checksums)
    : ExternalReference(ExternalReference::Type::FASTA_REF), checksums(_checksums) {
    for (auto &checksum : checksums) {
        UTILS_DIE_IF(checksums.front().getType() != checksum.getType(), "Different checksum algorithm");
    }
}

// ---------------------------------------------------------------------------------------------------------------------

Checksum::Algo FastaReference::getChecksumAlg() const { return checksums.front().getType(); }

// ---------------------------------------------------------------------------------------------------------------------

void FastaReference::write(util::BitWriter &bit_writer) {
    for (auto &checksum : checksums) {
        checksum.write(bit_writer);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
