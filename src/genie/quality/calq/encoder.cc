/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "encoder.h"
#include <core/record/alignment_split/same-rec.h>
#include <util/watch.h>
#include <vector>
#include "calq_coder.h"
#include "codebook.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace quality {
namespace calq {

// ---------------------------------------------------------------------------------------------------------------------

void Encoder::setUpParameters(const core::record::Chunk& rec, paramqv1::QualityValues1& param,
                              core::AccessUnit::Descriptor& desc, paramqv1::ParameterSet& set) {
    //paramqv1::ParameterSet set;

    //auto codebook = paramqv1::QualityValues1::getPresetCodebook(paramqv1::QualityValues1::QvpsPresetId::ASCII);
    //set.addCodeBook(std::move(codebook));

    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_PRESENT));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_CODEBOOK));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_0));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_1));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_2));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_3));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_4));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_5));
    desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_6));
    if (rec.getData().front().getClassID() == core::record::ClassType::CLASS_I ||
        rec.getData().front().getClassID() == core::record::ClassType::CLASS_HM) {
        desc.add(core::AccessUnit::Subsequence(4, core::GenSub::QV_STEPS_1));

        //codebook = paramqv1::QualityValues1::getPresetCodebook(paramqv1::QualityValues1::QvpsPresetId::ASCII);
        //set.addCodeBook(std::move(codebook));
    }

    param.setQvps(std::move(set));
}

// ---------------------------------------------------------------------------------------------------------------------

void Encoder::encodeAlignedSegment(const core::record::Segment& s, const std::string& ecigar,
                                   core::AccessUnit::Descriptor& desc) {
    for (const auto& q : s.getQualities()) {
        core::CigarTokenizer::tokenize(
            ecigar, core::getECigarInfo(),
            [&desc, &q](uint8_t cigar, const util::StringView& bs, const util::StringView&) -> bool {
                auto qvs = bs.deploy(q.data());
                uint8_t codebook = core::getECigarInfo().lut_step_ref[cigar] ||
                                           core::getAlphabetProperties(core::AlphabetID::ACGTN).isIncluded(cigar)
                                       ? 2
                                       : desc.getSize() - 1;
                for (const auto& c : qvs) {
                    UTILS_DIE_IF(c < 33 || c > 126, "Invalid quality score");
                    desc.get(codebook).push(c - 33);
                }
                return true;
            });
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Encoder::encodeUnalignedSegment(const core::record::Segment& s, core::AccessUnit::Descriptor& desc) {
    for (const auto& q : s.getQualities()) {
        for (const auto& c : q) {
            UTILS_DIE_IF(c < 33 || c > 126, "Invalid quality score");
            desc.get(desc.getSize() - 1).push(c - 33);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

core::QVEncoder::QVCoded Encoder::process(const core::record::Chunk& rec) {
    util::Watch watch;
    EncodingOptions opt;
    SideInformation sideInformation;
    EncodingBlock input;
    DecodingBlock output;

    sideInformation.qualOffset = 33;
    if (sideInformation.positions.empty())
    {
        sideInformation.posOffset = 0;
    }
    else
    {
        sideInformation.posOffset = sideInformation.positions[0];
    }
    
    auto param = util::make_unique<paramqv1::QualityValues1>(paramqv1::QualityValues1::QvpsPresetId::OFFSET33_RANGE41, false);
    core::AccessUnit::Descriptor desc(core::GenDesc::QV);

    for (const auto& r : rec.getData()) {
        size_t iter = 0;
        size_t num_aligned_segs = r.getAlignments().empty() ? 0 : 1;
        if (num_aligned_segs) {
            num_aligned_segs += r.getAlignments().front().getAlignmentSplits().size();
        }

        size_t ctr = 0;
        for (const auto& s : r.getSegments()) {
            bool aligned = ctr < num_aligned_segs;
            if (aligned) {
                sideInformation.cigars.push_back(ctr ? reinterpret_cast<const core::record::alignment_split::SameRec&>(
                                                     *r.getAlignments().front().getAlignmentSplits().at(ctr - 1))
                                                     .getAlignment()
                                                     .getECigar()
                                               : r.getAlignments().front().getAlignment().getECigar());
                sideInformation.sequences.push_back(s.getSequence());
                sideInformation.positions.push_back(r.getPosition(0,0)+1); // 1-based position
                for (const auto& t: s.getQualities()){
                    input.qvalues.push_back(t);
                }
                
                //encodeAlignedSegment(s, sideInformation.cigars[iter], desc);
            } else {
                //encodeUnalignedSegment(s, desc);
            }
            ctr++;
        }
        iter++;
    }
    encode(opt, sideInformation, input, &output);

   // setting param codeBooks
   paramqv1::ParameterSet set;
   size_t iter = 0;
   for (const auto& book : output.codeBooks){
       genie::quality::paramqv1::Codebook codebook;
       for (auto c = book.begin(); c != book.end(); c++){
          codebook.addEntry(std::move(*c));
       }
       set.addCodeBook(std::move(codebook));
       iter++;
   }
   setUpParameters(rec, *param, desc, set);

   //setting desc stepindeces
    iter = 2;
    for (const auto& sindex : output.stepindices){
        for (const auto& c : sindex){
            desc.get(iter).push(c);
        }
        iter++;
    }
    //setting desc quantizerIndices
    for (const auto& qindex : output.quantizerIndices){
        desc.get(1).push(qindex);
    }

    core::stats::PerfStats stats;
    stats.addDouble("time-calq", watch.check());
    return {std::move(param), std::move(desc), stats};
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace calq
}  // namespace quality
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------