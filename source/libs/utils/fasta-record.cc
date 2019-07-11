#include <utility>

#include <utility>

#include "fasta-record.h"

#include <iostream>

namespace utils {

    FastaRecord::FastaRecord(std::string header, std::string sequence)
            : header(std::move(header)),
              sequence(std::move(sequence)) {}

    FastaRecord::~FastaRecord() = default;

}  // namespace utils