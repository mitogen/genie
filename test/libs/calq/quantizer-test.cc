#include "../../../src/genie/quality/calq/quantizer.h"
#include <gtest/gtest.h>

TEST(Quantizer, EmptyLut) {  // NOLINT(cert-err58-cpp)
    genie::quality::calq::Quantizer q;

    // The inverse LUT should be empty.
    const std::map<int, int>& inverseLut = q.inverseLut();
    EXPECT_EQ(inverseLut.size(), 0);
}

TEST(Quantizer, PopulatedLut) {  // NOLINT(cert-err58-cpp)
    std::map<int, int> inverseLut = {{0, 0}, {1, 0}, {2, 1}, {3, 1}};
    genie::quality::calq::Quantizer q(inverseLut);

    // These should work
    EXPECT_EQ(q.indexToReconstructionValue(0), 0);
    EXPECT_EQ(q.indexToReconstructionValue(1), 0);
    EXPECT_EQ(q.indexToReconstructionValue(2), 1);
    EXPECT_EQ(q.indexToReconstructionValue(3), 1);

    // Check for correct inverse LUT
    EXPECT_EQ(q.inverseLut(), inverseLut);
}
