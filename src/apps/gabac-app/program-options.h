/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_APPS_GABAC_APP_PROGRAM_OPTIONS_H_
#define SRC_APPS_GABAC_APP_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------

#include <string>

// ---------------------------------------------------------------------------------------------------------------------

namespace gabacify {

/**
 * @brief
 */
class ProgramOptions {
 public:
    /**
     * @brief
     * @param argc
     * @param argv
     */
    ProgramOptions(int argc, char *argv[]);

    /**
     * @brief
     */
    ~ProgramOptions() = default;

 public:
    std::string logLevel;            //!< @brief
    std::string inputFilePath;       //!< @brief
    std::string dependencyFilePath;  //!< @brief
    std::string outputFilePath;      //!< @brief
    std::string task;                //!< @brief
    size_t blocksize;                //!< @brief

    uint8_t descID;    //!< @brief
    uint8_t subseqID;  //!< @brief

 private:
    /**
     * @brief
     * @param argc
     * @param argv
     */
    void processCommandLine(int argc, char *argv[]);

    /**
     * @brief
     */
    void validate() const;
};

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace gabacify

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_APPS_GABAC_APP_PROGRAM_OPTIONS_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
