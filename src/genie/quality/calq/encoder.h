
/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_CALQENCODER_H
#define GENIE_CALQENCODER_H

// ---------------------------------------------------------------------------------------------------------------------

#include <genie/core/cigar-tokenizer.h>
#include <genie/core/qv-encoder.h>
#include <genie/quality/paramqv1/qv_coding_config_1.h>
#include <genie/util/stringview.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace quality {
namespace calq {

/**
 *
 */
class Encoder : public core::QVEncoder {
   private:
   uint8_t polyploidy;
   std::string calq_version;

    /**
     *
     * @param rec
     * @param param
     * @param desc
     */
    static void setUpParameters(const core::record::Chunk& rec, paramqv1::QualityValues1& param,
                                core::AccessUnit::Descriptor& desc, paramqv1::ParameterSet& set);

    /**
     *
     * @param s
     * @param ecigar
     * @param desc
     */
    static void encodeAlignedSegment(const core::record::Segment& s, const std::string& ecigar,
                                     core::AccessUnit::Descriptor& desc);

    /**
     *
     * @param s
     * @param desc
     */
    static void encodeUnalignedSegment(const core::record::Segment& s, core::AccessUnit::Descriptor& desc);

   public:
    /**
     *
     * @param rec
     * @return
     */
    core::QVEncoder::QVCoded process(const core::record::Chunk& rec) override;

    /**
     *
     */
    Encoder();

    /**
     *
     * @param pploidy
     * @param calq_ver
     */
    Encoder(const uint8_t& pploidy, const std::string& calq_ver);
    
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace calq
}  // namespace quality
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_ENCODER_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------