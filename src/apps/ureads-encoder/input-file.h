#ifndef GABACIFY_INPUT_FILE_H_
#define GABACIFY_INPUT_FILE_H_

#include <string>

#include "file.h"

namespace genie {

class InputFile : public File {
   public:
    explicit InputFile(const std::string& path);

    ~InputFile() override;

    // TODO Jan to check these
    InputFile(const InputFile&) = delete;
    InputFile& operator=(const InputFile&) = delete;
    InputFile(InputFile&&) = delete;
    InputFile& operator=(InputFile&&) = delete;

    void read(void* items, size_t itemSize, size_t numItems);
};

}  // namespace genie

#endif  // GABACIFY_INPUT_FILE_H_
