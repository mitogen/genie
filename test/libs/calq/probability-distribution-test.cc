#include "../../../src/genie/quality/calq/probability_distribution.h"
#include <gtest/gtest.h>

TEST(ProbabilityDistribution, Generic) {  // NOLINT(cert-err58-cpp)
    size_t rangeMin = 0;
    size_t rangeMax = 1;

    genie::quality::calq::ProbabilityDistribution dist(rangeMin, rangeMax);

    // add()
    EXPECT_NO_THROW(dist.addToPdf(0, 1));
    EXPECT_NO_THROW(dist.addToPdf(1, 1));

    // size()
    EXPECT_EQ(dist.size(), 2);

    // count()
    EXPECT_EQ(dist.getCount(0), 0);
    EXPECT_EQ(dist.getCount(1), 1);

    // operator[]()
    EXPECT_EQ(dist[0], 1);
    EXPECT_EQ(dist[1], 1);
    dist.addToPdf(1, 1);
    EXPECT_EQ(dist[1], 2);

    // rangeMin()
    EXPECT_EQ(dist.getRangeMin(), 0);

    // rangeMax()
    EXPECT_EQ(dist.getRangeMax(), 1);
}
