#include "uniform_min_max_quantizer.h"
#include <gtest/gtest.h>

TEST(UniformMinMaxQuantizer, Initialization) {  // NOLINT(cert-err58-cpp)
    calq::UniformMinMaxQuantizer q3(0, 1, 2);
}

TEST(UniformMinMaxQuantizer, PositiveRange) {  // NOLINT(cert-err58-cpp)
    calq::UniformMinMaxQuantizer q(0, 4, 2);

    EXPECT_EQ(q.valueToIndex(0), 0);
    EXPECT_EQ(q.valueToIndex(1), 0);
    EXPECT_EQ(q.valueToIndex(2), 0);
    EXPECT_EQ(q.valueToIndex(3), 1);
    EXPECT_EQ(q.valueToIndex(4), 1);

    EXPECT_EQ(q.indexToReconstructionValue(0), 0);
    EXPECT_EQ(q.indexToReconstructionValue(1), 4);
}

TEST(UniformMinMaxQuantizer, NegativeRange) {  // NOLINT(cert-err58-cpp)
    calq::UniformMinMaxQuantizer q(-5, -1, 2);

    EXPECT_EQ(q.valueToIndex(-5), 0);
    EXPECT_EQ(q.valueToIndex(-4), 0);
    EXPECT_EQ(q.valueToIndex(-3), 0);
    EXPECT_EQ(q.valueToIndex(-2), 1);
    EXPECT_EQ(q.valueToIndex(-1), 1);

    EXPECT_EQ(q.indexToReconstructionValue(0), -5);
    EXPECT_EQ(q.indexToReconstructionValue(1), -1);
}

TEST(UniformMinMaxQuantizer, EightBinning) {  // NOLINT(cert-err58-cpp)
    calq::UniformMinMaxQuantizer q(0, 40, 8);

    EXPECT_EQ(q.valueToIndex(0), 0);
    EXPECT_EQ(q.valueToIndex(1), 0);
    EXPECT_EQ(q.valueToIndex(2), 0);
    EXPECT_EQ(q.valueToIndex(3), 0);
    EXPECT_EQ(q.valueToIndex(4), 0);
    EXPECT_EQ(q.valueToIndex(5), 0);
    EXPECT_EQ(q.valueToIndex(6), 1);
    EXPECT_EQ(q.valueToIndex(7), 1);
    EXPECT_EQ(q.valueToIndex(8), 1);
    EXPECT_EQ(q.valueToIndex(9), 1);
    EXPECT_EQ(q.valueToIndex(10), 1);
    EXPECT_EQ(q.valueToIndex(11), 2);
    EXPECT_EQ(q.valueToIndex(12), 2);
    EXPECT_EQ(q.valueToIndex(13), 2);
    EXPECT_EQ(q.valueToIndex(14), 2);
    EXPECT_EQ(q.valueToIndex(15), 2);
    EXPECT_EQ(q.valueToIndex(16), 3);
    EXPECT_EQ(q.valueToIndex(17), 3);
    EXPECT_EQ(q.valueToIndex(18), 3);
    EXPECT_EQ(q.valueToIndex(19), 3);
    EXPECT_EQ(q.valueToIndex(20), 3);
    EXPECT_EQ(q.valueToIndex(21), 4);
    EXPECT_EQ(q.valueToIndex(22), 4);
    EXPECT_EQ(q.valueToIndex(23), 4);
    EXPECT_EQ(q.valueToIndex(24), 4);
    EXPECT_EQ(q.valueToIndex(25), 4);
    EXPECT_EQ(q.valueToIndex(26), 5);
    EXPECT_EQ(q.valueToIndex(27), 5);
    EXPECT_EQ(q.valueToIndex(28), 5);
    EXPECT_EQ(q.valueToIndex(29), 5);
    EXPECT_EQ(q.valueToIndex(30), 5);
    EXPECT_EQ(q.valueToIndex(31), 6);
    EXPECT_EQ(q.valueToIndex(32), 6);
    EXPECT_EQ(q.valueToIndex(33), 6);
    EXPECT_EQ(q.valueToIndex(34), 6);
    EXPECT_EQ(q.valueToIndex(35), 6);
    EXPECT_EQ(q.valueToIndex(36), 7);
    EXPECT_EQ(q.valueToIndex(37), 7);
    EXPECT_EQ(q.valueToIndex(38), 7);
    EXPECT_EQ(q.valueToIndex(39), 7);
    EXPECT_EQ(q.valueToIndex(40), 7);

    EXPECT_EQ(q.indexToReconstructionValue(0), 0);
    EXPECT_EQ(q.indexToReconstructionValue(1), 8);
    EXPECT_EQ(q.indexToReconstructionValue(2), 13);
    EXPECT_EQ(q.indexToReconstructionValue(3), 18);
    EXPECT_EQ(q.indexToReconstructionValue(4), 23);
    EXPECT_EQ(q.indexToReconstructionValue(5), 28);
    EXPECT_EQ(q.indexToReconstructionValue(6), 33);
    EXPECT_EQ(q.indexToReconstructionValue(7), 40);
}
