#ifndef GENIE_PROGRAM_OPTIONS_H_
#define GENIE_PROGRAM_OPTIONS_H_

#include <string>
#include <vector>

namespace genie {

class ProgramOptions {
   public:
    ProgramOptions(int argc, char *argv[]);
    ~ProgramOptions();

    // TODO Jan to check these
    ProgramOptions(const ProgramOptions&) = delete;
    ProgramOptions& operator=(const ProgramOptions&) = delete;
    ProgramOptions(ProgramOptions&&) = delete;
    ProgramOptions& operator=(ProgramOptions&&) = delete;

   public:
    std::string inputFilePath;
    std::string outputFilePath;

   private:
    void processCommandLine(int argc, char *argv[]);
};

}  // namespace genie

#endif  // GENIE_PROGRAM_OPTIONS_H_
