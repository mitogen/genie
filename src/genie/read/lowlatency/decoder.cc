/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "decoder.h"
#include <genie/core/global-cfg.h>
#include <genie/read/basecoder/decoder.h>
#include <name-decoder.h>
#include <sstream>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace read {
namespace lowlatency {

// ---------------------------------------------------------------------------------------------------------------------

void Decoder::flowIn(core::AccessUnitRaw&& t, const util::Section& id) {
    core::record::Chunk ret;
    core::AccessUnitRaw data = std::move(t);

    std::stringstream str;
    util::BitReader reader(str);
    const auto& qvparam = data.getParameters().getQVConfig(data.getClassType());
    auto qvStream = std::move(data.get(core::GenDesc::QV));
    auto names = namecoder->process(data.get(core::GenDesc::RNAME));
    std::vector<std::string> ecigars;
    for (size_t i = 0; i < data.getNumRecords() / data.getParameters().getNumberTemplateSegments(); ++i) {
        core::record::Record rec(data.getParameters().getNumberTemplateSegments(), core::record::ClassType::CLASS_U,
                                 std::move(names[i]), "", 0);

        if (data.getParameters().getNumberTemplateSegments() > 1) {
            UTILS_DIE_IF(data.pull(core::GenSub::PAIR_DECODING_CASE) != core::GenConst::PAIR_SAME_RECORD,
                         "Only same record pairs supported");
            data.pull(core::GenSub::PAIR_SAME_REC);
        }

        for (size_t j = 0; j < data.getParameters().getNumberTemplateSegments(); ++j) {
            size_t length = data.getParameters().getReadLength();
            ecigars.emplace_back(length, '+');
            if (!length) {
                length = data.pull(core::GenSub::RLEN) + 1;
            }
            std::string seq(length, '\0');
            for (auto& c : seq) {
                c = core::getAlphabetProperties(core::AlphabetID::ACGTN).lut[data.pull(core::GenSub::UREADS)];
            }

            core::record::Segment seg(std::move(seq));
            //     seg.addQualities(qvdecoder->process(data.getParameters().getQVConfig(core::record::ClassType::CLASS_U),
            //                                        std::to_string(length) + "+", data.get(core::GenDesc::QV)));
            rec.addSegment(std::move(seg));
        }

        ret.getData().push_back(std::move(rec));
    }

    auto qvs = this->qvcoder->process(qvparam, ecigars, qvStream);
    size_t qvCounter = 0;
    for (auto& r : ret.getData()) {
        for (auto& s : r.getSegments()) {
            if (!qvs[qvCounter].empty()) {
                s.addQualities(std::move(qvs[qvCounter]));
            }
            qvCounter++;
        }
    }

    flowOut(std::move(ret), id);
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace lowlatency
}  // namespace read
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------