/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef GENIE_TAG_STRING_H
#define GENIE_TAG_STRING_H

// ---------------------------------------------------------------------------------------------------------------------

#include <memory>
#include "tag.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace sam {
namespace header {

/**
 *
 */
class TagString : public TagBase {
   private:
    std::string data;  //!<

   public:
    /**
     *
     * @param _name
     * @param _data
     */
    TagString(std::string _name, std::string _data);

    /**
     *
     * @return
     */
    std::string toString() const override;

    /**
     *
     * @param name
     * @param string
     * @return
     */
    static std::unique_ptr<TagBase> fromString(std::string name, const std::string& string);
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace header
}  // namespace sam
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // GENIE_TAG_STRING_H

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------