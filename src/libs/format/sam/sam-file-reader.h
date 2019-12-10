#ifndef UTIL_SAM_FILE_READER_H_
#define UTIL_SAM_FILE_READER_H_

#include <list>
#include <string>
#include "sam-record.h"
#include "util/file-reader.h"

namespace format {
namespace sam {

class SamFileReader : public util::FileReader {
   public:
    explicit SamFileReader(const std::string &path);

    ~SamFileReader() override;

    size_t readRecords(size_t numRecords, std::list<SamRecord> *records);

   public:
    std::string header;

   private:
    void readHeader();
};

}  // namespace sam
}  // namespace format

#endif  // UTIL_SAM_FILE_READER_H_
