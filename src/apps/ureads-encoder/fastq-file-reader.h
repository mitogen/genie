#ifndef GENIE_FASTQ_FILE_READER_H_
#define GENIE_FASTQ_FILE_READER_H_

#include <string>
#include <vector>

#include "fastq-record.h"
#include "file-reader.h"

namespace genie {

class FastqFileReader : public FileReader {
   public:
    explicit FastqFileReader(const std::string& path);
    ~FastqFileReader() override;

    // TODO Jan to check these
    FastqFileReader(const FastqFileReader&) = delete;
    FastqFileReader& operator=(const FastqFileReader&) = delete;
    FastqFileReader(FastqFileReader&&) = delete;
    FastqFileReader& operator=(FastqFileReader&&) = delete;


    size_t readRecords(size_t numRecords, std::vector<FastqRecord>* records);
};

}  // namespace genie

#endif  // GENIE_FASTQ_FILE_READER_H_
