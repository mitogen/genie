/**
 * @file
 * @copyright This file is part of GABAC. See LICENSE and/or
 * https://github.com/mitogen/gabac for more details.
 */

#ifndef GABAC_STREAMS_H_
#define GABAC_STREAMS_H_

#include <istream>
#include <ostream>

#include "data-block.h"

namespace gabac {

/**
 * @brief FILE* wrapper
 */
class FileBuffer : public std::streambuf {
   public:
    /**
     * @brief Create new buffer
     * @param f FILE* to operate on
     */
    explicit FileBuffer(FILE *f);

    // TODO Jan check these
    virtual ~FileBuffer() = default;
    FileBuffer(const FileBuffer&) = delete;
    FileBuffer& operator=(const FileBuffer&) = delete;
    FileBuffer(FileBuffer&&) = delete;
    FileBuffer& operator=(FileBuffer&&) = delete;

   protected:
    int overflow(int c) override;
    std::streamsize xsputn(const char *s, std::streamsize n) override;
    int sync() override;

    std::streamsize xsgetn(char *s, std::streamsize n) override;
    int underflow() override;

   private:
    FILE *fileptr;
};

/**
 * @brief Buffer for data blocks
 */
class DataBlockBuffer : public std::streambuf {
   public:
    /**
     * @brief Create new buffer
     * @param d Data block to operate on
     * @param pos_i Position to start in the block (mainly for input)
     */
    explicit DataBlockBuffer(DataBlock *d, size_t pos_i = 0);

    // TODO Jan check these
    virtual ~DataBlockBuffer() = default;
    DataBlockBuffer(const DataBlockBuffer&) = delete;
    DataBlockBuffer& operator=(const DataBlockBuffer&) = delete;
    DataBlockBuffer(DataBlockBuffer&&) = delete;
    DataBlockBuffer& operator=(DataBlockBuffer&&) = delete;

    size_t size() const;
   protected:
    int overflow(int c) override;
    std::streamsize xsputn(const char *s, std::streamsize n) override;
    std::streamsize xsgetn(char *s, std::streamsize n) override;
    int underflow() override;
    int uflow() override;
    virtual void flush_block(gabac::DataBlock *blk);

   private:
    DataBlock block;
    size_t pos;
};

/**
 * @brief File input
 */
class IFileStream : public FileBuffer, public std::istream {
   public:
    /**
     * @brief Create new stream
     * @param f FILE* to operate on
     */
    explicit IFileStream(FILE *f);

    // TODO Jan check these
    virtual ~IFileStream() = default;
    IFileStream(const IFileStream&) = delete;
    IFileStream& operator=(const IFileStream&) = delete;
    IFileStream(IFileStream&&) = delete;
    IFileStream& operator=(IFileStream&&) = delete;
};

/**
 * @brief File output
 */
class OFileStream : public FileBuffer, public std::ostream {
   public:
    /**
     * @brief Create new stream
     * @param f FILE* to operate on
     */
    explicit OFileStream(FILE *f);

    // TODO Jan check these
    virtual ~OFileStream() = default;
    OFileStream(const OFileStream&) = delete;
    OFileStream& operator=(const OFileStream&) = delete;
    OFileStream(OFileStream&&) = delete;
    OFileStream& operator=(OFileStream&&) = delete;
};

/**
 * @brief Buffer input
 */
class IBufferStream : public DataBlockBuffer, public std::istream {
   public:
    /**
     * @brief Create new stream.
     * @param d Data block to operate on.
     * @param pos_i Input starting position.
     */
    explicit IBufferStream(DataBlock *d, size_t pos_i = 0);

    // TODO Jan check these
    virtual ~IBufferStream() = default;
    IBufferStream(const IBufferStream&) = delete;
    IBufferStream& operator=(const IBufferStream&) = delete;
    IBufferStream(IBufferStream&&) = delete;
    IBufferStream& operator=(IBufferStream&&) = delete;
};

/**
 * @brief Buffer output
 */
class OBufferStream : public DataBlockBuffer, public std::ostream {
   public:
    /**
     * @brief Create new buffer
     * @param d data block to operate on
     * @todo check if parameter necessary
     */
    explicit OBufferStream(DataBlock *d);

    // TODO Jan check these
    virtual ~OBufferStream() = default;
    OBufferStream(const OBufferStream&) = delete;
    OBufferStream& operator=(const OBufferStream&) = delete;
    OBufferStream(OBufferStream&&) = delete;
    OBufferStream& operator=(OBufferStream&&) = delete;

    /**
     * @brief Swap internal block with external one
     * @param blk Block to fill with data
     */
    virtual void flush(gabac::DataBlock *blk) { flush_block(blk); }
};

/**
 * @brief Buffer doing nothing
 */
class NullBuffer : public std::streambuf {
   protected:
    int overflow(int c) override { return c; }
};

/**
 * @brief Stream doing nothing
 */
class NullStream : public std::ostream {
   public:
    /**
     * @brief Create new nullstream
     */
    NullStream();

    // TODO Jan check these
    virtual ~NullStream() = default;
    NullStream(const NullStream&) = delete;
    NullStream& operator=(const NullStream&) = delete;
    NullStream(NullStream&&) = delete;
    NullStream& operator=(NullStream&&) = delete;

   private:
    NullBuffer m_sb;
};

}  // namespace gabac

#endif  // GABAC_STREAMS_H_
