#ifndef UTIL_FASTA_RECORD_H_
#define UTIL_FASTA_RECORD_H_

#include <string>

namespace util {

struct FastaRecord {
   public:
    FastaRecord(std::string header, std::string sequence);
    ~FastaRecord();

    // TODO Jan check these
    FastaRecord(const FastaRecord&) = delete;
    FastaRecord& operator=(const FastaRecord&) = delete;
    FastaRecord(FastaRecord&&) = default;
    FastaRecord& operator=(FastaRecord&&) = delete;

   public:
    std::string header;
    std::string sequence;
};

}  // namespace util

#endif  // UTIL_FASTA_RECORD_H_
