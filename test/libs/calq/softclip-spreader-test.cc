#include "softclip_spreader.h"
#include <gtest/gtest.h>

TEST(SoftclipSpreader, Everything) {  // NOLINT(cert-err58-cpp)
    calq::SoftclipSpreader b(5, 3, false);

    EXPECT_EQ(b.getOffset(), 5);

    // Push 0 out
    EXPECT_EQ(b.push(1, 1), 0);
    EXPECT_EQ(b.push(1, 2), 0);
    EXPECT_EQ(b.push(1, 2), 0);
    EXPECT_EQ(b.push(1, 2), 0);
    EXPECT_EQ(b.push(1, 3), 0);

    // One base not affected by spread
    EXPECT_EQ(b.push(1, 3), 1);

    // One base affected by 1 spread
    EXPECT_EQ(b.push(1, 2), 2);

    // Six bases affected by 2 spreads
    EXPECT_EQ(b.push(1, 2), 3);
    EXPECT_EQ(b.push(1, 2), 3);
    EXPECT_EQ(b.push(1, 2), 3);
    EXPECT_EQ(b.push(1, 2), 3);
    EXPECT_EQ(b.push(1, 2), 3);
    EXPECT_EQ(b.push(1, 2), 3);

    // One base affected by 1 spread
    EXPECT_EQ(b.push(1, 1), 2);

    // No bases affected anymore
    EXPECT_EQ(b.push(1, 0), 1);
    EXPECT_EQ(b.push(1, 0), 1);
}
