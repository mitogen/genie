#ifndef GENIE_PART1_EXTERNAL_REFERENCE_MPEGG_REF_H
#define GENIE_PART1_EXTERNAL_REFERENCE_MPEGG_REF_H

#include <genie/util/bitreader.h>
#include <genie/util/bitwriter.h>
#include <genie/util/exception.h>
#include <genie/util/runtime-exception.h>

#include "external_reference.h"
#include "checksum.h"
#include "md5.h"
#include "sha256.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

class MpegReference: public ExternalReference {
   private:
    uint8_t dataset_group_ID;
    uint16_t dataset_ID;

    Checksum ref_checksum;

   public:
    /**
     *
     * @param _dataset_group_ID
     * @param _dataset_ID
     * @param _ref_checksum
     */
    MpegReference(uint8_t _dataset_group_ID, uint16_t _dataset_ID, Checksum &&_ref_checksum);

    /**
     *
     * @param reader
     * @param checksum_alg
     */
    MpegReference(util::BitReader& reader, Checksum::Algo checksum_alg);

    /**
     *
     * @return
     */
    uint8_t getDatasetGroupID() const;

    /**
     *
     * @return
     */
    uint16_t getDatasetID() const;

    //TODO(Yeremia): Is it needed?
//    /**
//     *
//     * @param _checksum
//     */
//    void addChecksum(Checksum &&_checksum);

    /**
     *
     * @return
     */
    Checksum::Algo getChecksumAlg() const override;

    /**
     *
     * @return
     */
    uint64_t getLength();

    /**
     *
     * @param writer
     */
    void write(genie::util::BitWriter& writer) override;

};

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

#endif  // GENIE_PART1_EXTERNAL_REFERENCE_MPEGG_REF_H