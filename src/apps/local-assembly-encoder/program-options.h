#ifndef LAE_PROGRAM_OPTIONS_H_
#define LAE_PROGRAM_OPTIONS_H_

#include <format/mpegg_rec/mpegg-record.h>
#include <string>

namespace lae {

class ProgramOptions {
   public:
    ProgramOptions(int argc, char *argv[]);

    ~ProgramOptions();

   public:
    std::string inputFilePath;
    std::string outputFilePath;
    std::string typeString;

    format::mpegg_rec::MpeggRecord::ClassType type;

   private:
    void processCommandLine(int argc, char *argv[]);
};

}  // namespace lae

#endif  // LAE_PROGRAM_OPTIONS_H_
