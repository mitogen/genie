/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#include "genie/run/main.h"
#include <iostream>
#include <string>
#include "cli11/CLI11.hpp"
#include "genie/module/manager.h"
#include "genie/util/runtime-exception.h"

// ---------------------------------------------------------------------------------------------------------------------

static void printCmdLine(int argc, char* argv[]) {
    std::cout << "genie: command line: ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
}

// ---------------------------------------------------------------------------------------------------------------------

int stat(int, char*[]) { UTILS_DIE("Stat not implemented"); }

// ---------------------------------------------------------------------------------------------------------------------

int help(int, char*[]) {
    std::cout << "Usage: \ngenie run -i [input-file] -o [output-file]\nUse -f to overwrite existing output files.\nUse "
                 "-t to specify the number of threads.\nIn case of paired fastq files, use --input-suppl-file or "
                 "--output-suppl-file to specify the second file."
              << std::endl;
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    printCmdLine(argc, argv);

#ifdef GENIE_USE_OPENMP
    std::cout << "genie: built with OpenMP\n\n" << std::endl;
#else
    std::cout << "genie: *not* built with OpenMP\n\n" << std::endl;
#endif
    genie::module::detect();
    constexpr int OPERATION_INDEX = 1;
    try {
        UTILS_DIE_IF(argc <= OPERATION_INDEX, "No operation specified, type 'genie help' for more info.");
        std::string operation = argv[OPERATION_INDEX];
        transform(operation.begin(), operation.end(), operation.begin(),
                  [](char x) -> char { return static_cast<char>(tolower(x)); });
        if (operation == "run") {
            genieapp::run::main(argc - OPERATION_INDEX, argv + OPERATION_INDEX);
        } else if (operation == "stat") {
            stat(argc - OPERATION_INDEX, argv + OPERATION_INDEX);
        } else if (operation == "help") {
            help(argc - OPERATION_INDEX, argv + OPERATION_INDEX);
        } else {
            UTILS_DIE("Unknown operation " + operation);
        }
    } catch (const genie::util::Exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
