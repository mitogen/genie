/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_PART1_DATASET_H
#define GENIE_PART1_DATASET_H

#include <memory>
#include <vector>

#include "dataset_header.h"
#include "genie/format/mgb/data-unit-factory.h"
#include "genie/format/mpegg_p1/access_unit_p1.h"
#include "genie/format/mpegg_p1/dataset_parameter_set.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

/**
 *
 */
class DTMetadata {
   public:
    /**
     *
     */
    DTMetadata();

   private:
    std::vector<uint8_t> DT_metadata_value;  //!<
};

/**
 *
 */
class DTProtection {
   public:
    /**
     *
     */
    DTProtection();

   private:
    std::vector<uint8_t> DT_protection_value;  //!<
};

class Dataset {
   private:
    /**
     * ISO 23092-1 Section 6.5.2 table 18
     *
     * ------------------------------------------------------------------------------------------------------------- */
    DatasetHeader dataset_header;

    // ISO 23092-1 Section 6.5.2.3 - specification 23092-3
    // optional
    std::unique_ptr<DTMetadata> DT_metadata;

    // ISO 23092-1 Section 6.5.2.3 - specification 23092-3
    // optional
    std::unique_ptr<DTProtection> DT_protection;

    std::vector<DatasetParameterSet> dataset_parameter_sets;

    // TODO(Yeremia): Master Index Table
    //MasterIndexTable master_index_table;

    std::vector<AccessUnit> access_units;

    // TODO(Yeremia): Descriptor Stream
    //std::vector<DescriptorStream> descriptor_streams;

   public:

    Dataset(const uint16_t dataset_ID);

    /**
     *
     * @param dataUnitFactory
     * @param accessUnits_p2
     * @param dataset_ID
     */
    Dataset(const genie::format::mgb::DataUnitFactory& dataUnitFactory,
            std::vector<genie::format::mgb::AccessUnit>& accessUnits_p2, const uint16_t dataset_ID);

    uint64_t getLength() const;

    uint16_t getDatasetParameterSetDatasetID() const;

    uint8_t getDatasetParameterSetDatasetGroupID() const;

    const DatasetHeader& getDatasetHeader() const;

    const std::vector<DatasetParameterSet>& getDatasetParameterSets() const;

    void setDatasetGroupId(uint8_t group_id);
    void setDatasetParameterSetsGroupId(uint8_t ps_group_id);

    void write(genie::util::BitWriter& bit_writer) const;

};

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie

#endif  // GENIE_PART1_DATASET_H
