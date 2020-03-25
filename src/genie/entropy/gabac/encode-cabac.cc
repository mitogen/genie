/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "encode-cabac.h"

#include <cassert>

#include <genie/util/block-stepper.h>
#include <genie/util/data-block.h>
#include "constants.h"
#include "exceptions.h"
#include "writer.h"

#include "context-selector.h"

namespace genie {
namespace entropy {
namespace gabac {

void encode_cabac(const paramcabac::TransformedSeq &conf,
                  util::DataBlock* const symbols,
                  size_t maxSize) {
    util::DataBlock block(0, 1);

    assert(symbols != nullptr);

    size_t numSymbols = symbols->size();
    if (numSymbols <= 0) return;

    const paramcabac::SupportValues &supportVals = conf.getSupportValues();
    const paramcabac::Binarization &binarzation = conf.getBinarization();
    const paramcabac::BinarizationParameters &binarzationParams = binarzation.getCabacBinarizationParameters();
    const paramcabac::StateVars &stateVars = conf.getStateVars();

    const uint8_t outputSymbolSize = supportVals.getOutputSymbolSize();
    const uint8_t codingSubsymSize = supportVals.getCodingSubsymSize();
    const uint64_t subsymMask = paramcabac::StateVars::get2PowN(codingSubsymSize)-1;
    const bool bypassFlag = binarzation.getBypassFlag();

    OBufferStream bitstream(&block);
    Writer writer(&bitstream,
                  bypassFlag,
                  stateVars.getNumCtxTotal());
    writer.start(numSymbols);

    std::vector<unsigned int> binParams({0});

    util::BlockStepper r = symbols->getReader();
    std::vector<Subsymbol> subsymbols(stateVars.getNumSubsymbols());

    if (bypassFlag) { // bypass mode
        void (Writer::*func)(uint64_t, const std::vector<unsigned int>) = nullptr;
        switch (binarzation.getBinarizationID()) {
            case paramcabac::BinarizationParameters::BinarizationId::BINARY_CODING:
                func = &Writer::writeAsBIbypass;
                binParams[0] = stateVars.getCLengthBI();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY:
                func = &Writer::writeAsTUbypass;
                binParams[0] = binarzationParams.getCMax();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::EXPONENTIAL_GOLOMB:
                func = &Writer::writeAsEGbypass;
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_EXPONENTIAL_GOMB:
                func = &Writer::writeAsSEGbypass;
                break;
            case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_EXPONENTIAL_GOLOMB:
                func = &Writer::writeAsTEGbypass;
                binParams[0] = binarzationParams.getCMaxTeg();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB:
                func = &Writer::writeAsSTEGbypass;
                binParams[0] = binarzationParams.getCMaxTeg();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SPLIT_UNITWISE_TRUNCATED_UNARY:
                func = &Writer::writeAsSUTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_SPLIT_UNITWISE_TRUNCATED_UNARY:
                func = &Writer::writeAsSSUTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::DOUBLE_TRUNCATED_UNARY:
                func = &Writer::writeAsDTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                binParams[2] = binarzationParams.getCMaxDtu();
                break;
            case paramcabac::BinarizationParameters::BinarizationId::SIGNED_DOUBLE_TRUNCATED_UNARY:
                func = &Writer::writeAsSDTUbypass;
                binParams[0] = outputSymbolSize;
                binParams[1] = binarzationParams.getSplitUnitSize();
                binParams[2] = binarzationParams.getCMaxDtu();
                break;
            default:
                GABAC_DIE("Unknown Binarization");
        }
        while (r.isValid()) {
            if (maxSize <= bitstream.size()) {
                break;
            }

            // Split symbol into subsymbols and then encode subsymbols
            uint64_t symbolValue = r.get();
            uint64_t subsymValue = 0;
            uint32_t oss = outputSymbolSize;
            for (uint8_t s=0; s<stateVars.getNumSubsymbols(); s++) {
                subsymValue = (symbolValue>>(oss-=codingSubsymSize)) & subsymMask;
                (writer.*func)(subsymValue, binParams);
            }

            r.inc();
        }

        writer.close();

        bitstream.flush(symbols);

        return;
    }

    void (Writer::*func)(uint64_t, const std::vector<unsigned int>, const unsigned int) = nullptr;
    switch (binarzation.getBinarizationID()) {
        case paramcabac::BinarizationParameters::BinarizationId::BINARY_CODING:
            func = &Writer::writeAsBIcabac;
            binParams[0] = stateVars.getCLengthBI();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_UNARY:
            func = &Writer::writeAsTUcabac;
            binParams[0] = binarzationParams.getCMax();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::EXPONENTIAL_GOLOMB:
            func = &Writer::writeAsEGcabac;
            break;
        case paramcabac::BinarizationParameters::BinarizationId::SIGNED_EXPONENTIAL_GOMB:
            func = &Writer::writeAsSEGcabac;
            break;
        case paramcabac::BinarizationParameters::BinarizationId::TRUNCATED_EXPONENTIAL_GOLOMB:
            func = &Writer::writeAsTEGcabac;
            binParams[0] = binarzationParams.getCMaxTeg();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::SIGNED_TRUNCATED_EXPONENTIAL_GOLOMB:
            func = &Writer::writeAsSTEGcabac;
            binParams[0] = binarzationParams.getCMaxTeg();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::SPLIT_UNITWISE_TRUNCATED_UNARY:
            func = &Writer::writeAsSUTUcabac;
            binParams[0] = outputSymbolSize;
            binParams[1] = binarzationParams.getSplitUnitSize();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::SIGNED_SPLIT_UNITWISE_TRUNCATED_UNARY:
            func = &Writer::writeAsSSUTUcabac;
            binParams[0] = outputSymbolSize;
            binParams[1] = binarzationParams.getSplitUnitSize();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::DOUBLE_TRUNCATED_UNARY:
            func = &Writer::writeAsDTUcabac;
            binParams[0] = outputSymbolSize;
            binParams[1] = binarzationParams.getSplitUnitSize();
            binParams[2] = binarzationParams.getCMaxDtu();
            break;
        case paramcabac::BinarizationParameters::BinarizationId::SIGNED_DOUBLE_TRUNCATED_UNARY:
            func = &Writer::writeAsSDTUcabac;
            binParams[0] = outputSymbolSize;
            binParams[1] = binarzationParams.getSplitUnitSize();
            binParams[2] = binarzationParams.getCMaxDtu();
            break;
        default:
            GABAC_DIE("Unknown Binarization");
    }

    uint8_t codingOrder = supportVals.getCodingOrder();
    if (codingOrder == 0) {
        while (r.isValid()) {
            if (maxSize <= bitstream.size()) {
                break;
            }

            // Split symbol into subsymbols and loop over subsymbols
            uint64_t symbolValue = r.get();
            uint64_t subsymValue = 0;
            uint32_t oss = outputSymbolSize;
            for (uint8_t s=0; s<stateVars.getNumSubsymbols(); s++) {
                subsymValue = (symbolValue>>(oss-=codingSubsymSize)) & subsymMask;
                subsymbols[s].subsymIdx = s;
                uint32_t ctxIdx = ContextSelector::getContextIdx(stateVars,
                                                                 bypassFlag,
                                                                 codingOrder,
                                                                 subsymbols[s]);
                (writer.*func)(subsymValue, binParams, ctxIdx);
            }
            
            r.inc();
        }
    } else if (codingOrder == 1) {
        while (r.isValid()) {
            if (maxSize <= bitstream.size()) {
                break;
            }

            // Split symbol into subsymbols and loop over subsymbols
            uint64_t symbolValue = r.get();
            uint64_t subsymValue = 0;
            uint32_t oss = outputSymbolSize;
            for (uint8_t s=0; s<stateVars.getNumSubsymbols(); s++) {
                subsymValue = (symbolValue>>(oss-=codingSubsymSize)) & subsymMask;
                subsymbols[s].subsymIdx = s;
                uint32_t ctxIdx = ContextSelector::getContextIdx(stateVars,
                                                                 bypassFlag,
                                                                 codingOrder,
                                                                 subsymbols[s]);
                (writer.*func)(subsymValue, binParams, ctxIdx);
                subsymbols[s].prvValues[0] = subsymValue;
            }

            r.inc();
        }
    } else if (codingOrder == 2) {
        while (r.isValid()) {
            if (maxSize <= bitstream.size()) {
                break;
            }

            // Split symbol into subsymbols and loop over subsymbols
            uint64_t symbolValue = r.get();
            uint64_t subsymValue = 0;
            uint32_t oss = outputSymbolSize;
            for (uint8_t s=0; s<stateVars.getNumSubsymbols(); s++) {
                subsymValue = (symbolValue>>(oss-=codingSubsymSize)) & subsymMask;
                subsymbols[s].subsymIdx = s;
                uint32_t ctxIdx = ContextSelector::getContextIdx(stateVars,
                                                                 bypassFlag,
                                                                 codingOrder,
                                                                 subsymbols[s]);
                (writer.*func)(subsymValue, binParams, ctxIdx);
                subsymbols[s].prvValues[1] = subsymbols[s].prvValues[0];
                subsymbols[s].prvValues[0] = subsymValue;
            }

            r.inc();
        }
    } else {
        GABAC_DIE("Invalid context selection");
    }

    writer.close();

    bitstream.flush(symbols);
}
}  // namespace gabac
}  // namespace entropy
}  // namespace genie
