#ifndef UTIL_SAM_FILE_READER_H_
#define UTIL_SAM_FILE_READER_H_

#include <list>
#include <string>
#include "file-reader.h"
#include "sam-record.h"

namespace util {

class SamFileReader : public FileReader {
   public:
    explicit SamFileReader(const std::string &path);
    ~SamFileReader() override;

    // TODO Jan check these
    SamFileReader(const SamFileReader&) = delete;
    SamFileReader& operator=(const SamFileReader&) = delete;
    SamFileReader(SamFileReader&&) = default;
    SamFileReader& operator=(SamFileReader&&) = delete;

    size_t readRecords(size_t numRecords, std::list<SamRecord> *records);

   public:
    std::string header;

   private:
    void readHeader();
};

}  // namespace util

#endif  // UTIL_SAM_FILE_READER_H_
