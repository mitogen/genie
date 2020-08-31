#include "filter_buffer.h"
#include <gtest/gtest.h>

TEST(FilterBuffer, Convolution) {  // NOLINT(cert-err58-cpp)
    genie::quality::calq::FilterBuffer buffer([](size_t pos, size_t size) -> double { return pos / static_cast<double>(size - 1); }, 3);

    EXPECT_EQ(buffer.filter(), 0);
    buffer.push(1);
    EXPECT_EQ(buffer.filter(), 1);
    buffer.push(2);
    EXPECT_EQ(buffer.filter(), 2.5);
    buffer.push(3);
    EXPECT_EQ(buffer.filter(), 4);
    buffer.push(4);
}

TEST(FilterBuffer, Offset) {  // NOLINT(cert-err58-cpp)
    genie::quality::calq::FilterBuffer buffer([](size_t pos, size_t size) -> double { return pos / static_cast<double>(size - 1); }, 3);

    EXPECT_EQ(buffer.getOffset(), 2);
}
