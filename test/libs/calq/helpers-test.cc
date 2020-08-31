#include "helpers.h"
#include <gtest/gtest.h>
#include "../../../src/genie/quality/calq/helpers.h"

TEST(Helpers, FileExists) {  // NOLINT(cert-err58-cpp)
    std::string gitRootDir = calq_tests::exec("git rev-parse --show-toplevel");

    EXPECT_EQ(genie::quality::calq::fileExists(gitRootDir + "/README.md"), true);
    EXPECT_EQ(genie::quality::calq::fileExists(gitRootDir + "/nonexistent"), false);
}

TEST(Helpers, FileNameExtension) {  // NOLINT(cert-err58-cpp)
    std::string gitRootDir = calq_tests::exec("git rev-parse --show-toplevel");

    EXPECT_EQ(genie::quality::calq::fileNameExtension(gitRootDir + "/README.md"), "md");
    EXPECT_EQ(genie::quality::calq::fileNameExtension(gitRootDir + "/no-extension"), "");
}
