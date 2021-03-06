/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/format/sam/header/header.h"
#include <iostream>
#include <regex>
#include <utility>
#include "genie/format/sam/header/tag-enum.h"
#include "genie/format/sam/header/tag-number.h"
#include "genie/format/sam/header/tag-string.h"
#include "genie/format/sam/header/tag-version.h"
#include "genie/util/string-helpers.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace format {
namespace sam {
namespace header {

// ---------------------------------------------------------------------------------------------------------------------

const std::string& HeaderLine::getName() const { return name; }

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<std::unique_ptr<TagBase>>& HeaderLine::getTags() const { return tags; }

// ---------------------------------------------------------------------------------------------------------------------

void HeaderLine::addTag(std::unique_ptr<TagBase> tag) { tags.push_back(std::move(tag)); }

// ---------------------------------------------------------------------------------------------------------------------

std::vector<std::unique_ptr<TagBase>>&& HeaderLine::moveTags() { return std::move(tags); }

// ---------------------------------------------------------------------------------------------------------------------

void HeaderLine::parseSingleTag(const std::string& value, const TagInfo& info) {
    if (!info.regex.empty()) {
        const std::regex regex(info.regex);
        if (!std::regex_match(value, regex)) {
            std::cerr << "Sam header regex fail: skipping tag " << info.name << std::endl;
            return;
        }
    }
    auto tag = info.create(info.name, value);
    tags.emplace_back(std::move(tag));
}

// ---------------------------------------------------------------------------------------------------------------------

void HeaderLine::parseTags(const std::vector<std::string>& _tags, const HeaderLineInfo& info) {
    bool first = true;
    for (const auto& t : _tags) {
        if (first) {
            first = false;
            continue;
        }
        auto separator = t.find(':');
        UTILS_DIE_IF(separator == std::string::npos, "Tag separator not found in sam header");
        std::string tagname = t.substr(0, separator);
        std::string tagvalue = t.substr(separator + 1);
        bool found = false;
        for (const auto& taginfo : info.tags) {
            if (taginfo.name == tagname) {
                found = true;
                parseSingleTag(tagvalue, taginfo);
            }
        }
        UTILS_DIE_IF(!found, "Unknown tag in SAM header");
    }
}

// ---------------------------------------------------------------------------------------------------------------------

HeaderLine::HeaderLine(const std::string& line) {
    auto ret = util::tokenize(line, '\t');
    name = ret.front().substr(1);

    bool found = false;
    for (const auto& header : getHeaderInfo()) {
        if (header.name == name) {
            found = true;
            parseTags(ret, header);
            break;
        }
    }
    UTILS_DIE_IF(!found, "Unknown SAM header line");
}

// ---------------------------------------------------------------------------------------------------------------------

HeaderLine::HeaderLine(HeaderLine&& line) noexcept
    //    : name(std::move(line.name)), comment(std::move(line.comment)), tags(std::move(line.tags)) {}
    : name(std::move(line.name)), tags(std::move(line.tags)) {}

// ---------------------------------------------------------------------------------------------------------------------

HeaderLine::HeaderLine(std::string&& _name, std::string&&)
    //    : name(std::move(_name)), comment(std::move(_comment)) {}
    : name(std::move(_name)) {}

// ---------------------------------------------------------------------------------------------------------------------

void HeaderLine::print(std::ostream& stream) const {
    stream << "@" << name;
    for (const auto& t : tags) {
        stream << "\t" << t->getName() << ":" << t->toString();
    }
    stream << "\n";
}

// ---------------------------------------------------------------------------------------------------------------------

Header::Header(std::istream& stream) {
    std::string str;
    while (stream.peek() == '@') {
        std::getline(stream, str);

        // Handle comment
        if (str.substr(1, 2) == "CO") {
            addComment(str.substr(4));
        } else {
            lines.emplace_back(str);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

Header::Header(Header&& header) noexcept : lines(std::move(header.lines)) {}

// ---------------------------------------------------------------------------------------------------------------------

void Header::print(std::ostream& stream) const {
    for (const auto& l : lines) {
        l.print(stream);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

const std::vector<HeaderLine>& Header::getLines() const { return lines; }

// ---------------------------------------------------------------------------------------------------------------------

void Header::addComment(std::string&& _str) { comments.push_back(_str.substr(4)); }

// ---------------------------------------------------------------------------------------------------------------------

void Header::addLine(HeaderLine&& line) { lines.emplace_back(std::move(line)); }

// ---------------------------------------------------------------------------------------------------------------------

void Header::globalChecks() const {
    // TODO(Fabian): Implement
}

// ---------------------------------------------------------------------------------------------------------------------

Header Header::createDefaultHeader() {
    Header header;
    HeaderLine hd("HD", "");
    hd.addTag(util::make_unique<TagVersion>("VN", std::array<uint8_t, 2>{1, 6}));
    hd.addTag(util::make_unique<TagEnum<SortingOrder>>("SO", SortingOrder::COORDINATE, getSortingOrderString()));
    header.addLine(std::move(hd));
    HeaderLine sq("SQ", "");
    sq.addTag(util::make_unique<TagString>("SN", "GenieTestRef"));
    sq.addTag(util::make_unique<TagNumber>("LN", 123456));
    sq.addTag(util::make_unique<TagString>("SP", "Anas-platyrhynchos"));
    header.addLine(std::move(sq));
    HeaderLine pg("PG", "");
    pg.addTag(util::make_unique<TagString>("ID", "Genie"));
    header.addLine(std::move(pg));
    HeaderLine rg("RG", "");
    rg.addTag(util::make_unique<TagString>("ID", "GenieGroup"));
    header.addLine(std::move(rg));
    return header;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace header
}  // namespace sam
}  // namespace format
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
