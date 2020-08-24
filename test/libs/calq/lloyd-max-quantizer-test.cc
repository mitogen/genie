#include "lloyd_max_quantizer.h"
#include <gtest/gtest.h>

TEST(LloydMaxQuantizer, UniformDistribution) {  // NOLINT(cert-err58-cpp)
    calq::LloydMaxQuantizer q(10);
    calq::ProbabilityDistribution pdf(0, 99);

    for (size_t i = 0; i <= 99; i++) {
        pdf.addToPdf(i);
    }
    q.build(pdf);

    EXPECT_EQ(q.valueToIndex(99), 9);
    EXPECT_EQ(q.valueToIndex(10), 1);
    EXPECT_EQ(q.valueToIndex(9), 0);
    EXPECT_EQ(q.valueToIndex(38), 3);
    EXPECT_EQ(q.valueToIndex(0), 0);
}

TEST(LloydMaxQuantizer, NonUniformDistribution) {  // NOLINT(cert-err58-cpp)
    calq::LloydMaxQuantizer q(10);
    calq::ProbabilityDistribution pdf(1, 100);

    for (int i = 1; i <= 100; ++i) {
        pdf.addToPdf(static_cast<size_t>(i), static_cast<size_t>(pow(abs(i - 50), 4)));
    }
    q.build(pdf);

    EXPECT_EQ(q.valueToIndex(100), 9);
    EXPECT_EQ(q.valueToIndex(94), 8);
    EXPECT_EQ(q.valueToIndex(11), 1);
    EXPECT_EQ(q.valueToIndex(50), 5);
    EXPECT_EQ(q.valueToIndex(73), 5);
    EXPECT_EQ(q.valueToIndex(1), 0);
}
