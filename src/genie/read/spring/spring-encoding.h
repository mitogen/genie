/**
 * @file
 * @copyright This file is part of GENIE. See LICENSE and/or
 * https://github.com/mitogen/genie for more details.
 */

#ifndef SRC_GENIE_READ_SPRING_SPRING_ENCODING_H_
#define SRC_GENIE_READ_SPRING_SPRING_ENCODING_H_

// ---------------------------------------------------------------------------------------------------------------------

#ifdef GENIE_USE_OPENMP
#include <omp.h>
#endif

// ---------------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include "genie/read/spring/bitset-util.h"
#include "genie/read/spring/params.h"
#include "genie/read/spring/util.h"

// ---------------------------------------------------------------------------------------------------------------------

namespace genie {
namespace read {
namespace spring {

/**
 *
 * @tparam bitset_size
 */
template <size_t bitset_size>
struct encoder_global_b {
    std::bitset<bitset_size> **basemask;  //!<
    int max_readlen;                      //!<
    // bitset for A,G,C,T,N at each position
    // used in stringtobitset, and bitsettostring
    std::bitset<bitset_size> mask63;  //!< bitset with 63 bits set to 1 (used in
    // bitsettostring for conversion to ullong)

    /**
     *
     * @param max_readlen_param
     */
    explicit encoder_global_b(int max_readlen_param);

    /**
     *
     */
    ~encoder_global_b();
};

/**
 *
 */
struct encoder_global {
    uint32_t numreads{};               //!<
    uint32_t numreads_s{};             //!<
    uint32_t numreads_N{};             //!<
    int numdict_s = NUM_DICT_ENCODER;  //!<

    int max_readlen{}, num_thr{};  //!<

    std::string basedir;            //!<
    std::string infile;             //!<
    std::string infile_flag;        //!<
    std::string infile_pos;         //!<
    std::string infile_seq;         //!<
    std::string infile_RC;          //!<
    std::string infile_readlength;  //!<
    std::string infile_N;           //!<
    std::string outfile_unaligned;  //!<
    std::string outfile_seq;        //!<
    std::string outfile_pos;        //!<
    std::string outfile_noise;      //!<
    std::string outfile_noisepos;   //!<
    std::string infile_order;       //!<
    std::string infile_order_N;     //!<

    char enc_noise[128][128]{};  //!<

    encoder_global() = default;
};

/**
 *
 */
struct contig_reads {
    std::string read;      //!<
    int64_t pos;           //!<
    char RC;               //!<
    uint32_t order;        //!<
    uint16_t read_length;  //!<
};

/**
 *
 * @param current_contig
 * @param list_size
 * @return
 */
std::string buildcontig(std::list<contig_reads> &current_contig, const uint32_t &list_size);

/**
 *
 * @param ref
 * @param current_contig
 * @param f_seq
 * @param f_pos
 * @param f_noise
 * @param f_noisepos
 * @param f_order
 * @param f_RC
 * @param f_readlength
 * @param abs_pos
 */
void writecontig(const std::string &ref, std::list<contig_reads> &current_contig, std::ofstream &f_seq,
                 std::ofstream &f_pos, std::ofstream &f_noise, std::ofstream &f_noisepos, std::ofstream &f_order,
                 std::ofstream &f_RC, std::ofstream &f_readlength, uint64_t &abs_pos);

/**
 *
 * @param eg
 * @param cp
 */
void getDataParams(encoder_global &eg, const compression_params &cp);

/**
 *
 * @param order_s
 * @param eg
 */
void correct_order(uint32_t *order_s, const encoder_global &eg);

/**
 *
 * @tparam bitset_size
 * @param b
 * @param readlen
 * @param egb
 * @return
 */
template <size_t bitset_size>
std::string bitsettostring(std::bitset<bitset_size> b, const uint16_t readlen,
                           const encoder_global_b<bitset_size> &egb);

/**
 *
 * @tparam bitset_size
 * @param read
 * @param dict
 * @param order_s
 * @param read_lengths_s
 * @param eg
 * @param egb
 */
template <size_t bitset_size>
void encode(std::bitset<bitset_size> *read, bbhashdict *dict, uint32_t *order_s, uint16_t *read_lengths_s,
            const encoder_global &eg, const encoder_global_b<bitset_size> &egb);

/**
 *
 * @tparam bitset_size
 * @param eg
 * @param egb
 */
template <size_t bitset_size>
void setglobalarrays(encoder_global &eg, encoder_global_b<bitset_size> &egb);

/**
 *
 * @tparam bitset_size
 * @param read
 * @param order_s
 * @param read_lengths_s
 * @param eg
 * @param egb
 */
template <size_t bitset_size>
void readsingletons(std::bitset<bitset_size> *read, uint32_t *order_s, uint16_t *read_lengths_s,
                    const encoder_global &eg, const encoder_global_b<bitset_size> &egb);

/**
 *
 * @tparam bitset_size
 * @param temp_dir
 * @param cp
 */
template <size_t bitset_size>
void encoder_main(const std::string &temp_dir, const compression_params &cp);

// ---------------------------------------------------------------------------------------------------------------------

}  // namespace spring
}  // namespace read
}  // namespace genie

// ---------------------------------------------------------------------------------------------------------------------

#include "genie/read/spring/spring-encoding.impl.h"

// ---------------------------------------------------------------------------------------------------------------------

#endif  // SRC_GENIE_READ_SPRING_SPRING_ENCODING_H_

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
