#include "sam_pileup.h"
#include <gtest/gtest.h>
#include "sam_pileup_deque.h"
#include "qual_encoder.h"

TEST(SamPileupDeque, Generic) {  // NOLINT(cert-err58-cpp)
    calq::EncodingRead record1;
    record1.posMin = 0;
    record1.posMax = 6;
    record1.reference = "chr1";
    record1.cigar = "7M";
    record1.sequence = "GATTACA";
    record1.qvalues = "QUALITY";

    calq::SAMPileupDeque deque;

    // TODO(Jan): The function add() should work w/o requiring the user to call setPos[Min|Max]() first.
    deque.setPosMin(record1.posMin);
    deque.setPosMax(record1.posMax);

    uint8_t qualOffset = 33;
    uint8_t hqSoftclipThreshold = 0;

    deque.add(record1, qualOffset, hqSoftclipThreshold);
}
