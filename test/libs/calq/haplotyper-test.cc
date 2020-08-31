#include "../../../src/genie/quality/calq/haplotyper.h"
#include <gtest/gtest.h>
#include "../../../src/genie/quality/calq/data-structures.h"
//#include "calq_coder.h"

TEST(Haplotyper, Everything) {  // NOLINT(cert-err58-cpp)
    genie::quality::calq::Haplotyper h(5, 2, 33, 8, 5, 3, 5, false, true, genie::quality::calq::FilterType::GAUSS);

    EXPECT_EQ(h.getOffset(), 10);

    // First 5 pushes into base spreader
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(h.push("C", "}", 0, 'A'), 0);
    }

    // Next pushes into filter buffer, approaching 0.73
    for (int i = 0; i < 10; ++i) {
        h.push("C", "}", 0, 'A');
    }
    EXPECT_EQ(h.push("C", "}", 0, 'A'), 7);
    EXPECT_EQ(h.push("C", "}", 0, 'A'), 7);

    // Reset
    genie::quality::calq::Haplotyper h2(5, 2, 33, 8, 5, 3, 5, false, true, genie::quality::calq::FilterType::GAUSS);

    h2.push("CCC", "}}}", 15, 'A');

    // Push spread bases into filter buffer
    for (int i = 0; i < 8; ++i) {
        h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C');  // activity close to 0
    }

    // Reach maximum
    EXPECT_EQ(h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C'), 7);
    EXPECT_EQ(h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C'), 7);
    EXPECT_EQ(h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C'), 7);

    // Reach minimum
    for (int i = 0; i < 9; ++i) {
        h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C');
    }

    EXPECT_EQ(h2.push("CCCCCCCCCCCCCC", "}}}}}}}}}}}}", 0, 'C'), 0);
}
