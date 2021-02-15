/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_CORE_RECORD_RECORD_H_
#define SRC_GENIE_CORE_RECORD_RECORD_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "genie/core/constants.h"
#include "genie/core/record/alignment-box.h"
#include "genie/core/record/alignment-external.h"
#include "genie/core/record/alignment-shared-data.h"
#include "genie/core/record/alignment_split/same-rec.h"
#include "genie/core/record/class-type.h"
#include "genie/core/record/segment.h"
#include "genie/core/stats/perf-stats.h"
#include "genie/util/bitreader.h"
#include "genie/util/bitwriter.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace core {
namespace record {

/**
 *
 */
class Record {
 private:
    uint8_t number_of_template_segments{};    //!<
    ClassType class_ID{ClassType::NONE};      //!<
    bool read_1_first{};                      //!<
    AlignmentSharedData sharedAlignmentInfo;  //!<
    uint8_t qv_depth{};                       //!<
    std::string read_name;                    //!<
    std::string read_group;                   //!<
    std::vector<Segment> reads;               //!<
    std::vector<AlignmentBox> alignmentInfo;  //!<
    uint8_t flags{};                          //!<

    std::unique_ptr<AlignmentExternal> moreAlignmentInfo;  //!<

 public:
    /**
     *
     */
    Record();

    /**
     *
     * @param _number_of_template_segments
     * @param _auTypeCfg
     * @param _read_name
     * @param _read_group
     * @param _flags
     */
    Record(uint8_t _number_of_template_segments, ClassType _auTypeCfg, std::string&& _read_name,
           std::string&& _read_group, uint8_t _flags, bool _is_read_1_first = true);

    /**
     *
     * @param rec
     */
    Record(const Record& rec);

    /**
     *
     * @param rec
     */
    Record(Record&& rec) noexcept;

    /**
     *
     */
    ~Record() = default;

    /**
     *
     * @param rec
     * @return
     */
    Record& operator=(const Record& rec);

    /**
     *
     * @param rec
     * @return
     */
    Record& operator=(Record&& rec) noexcept;

    /**
     *
     * @param reader
     */
    explicit Record(util::BitReader& reader);

    /**
     *
     * @param rec
     */
    void addSegment(Segment&& rec);

    /**
     *
     * @param _seq_id
     * @param rec
     */
    void addAlignment(uint16_t _seq_id, AlignmentBox&& rec);

    /**
     *
     * @param index
     * @return
     */
    std::vector<Segment>& getSegments();

    /**
     *
     * @return
     */
    const std::vector<Segment>& getSegments() const;

    /**
     *
     * @return
     */
    size_t getNumberOfTemplateSegments() const;

    /**
     *
     * @param index
     * @return
     */
    const std::vector<AlignmentBox>& getAlignments() const;

    /**
     *
     * @param writer
     */
    void write(util::BitWriter& writer) const;

    /**
     *
     * @return
     */
    uint8_t getFlags() const;

    /**
     *
     * @return
     */
    ClassType getClassID() const;

    /**
     *
     * @return
     */
    const std::string& getName() const;

    /**
     *
     * @param _name
     */
    void setName(const std::string& _name);

    /**
     *
     * @param type
     */
    void setClassType(ClassType type);

    /**
     *
     * @return
     */
    const std::string& getGroup() const;

    /**
     *
     * @return
     */
    const AlignmentSharedData& getAlignmentSharedData() const;

    /**
     *
     * @return
     */
    const AlignmentExternal& getAlignmentExternal() const;

    /**
     *
     * @return
     */
    bool isRead1First() const;

    /**
     *
     * @param cigar
     * @return
     */
    static uint64_t getLengthOfCigar(const std::string& cigar);

    /**
     *
     * @param alignment
     * @param split
     * @return
     */
    size_t getMappedLength(size_t alignment, size_t split) const;

    /**
     *
     * @return
     */
    std::pair<size_t, size_t> getTemplatePosition() const;

    /**
     *
     * @param alignment
     * @param split
     * @return
     */
    size_t getPosition(size_t alignment, size_t split) const;

    /**
     *
     * @return
     */
    bool getRead1First() const;

    /**
     *
     * @return
     */
    void setMoreAlignmentInfo(std::unique_ptr<AlignmentExternal> _more_alignment_info);
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace record
}  // namespace core
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_CORE_RECORD_RECORD_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
