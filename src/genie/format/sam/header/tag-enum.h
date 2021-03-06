/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_FORMAT_SAM_HEADER_TAG_ENUM_H_
#define SRC_GENIE_FORMAT_SAM_HEADER_TAG_ENUM_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "genie/format/sam/header/tag.h"
#include "genie/util/make-unique.h"
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace sam {
namespace header {

// ---------------------------------------------------------------------------------------------------------------------

template <typename Type>
class TagEnum : public TagBase {
 private:
    Type data;
    const std::vector<std::string>& lookup;

 public:
    TagEnum(std::string _name, Type _data, const std::vector<std::string>& _lookup)
        : TagBase(std::move(_name)), data(_data), lookup(_lookup) {}

    std::string toString() const override { return lookup[size_t(data)]; }

    Type getData() const { return data; }

    static std::unique_ptr<TagBase> fromString(std::string name, const std::string& string,
                                               const std::vector<std::string>& lookup) {
        for (size_t s = 0; s < lookup.size(); ++s) {
            if (string == lookup[s]) {
                return util::make_unique<TagEnum<Type>>(std::move(name), Type(s), lookup);
            }
        }
        UTILS_DIE("Unknown Tag value");
    }
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace header
}  // namespace sam
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_FORMAT_SAM_HEADER_TAG_ENUM_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
