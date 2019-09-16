#ifndef GENIE_QV_CODEBOOK_H
#define GENIE_QV_CODEBOOK_H

// -----------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <vector>

// -----------------------------------------------------------------------------------------------------------------

namespace format {
    class BitWriter;
    namespace qv_coding1 {

        /**
        * ISO 23092-2 Section 3.3.2.2 table 9 Lines 4 to 7
        */
        class QvCodebook {
        private:
            uint8_t qv_num_codebook_entries : 8; //!< Line 4
            std::vector<uint8_t> qv_recon; //!< Lines 5 to 7

        public:
            QvCodebook();

            void addEntry(uint8_t entry);

            virtual void write(BitWriter *writer) const;
        };
    }
}

// -----------------------------------------------------------------------------------------------------------------

#endif //GENIE_QV_CODEBOOK_H