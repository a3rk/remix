// Copyright (c) 2018, The Remix Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/int-util.h"
#include "crypto/hash.h"
#include "cryptonote_config.h"
#include "difficulty.h"
#include <boost/math/special_functions/round.hpp>

#undef REMIX_DEFAULT_LOG_CATEGORY
#define REMIX_DEFAULT_LOG_CATEGORY "difficulty"

namespace cryptonote {

  using std::size_t;
  using std::uint64_t;
  using std::vector;


#if defined(__x86_64__)
  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    low = mul128(a, b, &high);
  }

#else

  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    // __int128 isn't part of the standard, so the previous function wasn't portable. mul128() in Windows is fine,
    // but this portable function should be used elsewhere. Credit for this function goes to latexi95.

    uint64_t aLow = a & 0xFFFFFFFF;
    uint64_t aHigh = a >> 32;
    uint64_t bLow = b & 0xFFFFFFFF;
    uint64_t bHigh = b >> 32;

    uint64_t res = aLow * bLow;
    uint64_t lowRes1 = res & 0xFFFFFFFF;
    uint64_t carry = res >> 32;

    res = aHigh * bLow + carry;
    uint64_t highResHigh1 = res >> 32;
    uint64_t highResLow1 = res & 0xFFFFFFFF;

    res = aLow * bHigh;
    uint64_t lowRes2 = res & 0xFFFFFFFF;
    carry = res >> 32;

    res = aHigh * bHigh + carry;
    uint64_t highResHigh2 = res >> 32;
    uint64_t highResLow2 = res & 0xFFFFFFFF;

    //Addition

    uint64_t r = highResLow1 + lowRes2;
    carry = r >> 32;
    low = (r << 32) | lowRes1;
    r = highResHigh1 + highResLow2 + carry;
    uint64_t d3 = r & 0xFFFFFFFF;
    carry = r >> 32;
    r = highResHigh2 + carry;
    high = d3 | (r << 32);
  }

