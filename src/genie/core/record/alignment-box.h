/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_RECORD_ALIGNMENT_BOX_H_
#define SRC_GENIE_CORE_RECORD_ALIGNMENT_BOX_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <vector>
#include "genie/core/record/alignment-split.h"
#include "genie/core/record/alignment.h"
#include "genie/core/record/class-type.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"
#include "genie/util/make-unique.h"
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {

/**
 *ExternalAlignment
 */
class AlignmentBox {
    uint64_t mapping_pos{};                                           //!<
    Alignment alignment;                                              //!<
    std::vector<std::unique_ptr<AlignmentSplit>> splitAlignmentInfo;  //!<

 public:
    /**
     *
     * @param writer
     */
    void write(util::BitWriter& writer) const;

    /**
     *
     * @param type
     * @param as_depth
     * @param number_of_template_segments
     * @param reader
     */
    explicit AlignmentBox(ClassType type, uint8_t as_depth, uint8_t number_of_template_segments,
                          util::BitReader& reader);

    /**
     *
     * @param _mapping_pos
     * @param _alignment
     */
    AlignmentBox(uint64_t _mapping_pos, Alignment&& _alignment);

    /**
     *
     * @param container
     */
    AlignmentBox(const AlignmentBox& container);

    /**
     *
     * @param container
     */
    AlignmentBox(AlignmentBox&& container) noexcept;

    /**
     *
     */
    ~AlignmentBox() = default;

    /**
     *
     * @param container
     * @return
     */
    AlignmentBox& operator=(const AlignmentBox& container);

    /**
     *
     * @param container
     * @return
     */
    AlignmentBox& operator=(AlignmentBox&& container) noexcept;

    /**
     *
     * @param _alignment
     */
    void addAlignmentSplit(std::unique_ptr<AlignmentSplit> _alignment);

    /**
     *
     */
    AlignmentBox();

    /**
     *
     * @return
     */
    uint64_t getPosition() const;

    /**
     *
     * @return
     */
    const Alignment& getAlignment() const;

    /**
     *
     * @return
     */
    const std::vector<std::unique_ptr<AlignmentSplit>>& getAlignmentSplits() const;

    /**
     *
     * @return
     */
    uint8_t getNumberOfTemplateSegments() const;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_RECORD_ALIGNMENT_BOX_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
