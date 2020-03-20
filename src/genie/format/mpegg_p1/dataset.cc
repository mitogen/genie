/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "dataset.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

Dataset::Dataset(const genie::format::mgb::DataUnitFactory& dataUnitFactory,
                 std::vector<genie::format::mgb::AccessUnit>& accessUnits_p2, const uint16_t dataset_ID)
    : dataset_header(dataset_ID)  // TODO: dataset_header constructor
{
    for (unsigned int i = 0;; ++i) {
        try {  // to iterate over dataUnitFactory.getParams(i)
            dataset_parameter_sets.emplace_back(std::move(dataUnitFactory.getParams(i)), dataset_ID);
        } catch (const std::out_of_range&) {
            // std::cout << "Got " << i << " ParameterSet/s from DataUnitFactory" << std::endl; //debuginfo
            break;
        }
    }

    for (auto& au : accessUnits_p2) {
        access_units.emplace_back(std::move(au));
    }
}

void Dataset::setDatasetParameterSetsGroupId(uint8_t GroupId) {
    for (auto& ps : dataset_parameter_sets) {
        ps.setDatasetGroupId(GroupId);
    }
}

const uint64_t Dataset::getLength() const
{
    uint64_t length = 12; //gen_info
    length += dataset_header.getLength();

    for (auto const& it: dataset_parameter_sets) {
        length += it.getSize();
    }

    for (auto const& it: access_units) {
        length += it.getSize();
    }

    return length;
}

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie