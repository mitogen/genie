// Copyright 2018 The genie authors

#ifndef GENIE_GENIE_GABAC_OUTPUT_STREAM_H
#define GENIE_GENIE_GABAC_OUTPUT_STREAM_H

#include <gabac/gabac.h>

namespace genie {

class GenieGabacOutputBuffer : public std::streambuf {
   public:
    GenieGabacOutputBuffer();

    // TODO Jan to check these
    ~GenieGabacOutputBuffer();
    GenieGabacOutputBuffer(const GenieGabacOutputBuffer&) = delete;
    GenieGabacOutputBuffer& operator=(const GenieGabacOutputBuffer&) = delete;
    GenieGabacOutputBuffer(GenieGabacOutputBuffer&&) = delete;
    GenieGabacOutputBuffer& operator=(GenieGabacOutputBuffer&&) = delete;

   protected:
    int overflow(int c) override;
    std::streamsize xsputn(const char *s, std::streamsize n) override;

   private:
    std::vector<gabac::DataBlock> streams;
    std::streamsize bytesLeft;
    std::vector<uint8_t> sizeBuf;

   public:
    void flush_blocks(std::vector<gabac::DataBlock> *dat);
};

class GenieGabacOutputStream : public GenieGabacOutputBuffer, public std::ostream {
   public:
    GenieGabacOutputStream() : GenieGabacOutputBuffer(), std::ostream(this) {}

    // TODO Jan to check these
    ~GenieGabacOutputStream();
    GenieGabacOutputStream(const GenieGabacOutputStream&) = delete;
    GenieGabacOutputStream& operator=(const GenieGabacOutputStream&) = delete;
    GenieGabacOutputStream(GenieGabacOutputStream&&) = delete;
    GenieGabacOutputStream& operator=(GenieGabacOutputStream&&) = delete;
};

}  // namespace genie

#endif  // GENIE_GENIE_GABAC_OUTPUT_STREAM_H
