#include "helpers.h"
#include <gtest/gtest.h>
#include "../../../src/genie/quality/calq/helpers.h"

TEST(Helpers, FileExists) {  // NOLINT(cert-err58-cpp)
    std::string gitRootDir = calq_tests::exec("git rev-parse --show-toplevel");

    EXPECT_EQ(calq::fileExists(gitRootDir + "/README.md"), true);
    EXPECT_EQ(calq::fileExists(gitRootDir + "/nonexistent"), false);
}

TEST(Helpers, FileNameExtension) {  // NOLINT(cert-err58-cpp)
    std::string gitRootDir = calq_tests::exec("git rev-parse --show-toplevel");

    EXPECT_EQ(calq::fileNameExtension(gitRootDir + "/README.md"), "md");
    EXPECT_EQ(calq::fileNameExtension(gitRootDir + "/no-extension"), "");
}
