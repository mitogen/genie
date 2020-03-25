/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "dataset_parameter_set.h"

namespace genie {
namespace format {
namespace mpegg_p1 {
DatasetParameterSet::DatasetParameterSet(const core::parameter::ParameterSet&& parameterSet)
    : parameterSet_p2(std::move(parameterSet)) {
    // TODO: add dataset_group_ID and dataset_ID
}

DatasetParameterSet::DatasetParameterSet(const core::parameter::ParameterSet&& parameterSet,
                                         const uint8_t x_dataset_group_ID, uint16_t x_dataset_ID)
    : dataset_group_ID(x_dataset_group_ID), dataset_ID(x_dataset_ID), parameterSet_p2(std::move(parameterSet)) {}

DatasetParameterSet::DatasetParameterSet(const core::parameter::ParameterSet&& parameterSet,
                                         const uint16_t x_dataset_ID)
    : dataset_ID(x_dataset_ID), parameterSet_p2(std::move(parameterSet)) {}

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie