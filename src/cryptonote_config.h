// Copyright (c) 2018, The Remix Project
// Copyright (c) 2014-2017, The Monero Project
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
// Parts of this file are originally copyright (c) 2014-2017, The Monero Project

#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

#define CRYPTONOTE_DNS_TIMEOUT_MS                       20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER                     500000000
#define CRYPTONOTE_MAX_BLOCK_SIZE                       500000000  // block header blob limit, never used!
#define CRYPTONOTE_GETBLOCKTEMPLATE_MAX_BLOCK_SIZE	    196608 //size of block (bytes) that is the maximum that miners will produce
#define CRYPTONOTE_MAX_TX_SIZE                          1000000000
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER          0
#define CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW            60
#define CURRENT_TRANSACTION_VERSION                     1
#define CURRENT_BLOCK_MAJOR_VERSION                     1
#define CURRENT_BLOCK_MINOR_VERSION                     0
#define CRYPTONOTE_POW_BLOCK_VERSION                    1

#define CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE             10

// MONEY_SUPPLY - total number coins to be generated
#define MONEY_SUPPLY                                    ((uint64_t)(-1))
#define DEV_ALLOC                                       ((uint64_t)1106804000000000001)
#define EMISSION_SPEED_FACTOR_PER_MINUTE                (20)
#define FINAL_SUBSIDY_PER_MINUTE                        ((uint64_t)300000000000) // 3 * pow(10, 11)

#define CRYPTONOTE_REWARD_BLOCKS_WINDOW                 100
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE    300000  


#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE          600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT                12
// COIN - number of smallest units in one coin
#define COIN                                            ((uint64_t)1000000000000) // pow(10, 12)

#define FEE_PER_KB                                      ((uint64_t)2000000000) // 2 * pow(10, 9)
#define DEFAULT_FEE                                     ((uint64_t)10000000000) // pow(10, 10)
#define MINIMUM_RELAY_FEE                               ((uint64_t)1000000) // pow(10, 6)
#define DYNAMIC_FEE_PER_KB_BASE_BLOCK_REWARD            ((uint64_t)10000000000000) // 10 * pow(10,12)
#define DYNAMIC_FEE_PER_KB_BASE_FEE                     ((uint64_t)400000000)

#define ORPHANED_BLOCKS_MAX_COUNT                       100

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW               11
#define DIFFICULTY_TARGET                               120  // seconds

#define DIFFICULTY_WINDOW                               60 + 1 // blocks
#define DIFFICULTY_LAG                                  15  // !!!
#define DIFFICULTY_CUT                                  6  // timestamps to cut after sorting, only used in tests
#define DIFFICULTY_BLOCKS_COUNT                         DIFFICULTY_WINDOW + 1

#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT              3*DIFFICULTY_TARGET

#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS   DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS       1


#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN             DIFFICULTY_TARGET //just alias; used by tests


#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT          10000  //by default, blocks ids count in synchronizing
#define BLOCKS_SYNCHRONIZING_DEFAULT_COUNT              200

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME                  86400 //seconds, one day
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME   604800 //seconds, one week

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT           1000

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT                  1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT                   5000

#define P2P_DEFAULT_CONNECTIONS_COUNT                   8
#define P2P_DEFAULT_HANDSHAKE_INTERVAL                  60           //seconds
#define P2P_DEFAULT_PACKET_MAX_SIZE                     50000000     //50000000 bytes maximum packet size
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE                  250
#define P2P_DEFAULT_CONNECTION_TIMEOUT                  5000       //5 seconds
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT             2000       //2 seconds
#define P2P_DEFAULT_INVOKE_TIMEOUT                      60*2*1000  //2 minutes
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT            5000       //5 seconds
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT       70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT            2

#define P2P_FAILED_ADDR_FORGET_SECONDS                  (60*60)     //1 hour
#define P2P_IP_BLOCKTIME                                (60*60*24)  //24 hour
#define P2P_IP_FAILS_BEFORE_BLOCK                       10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL               (5*60) //5 minutes

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS                  0x01
#define P2P_SUPPORT_FLAGS                               P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

#define ALLOW_DEBUG_COMMANDS

#define CRYPTONOTE_NAME                                 "remix"
#define CRYPTONOTE_POOLDATA_FILENAME                    "poolstate.bin"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME              "data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME         "lock.mdb"
#define P2P_NET_DATA_FILENAME                           "p2pstate.bin"
#define MINER_CONFIG_FILE_NAME                          "miner_conf.json"

#define THREAD_STACK_SIZE                               5 * 1024 * 1024

#define HF_VERSION_DYNAMIC_FEE                          255
#define HF_VERSION_MIN_MIXIN_4                          2
#define HF_VERSION_ENFORCE_RCT                          255

