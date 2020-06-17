/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "default-setup.h"
#include <genie/read/localassembly/decoder.h>
#include <genie/read/localassembly/encoder.h>
#include <genie/read/refcoder/decoder.h>
#include <genie/read/refcoder/encoder.h>
//#include <genie/read/spring/encoder.h>
#include <genie/core/classifier-bypass.h>
#include <genie/core/classifier-regroup.h>
#include <genie/core/flowgraph-convert.h>
#include <genie/entropy/gabac/decoder.h>
#include <genie/entropy/gabac/encoder.h>
#include <genie/name/tokenizer/decoder.h>
#include <genie/name/tokenizer/encoder.h>
#include <genie/quality/qvwriteout/decoder.h>
#include <genie/quality/qvwriteout/encoder.h>
#include <genie/read/lowlatency/decoder.h>
#include <genie/read/lowlatency/encoder.h>
#include <genie/read/spring/decoder.h>
#include <genie/read/spring/encoder.h>

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace module {

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::FlowGraphEncode> buildDefaultEncoder(size_t threads, const std::string& working_dir, size_t blocksize) {
    std::unique_ptr<core::FlowGraphEncode> ret = genie::util::make_unique<core::FlowGraphEncode>(threads);

    ret->setClassifier(genie::util::make_unique<genie::core::ClassifierRegroup>(blocksize));

    ret->addReadCoder(genie::util::make_unique<genie::read::refcoder::Encoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::localassembly::Encoder>(2048, false));
    ret->addReadCoder(genie::util::make_unique<genie::read::spring::Encoder>(working_dir, threads, false));
    ret->addReadCoder(genie::util::make_unique<genie::read::spring::Encoder>(working_dir, threads, true));
    ret->setReadCoderSelector([](const genie::core::record::Chunk& chunk) -> size_t {
        if (chunk.getData().front().getClassID() == genie::core::record::ClassType::CLASS_U) {
            if(chunk.getData().front().getNumberOfTemplateSegments() > 1) {
                return 3;
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    });

    ret->addQVCoder(genie::util::make_unique<genie::quality::qvwriteout::Encoder>());
    ret->setQVSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    ret->addNameCoder(genie::util::make_unique<genie::name::tokenizer::Encoder>());
    ret->setNameSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    ret->addEntropyCoder(genie::util::make_unique<genie::entropy::gabac::Encoder>());
    ret->setEntropyCoderSelector([](const genie::core::AccessUnit::Descriptor&) -> size_t { return 0; });

    ret->setExporterSelector([](const genie::core::AccessUnit&) -> size_t { return 0; });

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::FlowGraphDecode> buildDefaultDecoder(size_t threads, const std::string& working_dir, size_t) {
    std::unique_ptr<core::FlowGraphDecode> ret = genie::util::make_unique<core::FlowGraphDecode>(threads);

    ret->addReadCoder(genie::util::make_unique<genie::read::refcoder::Decoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::localassembly::Decoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::lowlatency::Decoder>());
    ret->addReadCoder(genie::util::make_unique<genie::read::spring::Decoder>(working_dir, true, false, threads));
    ret->addReadCoder(genie::util::make_unique<genie::read::spring::Decoder>(working_dir, true, true, threads));
    ret->setReadCoderSelector([](const genie::core::AccessUnit& au) -> size_t {
        if (au.getParameters().isComputedReference()) {
            switch (au.getParameters().getComputedRef().getAlgorithm()) {
                case core::parameter::ComputedRef::Algorithm::GLOBAL_ASSEMBLY:
                    if(au.getParameters().getNumberTemplateSegments() >= 2) {
                        return 4;
                    } else {
                        return 3;
                    }
                    break;
                case core::parameter::ComputedRef::Algorithm::REF_TRANSFORM:
                    UTILS_DIE("Ref Transform decoding not supported");
                    break;
                case core::parameter::ComputedRef::Algorithm::LOCAL_ASSEMBLY:
                    return 1;
                case core::parameter::ComputedRef::Algorithm::PUSH_IN:
                    UTILS_DIE("Push in decoding not supported");
                    break;
                default:
                    UTILS_DIE("Unknown decoding not supported");
                    break;
            }
        } else {
            if (au.getClassType() == core::record::ClassType::CLASS_U) {
                return 2;
            } else {
                return 0;
            }
        }
    });

    ret->addQVCoder(genie::util::make_unique<genie::quality::qvwriteout::Decoder>());
    ret->setQVSelector([](const genie::core::parameter::QualityValues& param, const std::vector<std::string>&,
                          genie::core::AccessUnit::Descriptor&) -> size_t {
        UTILS_DIE_IF(param.getMode() != 1, "Unsupported QV decoding mode");
        return 0;
    });

    ret->addNameCoder(genie::util::make_unique<genie::name::tokenizer::Decoder>());
    ret->setNameSelector([](const genie::core::AccessUnit::Descriptor&) -> size_t { return 0; });

    ret->addEntropyCoder(genie::util::make_unique<genie::entropy::gabac::Decoder>());
    ret->setEntropyCoderSelector([](const genie::core::parameter::DescriptorSubseqCfg&, genie::core::AccessUnit::Descriptor&) -> size_t { return 0; });

    ret->setExporterSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<core::FlowGraphConvert> buildDefaultConverter(size_t threads) {
    std::unique_ptr<core::FlowGraphConvert> ret = genie::util::make_unique<core::FlowGraphConvert>(threads);

    ret->setExporterSelector([](const genie::core::record::Chunk&) -> size_t { return 0; });

    ret->setClassifier(genie::util::make_unique<genie::core::ClassifierBypass>());

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace module
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------