#ifndef GENIE_LOCAL_ASSEMBLY_DECODER_H
#define GENIE_LOCAL_ASSEMBLY_DECODER_H

#include "mpegg-decoder.h"
#include "read-decoder.h"
#include "reference-encoder.h"
class LocalAssemblyDecoder : public MpeggDecoder {
   private:
   public:
    void flowIn(MpeggRawAu&& t, size_t id) override {
        size_t numRecords = t.getNumRecords();
        size_t bufSize = t.getParameters().getParameterSetCrps().getCrpsInfo().getBufMaxSize();
        size_t segments = t.getParameters().getNumberTemplateSegments();
        lae::LocalAssemblyReferenceEncoder refEncoder(bufSize);
        lae::LocalAssemblyReadDecoder decoder(std::move(t), segments);
        format::mpegg_rec::MpeggChunk chunk;
        for (size_t recID = 0; recID < numRecords; ++recID) {
            auto meta = decoder.readSegmentMeta();
            std::vector<std::string> refs;
            refs.reserve(meta.size());
            for (const auto& m : meta) {
                refs.emplace_back(refEncoder.getReference(m.position, m.length));
            }
            auto rec = decoder.pull(std::move(refs));
            refEncoder.addRead(rec);
            chunk.emplace_back(std::move(rec));
        }
        flowOut(std::move(chunk), id);
    }
    void dryIn() override { dryOut(); }
};

#endif  // GENIE_LOCAL_ASSEMBLY_DECODER_H