#define PER_KB_FEE_QUANTIZATION_DECIMALS                8

#define HASH_OF_HASHES_STEP                             256

#define DONATION_URL                                    "donate.remix.cash"
#define DONATION_EMAIL_ADDRESS                          "donate@remix.cash"

// New constants are intended to go here
namespace config
{
  uint8_t const FEE_CALCULATION_MAX_RETRIES = 10;

  uint16_t const P2P_DEFAULT_PORT = 11330;
  uint16_t const RPC_DEFAULT_PORT = 11331;
  uint16_t const ZMQ_RPC_DEFAULT_PORT = 11332;

  uint32_t const GENESIS_NONCE = 10013;

  uint64_t const DEFAULT_FEE_ATOMIC_XMR_PER_KB = 500; // Just a placeholder!  Change me!
  uint64_t const DEFAULT_DUST_THRESHOLD = ((uint64_t)2000000000); // 2 * pow(10, 9)
  uint64_t const BASE_REWARD_CLAMP_THRESHOLD = ((uint64_t)100000000); // pow(10, 8)
  uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x3dee10; // Wallet Starts with REMXi
  uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0xfca18ec90; //REEmix
  uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x67d311a; //SubRM

  std::string const P2P_REMOTE_DEBUG_TRUSTED_PUB_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

  boost::uuids::uuid const NETWORK_ID = { 
    { 0x45, 0x36, 0x78, 0x0f, 0x45, 0xd2, 0x11, 0xe8, 0xac, 0x80, 0x28, 0x18, 0x78, 0xb8, 0x64, 0x80 } 
  }; //RemixCoin MAINNET
  
  std::string const GENESIS_TX = "013c01ff0001ffffffffffff0f029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd088071210128dbc932d1c2cb30cf4acb9d6d73467d24bd931289745232ecea3f42c89e7346";
  
  std::string const DONATION_REMIX_WALLET_ADDRESS = "TODO";
  std::string const DONATION_REMIX_WALLET_PUBLIC_VIEWKEY = "TODO";
  std::string const DONATION_BITCOIN_WALLET_ADDRESS = "TODO";

  namespace testnet
  {
    uint16_t const P2P_DEFAULT_PORT = 21330;
    uint16_t const RPC_DEFAULT_PORT = 21331;
    uint16_t const ZMQ_RPC_DEFAULT_PORT = 21332;

    uint32_t const GENESIS_NONCE = 10001;

    uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x2d4b45; // Wallet Starts with a3rkx
    uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x509c5; //Starts with a3in
    uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0xe8bc5; //a3su

    boost::uuids::uuid const NETWORK_ID = { 
      { 0x76, 0x36, 0x78, 0x0f, 0x45, 0xd2, 0x11, 0xe8, 0xac, 0x80, 0x28, 0x18, 0x78, 0xb8, 0x64, 0x15 } 
    };

    //std::string const GENESIS_TX = "013c01ff0001ffffffffffff03029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd0880712101a3b2db6ba6d8941ee26cfc96cd57bfa62562eb38e9c940520b847daa8967f374"; //A3RK Test Net
    //std::string const GENESIS_TX = "013c01ff00018180edbffb8b8aae0f02c5a0320489885cb0836a1deb229bb2b00cccd0dc1ce97f42f3b9b4884fa745e521010b49aadfb8358450760ec5d90e912409b0f3d4e20e2bf6fb304c0f5c0c2a5753";
    std::string const GENESIS_TX = "013c01ff0005ff8fe3a410027a6ee1dfd475b8e9fe858fd5a49e8e04c7060718ea2624bdd269c4708b4e1d0e80c0fc82aa0202829bed24f779b807176fa1a24b54c847bff3c561784fb735b822043d6e511f2680d0dbc3f40202a33d81ea8befdf0fe801b896f8a1de0cadc5db24e296e3739b6192e3b34b742980a0e5b9c2910102b52058b51f7b6fc79214439d2019cb3238b3d3eface72b6e87c21ccddec72fc080c0dfda8ee906026d9a649ae94b34865ecbc1bfef2058d6015e607b804ae176c4cb25046ae1915e2101da30a1d7c21c0e8af1d3ef4682163ca12baaa159458d91ebe592073cf7b675f8";
    std::string const DONATION_REMIX_WALLET_ADDRESS = "Wx22Jx6NsuXViqX6dGiM1gXdzZBphgpPBAo1WMG8HdxLQUXKzn48X4sFXHKUbtjmDNUkmuurbFvA9HmNSh7cVDwT2KoiMvVoY";
    std::string const DONATION_REMIX_WALLET_PUBLIC_VIEWKEY = "55df2eb6cfc056d2692a405442bda5ef47d4cc898576643b3a2248bffed69a3c";
  }
}
