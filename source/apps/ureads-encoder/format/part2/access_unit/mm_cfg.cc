#include "mm_cfg.h"
#include "ureads-encoder/format/part2/bitwriter.h"

// -----------------------------------------------------------------------------------------------------------------

namespace format {
    void MmCfg::write(BitWriter *writer) {
        writer->write(mm_threshold, 16);
        writer->write(mm_count, 32);
    }

    // -----------------------------------------------------------------------------------------------------------------

    MmCfg::MmCfg(uint16_t _mm_threshold, uint32_t _mm_count) : mm_threshold(_mm_threshold), mm_count(_mm_count) {

    }
}