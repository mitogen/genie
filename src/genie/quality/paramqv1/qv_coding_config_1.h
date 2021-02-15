/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_QUALITY_PARAMQV1_QV_CODING_CONFIG_1_H_
#define SRC_GENIE_QUALITY_PARAMQV1_QV_CODING_CONFIG_1_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <memory>
#include "boost/optional/optional.hpp"
#include "genie/core/parameter/parameter_set.h"
#include "genie/core/parameter/quality-values.h"
#include "genie/quality/paramqv1/parameter_set.h"
#include "genie/util/bitwriter.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace quality {
namespace paramqv1 {

/**
 *
 */
class QualityValues1 : public core::parameter::QualityValues {
 public:
    static const uint8_t MODE_QV1 = 1;  //!<

    /**
     *
     */
    enum class QvpsPresetId { ASCII = 0, OFFSET33_RANGE41 = 1, OFFSET64_RANGE40 = 2 };

    /**
     *
     * @param reader
     */
    explicit QualityValues1(genie::core::GenDesc desc, util::BitReader& reader);

    /**
     *
     * @param _qvps_preset_ID
     * @param _reverse_flag
     */
    explicit QualityValues1(QvpsPresetId _qvps_preset_ID, bool _reverse_flag);

    /**
     *
     * @param _parameter_set_qvps
     */
    void setQvps(ParameterSet&& _parameter_set_qvps);

    /**
     *
     * @param writer
     */
    void write(util::BitWriter& writer) const override;

    /**
     *
     * @return
     */
    std::unique_ptr<QualityValues> clone() const override;

    /**
     *
     * @param reader
     * @return
     */
    static std::unique_ptr<QualityValues> create(genie::core::GenDesc desc, util::BitReader& reader);

    /**
     *
     * @param id
     * @return
     */
    static const Codebook& getPresetCodebook(QvpsPresetId id);

    /**
     *
     * @param type
     * @return
     */
    static std::unique_ptr<core::parameter::QualityValues> getDefaultSet(core::record::ClassType type);

    /**
     *
     * @return
     */
    size_t getNumberCodeBooks() const;

    /**
     *
     * @param id
     * @return
     */
    const Codebook& getCodebook(size_t id) const;

    /**
     *
     * @return
     */
    size_t getNumSubsequences() const override;

    /**
     *
     * @param qv
     * @return
     */
    bool equals(const QualityValues* qv) const override;

 private:
    boost::optional<ParameterSet> parameter_set_qvps;  //!<
    boost::optional<QvpsPresetId> qvps_preset_ID;      //!<
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace paramqv1
}  // namespace quality
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_QUALITY_PARAMQV1_QV_CODING_CONFIG_1_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
