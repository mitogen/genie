/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "apps/genie/run/program-options.h"
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include "cli11/CLI11.hpp"
#include "filesystem/filesystem.hpp"
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genieapp {
namespace run {

// ---------------------------------------------------------------------------------------------------------------------

std::string parent_dir(const std::string &path) {
    std::string ret;

    auto pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        ret = ".";
    } else {
        ret = path.substr(0, pos);
    }

    while (ret.back() == '/') {
        ret.pop_back();
    }
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

ProgramOptions::ProgramOptions(int argc, char *argv[]) : help(false) {
    CLI::App app("Genie MPEG-G reference encoder\n");

    app.add_option("-i,--input-file", inputFile, "")->mandatory(true);
    app.add_option("-o,--output-file", outputFile, "")->mandatory(true);

    inputSupFile = "";
    app.add_option("--input-suppl-file", inputSupFile, "");
    inputRefFile = "";
    app.add_option("--input-ref-file", inputRefFile, "");

    outputSupFile = "";
    app.add_option("--output-suppl-file", outputSupFile, "");

    workingDirectory = "";
    app.add_option("-w,--working-dir", workingDirectory, "");

    qvMode = "lossless";
    app.add_option("--qv", qvMode, "");

    readNameMode = "lossless";
    app.add_option("--read-ids", readNameMode, "");

    forceOverwrite = false;
    app.add_flag("-f,--force", forceOverwrite, "");

    combinePairsFlag = false;
    app.add_flag("--combine-pairs", combinePairsFlag, "");

    lowLatency = false;
    app.add_flag("--low-latency", lowLatency, "");

    refMode = "relevant";
    app.add_option("--embedded-ref", refMode, "");

    numberOfThreads = std::thread::hardware_concurrency();
    app.add_option("-t,--threads", numberOfThreads, "");

    rawReference = false;
    app.add_flag("--raw-ref", rawReference, "");

    try {
        app.parse(argc, argv);

        if (workingDirectory.empty()) {
            workingDirectory = parent_dir(outputFile);
        }

        while (workingDirectory.back() == '/') {
            workingDirectory.pop_back();
        }
    } catch (const CLI::CallForHelp &) {
        std::cout << app.help() << std::endl;
        help = true;
        return;
    } catch (const CLI::ParseError &e) {
        UTILS_DIE("Command line parsing failed:" + std::to_string(app.exit(e)));
    }

    validate();
}

// ---------------------------------------------------------------------------------------------------------------------

std::string size_string(std::uintmax_t f_size) {
    size_t exponent = 0;
    auto size = static_cast<double>(f_size);
    while (size / 1024.0 > 1.0) {
        size = size / 1024.0;
        ++exponent;
    }
    const std::vector<std::string> UNITS = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    UTILS_DIE_IF(exponent >= UNITS.size(), "Filesize >= 1 exbibyte not supported");
    std::string number = std::to_string(size);
    number = number.substr(0, 4);
    if (number.back() == '.') {
        number = number.substr(0, 3);
    }
    return number + UNITS[exponent];
}

// ---------------------------------------------------------------------------------------------------------------------

void validateInputFile(const std::string &file) {
    UTILS_DIE_IF(!ghc::filesystem::exists(file), "Input file does not exist: " + file);
    std::ifstream stream(file);
    UTILS_DIE_IF(!stream, "Input file does exist, but is not accessible. Insufficient permissions? " + file);
}

// ---------------------------------------------------------------------------------------------------------------------

std::string random_string(size_t length) {
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

// ---------------------------------------------------------------------------------------------------------------------

void validateWorkingDir(const std::string &dir) {
    UTILS_DIE_IF(!ghc::filesystem::exists(dir), "Directory does not exist: " + dir);
    UTILS_DIE_IF(!ghc::filesystem::is_directory(dir), "Is a file and not a directory: " + dir);

    std::string test_name;
    do {
        const size_t NAME_LENGTH = 16;
        test_name = dir + "/" + random_string(NAME_LENGTH) + ".test";
    } while (ghc::filesystem::exists(test_name));

    {
        const std::string TEST_STRING = "test";
        std::ofstream test_file(test_name);
        test_file << TEST_STRING << std::endl;
        UTILS_DIE_IF(!test_file, "Can't write to working directory. Insufficient permissions? " + dir);
    }

    ghc::filesystem::remove(test_name);
}

// ---------------------------------------------------------------------------------------------------------------------

void validateOutputFile(const std::string &file, bool forced) {
    UTILS_DIE_IF(ghc::filesystem::exists(file) && !forced,
                 "Output file already existing and no force flag set: " + file);
    UTILS_DIE_IF(ghc::filesystem::exists(file) && !ghc::filesystem::is_regular_file(file),
                 "Output file already existing, force flag set, but not a regular file. Genie won't overwrite folders "
                 "or special files: " +
                     file);
    {
        std::ofstream stream(file);
        const std::string TEST_STRING = "test";
        stream << TEST_STRING << std::endl;
        UTILS_DIE_IF(!stream, "Output file not accessible. Insufficient permissions? " + file);
    }
    ghc::filesystem::remove(file);
}

// ---------------------------------------------------------------------------------------------------------------------

void ProgramOptions::validate() {
    validateInputFile(inputFile);
    std::cout << "Input file: " << inputFile << " with size " << size_string(ghc::filesystem::file_size(inputFile))
              << std::endl;

    if (!inputSupFile.empty()) {
        validateInputFile(inputSupFile);
        std::cout << "Input supplementary file: " << inputSupFile << " with size "
                  << size_string(ghc::filesystem::file_size(inputSupFile)) << std::endl;
    }
    if (!inputRefFile.empty()) {
        validateInputFile(inputRefFile);
        std::cout << "Input reference file: " << inputRefFile << " with size "
                  << size_string(ghc::filesystem::file_size(inputRefFile)) << std::endl;
    }

    if (!paramsetPath.empty()) {
        validateInputFile(paramsetPath);
        std::cout << "Parameter input file: " << paramsetPath << " with size "
                  << size_string(ghc::filesystem::file_size(paramsetPath)) << std::endl;
    }

    std::cout << std::endl;

    validateWorkingDir(workingDirectory);
    std::cout << "Working directory: " << workingDirectory << " with "
              << size_string(ghc::filesystem::space(workingDirectory).available) << " available" << std::endl;

    validateOutputFile(outputFile, forceOverwrite);
    std::cout << "Output file: " << outputFile << " with "
              << size_string(ghc::filesystem::space(parent_dir(outputFile)).available) << " available" << std::endl;

    if (!outputSupFile.empty()) {
        validateOutputFile(outputSupFile, forceOverwrite);
        std::cout << "Output supplementary file: " << outputSupFile << " with "
                  << size_string(ghc::filesystem::space(parent_dir(outputSupFile)).available) << " available"
                  << std::endl;
    }

    std::cout << std::endl;

    UTILS_DIE_IF(qvMode != "none" && qvMode != "lossless", "QVMode " + qvMode + " unknown");
    UTILS_DIE_IF(refMode != "none" && refMode != "relevant" && refMode != "full", "RefMode " + refMode + " unknown");
    UTILS_DIE_IF(readNameMode != "none" && readNameMode != "lossless", "Read name mode " + readNameMode + " unknown");

    if (std::thread::hardware_concurrency()) {
        UTILS_DIE_IF(numberOfThreads < 1 || numberOfThreads > std::thread::hardware_concurrency(),
                     "Invalid number of threads: " + std::to_string(numberOfThreads) +
                         ". Your system supports between 1 and " + std::to_string(std::thread::hardware_concurrency()) +
                         " threads.");
        std::cout << "Threads: " << numberOfThreads << " with " << std::thread::hardware_concurrency() << " supported"
                  << std::endl;
    } else {
        UTILS_DIE_IF(!numberOfThreads,
                     "Could not detect hardware concurrency level. Please provide a number of threads manually.");
        std::cout << "Threads: " << numberOfThreads << " (could not detected supported number automatically)"
                  << std::endl;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace run
}  // namespace genieapp

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
