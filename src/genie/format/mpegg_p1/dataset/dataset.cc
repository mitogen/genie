/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "dataset.h"

namespace genie {
namespace format {
namespace mpegg_p1 {

// ---------------------------------------------------------------------------------------------------------------------

uint64_t DTMetadata::getLength() const {
    uint64_t len = 12;  // gen_info

    len += DT_metadata_value.size();

    return len;
}

// ---------------------------------------------------------------------------------------------------------------------

void DTMetadata::write(genie::util::BitWriter &bit_writer) const {
    // KLV (Key Length Value) format

    // Key of KVL format
    bit_writer.write("dtmd");

    // Length of KVL format
    bit_writer.write(getLength(), 64);

    for (auto val : DT_metadata_value){
        bit_writer.write(val, 8);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uint64_t DTProtection::getLength() const {
    uint64_t len = 12;  // gen_info

    len += DT_protection_value.size();

    return len;
}

// ---------------------------------------------------------------------------------------------------------------------

void DTProtection::write(genie::util::BitWriter &bit_writer) const {
    // KLV (Key Length Value) format

    // Key of KVL format
    bit_writer.write("dtpr");

    // Length of KVL format
    bit_writer.write(getLength(), 64);

    for (auto val : DT_protection_value){
        bit_writer.write(val, 8);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

//Dataset::Dataset(uint16_t ID, const genie::format::mgb::DataUnitFactory& dataUnitFactory,
//                 std::vector<genie::format::mgb::AccessUnit>& accessUnits_p2)
//    : dataset_group_ID(group_ID),
//      dataset_ID(ID),
//      version("XXXX"), // TODO (Yeremia): Version
//      byte_offset_size_flag(_byte_offset_size_flag),
//      non_overlapping_AU_range_flag(_non_overlapping_AU_range_flag),
//      pos_40_bits_flag(_pos_40_bits_flag),
//      dataset_type(_dataset_type),
//      alphabet_ID(_alphabet_ID),
//      num_U_access_units(_num_U_access_units){
//
//    for (unsigned int i = 0;; ++i) {
//        try {  // to iterate over dataUnitFactory.getParams(i)
//            dataset_parameter_sets.emplace_back(ID, std::move(dataUnitFactory.getParams(i)));
//        } catch (const std::out_of_range&) {
//            // std::cout << "Got " << i << " ParameterSet/s from DataUnitFactory" << std::endl; //debuginfo
//            break;
//        }
//    }
//
//    for (auto& au : accessUnits_p2) {
//        access_units.emplace_back(std::move(au));
//    }
//}

// ---------------------------------------------------------------------------------------------------------------------

void Dataset::setGroupId(uint8_t group_ID) {dataset_group_ID = group_ID;}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t Dataset::getGroupID() const { return dataset_group_ID; }

// ---------------------------------------------------------------------------------------------------------------------

void Dataset::setID(uint16_t ID) {dataset_ID = ID;}

// ---------------------------------------------------------------------------------------------------------------------

uint16_t Dataset::getID() const { return dataset_ID; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<DatasetParameterSet>& Dataset::getParameterSets() const { return dataset_parameter_sets; }

// ---------------------------------------------------------------------------------------------------------------------

uint64_t Dataset::getHeaderLength() const {
    /// gen_info
    uint64_t bitlength = 12 * 8;

    /// dataset_group_ID, dataset_ID, version
    bitlength += (1 + 2 + 4) * 8;

    /// byte_offset_size_flag, non_overlapping_AU_range_flag, pos_40_bits_flag
    bitlength += 1 + 1 + 1;

    /// block_header_flag, MIT_flag, CC_mode_flag, ordered_blocks_flag
    /// ClassInfo (num_classes, clid[], um_descriptors[], descriptor_ID[][])
    bitlength += block_config.getBitLength();

    /// dataset_type u(4), alphabet_ID(8), num_U_access_units u(32)
    bitlength += 4 + 8 + 32;

    if (num_U_access_units > 0){
        /// u_access_unit_info
        bitlength += u_access_unit_info->getBitLength();
    }

    /// seq_count, reference_ID, seq_ID[], seq_blocks[], tflag[], thres[]
    bitlength += seq_info.getLength();

    // Byte aligned
    bitlength += bitlength % 8;

    return bitlength / 8;
}

// ---------------------------------------------------------------------------------------------------------------------

void Dataset::writeHeader(util::BitWriter& bit_writer) const {
    // KLV (Key Length Value) format

    // Key of KVL format
    bit_writer.write("dthd");

    // Length of KVL format
    bit_writer.write(getHeaderLength(), 64);

    // Value of KVL format:

    // dataset_group_ID u(8)
    bit_writer.write(dataset_group_ID, 8);

    // dataset_IDs u(16)
    bit_writer.write(dataset_ID, 16);

    // version c(4)
    UTILS_DIE_IF(version.size() != 4, "Invalid version");
    bit_writer.write(version);

    // byte_offset_size_flag u(1)
    if (byte_offset_size_flag == ByteOffsetSizeFlag::ON){
        bit_writer.write(1, 1);
    } else {
        bit_writer.write(0, 1);
    }

    // non_overlapping_AU_range_flag u(1)
    bit_writer.write(non_overlapping_AU_range_flag, 1);

    // pos_40_bits_flag u(1)
    if (pos_40_bits_flag == Pos40SizeFlag::ON) {
        bit_writer.write(1, 1);
    } else {
        bit_writer.write(0, 1);
    }

    // block_header_flag, MIT_flag, CC_mode_flag, ordered_blocks_flag
    block_config.write(bit_writer);

    // seq_count u(16)
    // reference_ID, seq_ID[seq] and seq_blocks[seq]
    seq_info.write(bit_writer);

    // dataset_type u(4)
    switch(dataset_type) {
        case core::parameter::DataUnit::DatasetType::NON_ALIGNED:
            bit_writer.write(0, 4);
            break;
        case core::parameter::DataUnit::DatasetType::ALIGNED:
            bit_writer.write(1, 4);
            break;
        case core::parameter::DataUnit::DatasetType::REFERENCE:
            bit_writer.write(2, 4);
            break;
    }

    // num_classes, clid[ci], num_descriptors[ci], descriptor_ID[ci][di]
    block_config.writeClassInfos(bit_writer);

    // alphabet_ID u(8)
    bit_writer.write(alphabet_ID, 8);

    // num_U_access_units u(32)
    bit_writer.write(num_U_access_units, 32);

    if (num_U_access_units > 0){
        UTILS_DIE_IF(u_access_unit_info == nullptr, "No u_access_unit_info stored!");

        //num_U_clusters, multiple_signature_base, U_signature_size, U_signature_constant_length, U_signature_length
        u_access_unit_info->write(bit_writer);
    }

    // tflag[], thres[]
    seq_info.writeThres(bit_writer);

    // nesting zero bit
    bit_writer.flush();
}

// ---------------------------------------------------------------------------------------------------------------------

uint64_t Dataset::getLength() const {
    uint64_t len = 12;  // KVL

    len += getHeaderLength();

    // DT_metadata
    if (DT_metadata != nullptr){
        DT_metadata->getLength();
    }

    // DT_protection
    if (DT_protection != nullptr){
        DT_protection->getLength();
    }

    // write master_index_table depending on MIT_FLAG
    if (block_config.getMITFlag()){
        master_index_table->getLength();
    }

    for (auto const& it : dataset_parameter_sets) {
        // dataset_group_ID u(8) + dataset_ID u(16)
        len += sizeof(uint8_t) + sizeof(uint16_t);
        len += it.getLength();
    }

    for (auto const& it : access_units) {
        len += it.getLength();
    }

    return len;
}

// ---------------------------------------------------------------------------------------------------------------------

void Dataset::write(util::BitWriter& bit_writer) const {
    // KLV (Key Length Value) format

    // Key of KVL format
    bit_writer.write("dtcn");

    // Length of KVL format
    bit_writer.write(getLength(), 64);

    // Value of KVL format:

    // dataset_header
    writeHeader(bit_writer);

    // DT_metadata
    if (DT_metadata != nullptr){
        DT_metadata->write(bit_writer);
    }

    // DT_protection
    if (DT_protection != nullptr){
        DT_protection->write(bit_writer);
    }

    // write master_index_table depending on MIT_FLAG
    if (block_config.getMITFlag()){
        master_index_table->write(bit_writer);
    }

    // dataset_parameter_set[]
    for (auto const& ps : dataset_parameter_sets) {
        ps.write(bit_writer);
    }

    for (auto &ac: access_units){
        ac.write(bit_writer);
    }

    // TODO (Yeremia): write descriptor_stream depending on block_header_flag
//    if (block_header_flag == 0){
//        for (auto &ds: descriptor_streams){
//            dc.write(bit_writer);
//        }
//    }

    // TODO (Yeremia): implement write of Dataset
    UTILS_DIE("Not implemented yet");
}

}  // namespace mpegg_p1
}  // namespace format
}  // namespace genie