#endif

  static inline bool cadd(uint64_t a, uint64_t b) {
    return a + b < a;
  }

  static inline bool cadc(uint64_t a, uint64_t b, bool c) {
    return a + b < a || (c && a + b == (uint64_t) -1);
  }

  bool check_hash(const crypto::hash &hash, difficulty_type difficulty) {
    uint64_t low, high, top, cur;
    // First check the highest word, this will most likely fail for a random hash.
    mul(swap64le(((const uint64_t *) &hash)[3]), difficulty, top, high);
    if (high != 0) {
      return false;
    }
    mul(swap64le(((const uint64_t *) &hash)[0]), difficulty, low, cur);
    mul(swap64le(((const uint64_t *) &hash)[1]), difficulty, low, high);
    bool carry = cadd(cur, low);
    cur = high;
    mul(swap64le(((const uint64_t *) &hash)[2]), difficulty, low, high);
    carry = cadc(cur, low, carry);
    carry = cadc(high, top, carry);
    return !carry;
  }
  
  difficulty_type next_difficulty(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds) {

    const int64_t T = static_cast<int64_t>(target_seconds);
    size_t N = DIFFICULTY_WINDOW;
    int64_t FTL = static_cast<int64_t>(CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT);

    // Return a difficulty of 1 for first 3 blocks if it's the start of the chain.
    if (timestamps.size() < 4) {
      return 1;    
    }
    // Otherwise, use a smaller N if the start of the chain is less than N+1.
    else if ( timestamps.size() < N+1 ) {
      N = timestamps.size() - 1;
    }
    // Otherwise make sure timestamps and cumulative_difficulties are correct size.
    else {
      timestamps.resize(N+1);
      cumulative_difficulties.resize(N+1);
    }
    // To get an average solvetime to within +/- ~0.1%, use an adjustment factor.
    // adjust=0.998 for N = 60
    const double adjust = 0.998;
    // The divisor k normalizes the LWMA sum to a standard LWMA.
    const double k = N * (N + 1) / 2;    
    
    double LWMA(0), sum_inverse_D(0), harmonic_mean_D(0), nextDifficulty(0);
    int64_t solveTime(0);
    uint64_t difficulty(0), next_difficulty(0);

    // Loop through N most recent blocks. N is most recently solved block.
    for (size_t i = 1; i <= N; i++) {
      solveTime = static_cast<int64_t>(timestamps[i]) - static_cast<int64_t>(timestamps[i - 1]);
      solveTime = std::min<int64_t>((T * 10), std::max<int64_t>(solveTime, -FTL));
      difficulty = cumulative_difficulties[i] - cumulative_difficulties[i - 1];
      LWMA += (int64_t)(solveTime * i) / k;
      sum_inverse_D += 1 / static_cast<double>(difficulty);
    }

    harmonic_mean_D = N / sum_inverse_D;

    // Limit LWMA same as Bitcoin's 1/4 in case something unforeseen occurs.
    if (static_cast<int64_t>(boost::math::round(LWMA)) < T / 4)
      LWMA = static_cast<double>(T / 4);

    nextDifficulty = harmonic_mean_D * T / LWMA * adjust;

    // No limits should be employed, but this is correct way to employ a 20% symmetrical limit:
    // nextDifficulty=max(previous_Difficulty*0.8,min(previous_Difficulty/0.8, next_Difficulty));
    next_difficulty = static_cast<uint64_t>(nextDifficulty);
    return next_difficulty;
  }


  // LWMA-2 difficulty algorithm (commented version)
  // Copyright (c) 2017-2018 Zawy
  // https://github.com/zawy12/difficulty-algorithms/issues/3
  // Bitcoin clones must lower their FTL. 
  // Cryptonote et al coins must make the following changes:
  // #define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW    11
  // #define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT        3 * DIFFICULTY_TARGET 
  // #define DIFFICULTY_WINDOW                      60 + 1
  // Do not sort timestamps.  CN coins must deploy the Jagerman MTP Patch. See:
  // https://github.com/loki-project/loki/pull/26   or
  // https://github.com/wownero/wownero/commit/1f6760533fcec0d84a6bd68369e0ea26716b01e7

  // New coins:  "return 100;" should have the 100 changed based on expected hashrate.

  difficulty_type next_difficulty_v2(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds, size_t height) {
    
    
    double T = DIFFICULTY_TARGET; // target solvetime seconds
    double N = DIFFICULTY_WINDOW - 1; //  N=45, 60, and 90 for T=600, 120, 60.
    double FTL = CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT; // < 3xT
    double L(0), sum_6_ST(0), sum_9_ST(0), ST, next_D, prev_D, SMAn, SMAd;

    // If expecting a 10x decrease or 1000x increase in D after a fork, consider: 
    // if ( height >= fork_height && height < fork_height+62 )  { return uint64_t difficulty_guess; }

    // TS and CD vectors should be size 61 after startup.
    if ( timestamps.size() > N  ) { timestamps.resize(N+1); cumulative_difficulties.resize(N+1);  }
    else if (timestamps.size() < 4)  {  return 100;  } // start up
    else  {  N = timestamps.size() - 1;  } // start up

    // N is most recently solved block. i must be signed
    for ( int64_t i = 1; i <= N; i++) {  
        // +/- FTL limits are bad timestamp protection.  6xT limits drop in D to reduce oscillations.
        ST = std::max(-FTL, std::min(double(timestamps[i] - timestamps[i-1]), 6*T));
        L +=  ST * i ; // Give more weight to most recent blocks.
        // Do these inside loop to capture -FTL and +6*T limitations.
        if ( i > N-6 ) { sum_6_ST += ST; }      
        if ( i > N-9 ) { sum_9_ST += ST; }     
    }
    if (L < T*N) { L= T*N*6; } // sanity limit. Accidentally limits D rise at startup for ~60 blocks.

    // Calculate next_D = avgD * T / LWMA(STs) 
    next_D = (cumulative_difficulties[N] - cumulative_difficulties[0]) * T*(N+1)*0.991 / (L*2);

    // LWMA-2 change from LWMA
    // Trigger sudden increase in D if hash attack is detected. Also limit rate of fall in D.
    // ====  begin LWMA-2 trigger and fall limitations  ====

    prev_D = cumulative_difficulties[N] - cumulative_difficulties[N-1];

    // Use Digishield-type tempered SMA to get long-term (~4*N) avg D to limit how high 
    // consecutive triggers can send D.
    SMAn = (cumulative_difficulties[N] - cumulative_difficulties[0]) * 4 *T;
    SMAd = 3*N + double( timestamps[N] - timestamps[0] ); 

    // If a trigger would send D>1.7*avgD, then don't do it.
    // 1.70 allows 30% increase for 2 consecutive blocks and 14% for 4 blocks.
    // Tempered SMA = SMAn / SMAd, but do not divide for round off protection

    if  ( 1.14*next_D*SMAd > 1.70*SMAn ) {  
        if ( next_D < 0.7*prev_D ) { next_D = 0.7*prev_D; }
    }
    else if  ( 1.3*next_D*SMAd < 1.70*SMAn && sum_6_ST < 1.2*T )   {
        next_D = 1.3*prev_D; 
    }
    else if (sum_9_ST < 3.4*T )   {  next_D = 1.14*prev_D;  }
    // Prevent D falling too much after triggers, as long as D is not > 1.7*avgD
    else if  ( next_D < 0.9*prev_D )  { next_D = 0.9*prev_D;  }

    // ==== end LWMA-2's trigger and fall limitations ====

    // Prevent round off difference between systems to prevent chain split.
    // Theoretically not needed by C++ standard.  Requires D > 1. Needs 1E12 > D > 100.

    if  ( ceil(next_D + 0.01) > ceil(next_D) ) { next_D = ceil(next_D + 0.03);  }

    // next_Target = sumTargets*L*2/0.998/T/(N+1)/N/N; // To show the difference.

    return static_cast<uint64_t>(next_D);
    
  }
}





