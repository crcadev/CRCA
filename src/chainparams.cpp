// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018-2019 The Crca Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "spork.h"

#include <assert.h>
#include <limits>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("000004266b062cfc9690fe3e4ca4d1219a504a048a2ffed7446b9b6f280192e0"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1560878434, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1560878434,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1560878434,
    0,
    100};

void MineGenesis(CBlock genesis)
{
    printf("Searching for genesis block...\n");
    // This will figure out a valid hash and Nonce if you're
    // creating a different genesis block:
    uint256 hashTarget = ~uint256(0) >> 20;
    uint256 thash;
    while(true)
    {
        thash = genesis.GetHash();
        if (thash <= hashTarget)
            break;
        if ((genesis.nNonce & 0xFFF) == 0)
        {
            printf("nonce %u: hash = %s \n", genesis.nNonce, thash.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0)
        {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesis.nTime;
        }
    }
    printf("block.nTime = %u \n", genesis.nTime);
    printf("block.nNonce = %u \n", genesis.nNonce);
    printf("block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
    printf("block.merkle = %s\n", genesis.hashMerkleRoot.ToString().c_str());
    std::fflush(stdout);
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x81;
        pchMessageStart[1] = 0x6F;
        pchMessageStart[2] = 0x6E;
        pchMessageStart[3] = 0x65;
        vAlertPubKey = ParseHex("0493908caa06be69788da7193634a55d2f500c7091cb796e33c5d7d2bbc09ed456daefac54273a5ea69447b860cf96cc9722f7320aac5fdc96ebec86339a7b1075");

        nDefaultPort = 12888;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        bnStartWork = ~uint256(0) >> 24;

        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 120;
        nTargetSpacingSlowLaunch = 1 * 60;
        nMaturity = 50;
        nMasternodeCountDrift = 3;
        nMaxMoneyOut = 50000000 * COIN;

        nStartMasternodePaymentsBlock = 64;

        nLastPOWBlock = 250;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Bitsa Is a Crypto Card Europeans Can Top Up With BCH";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = pow(11 ,3) * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04812070ec0f87a6a73340c61e77ed33eb4e2c05782e1e7b4f76ba1129df1882866d5c28bc4b315a0f36c9bcc78dd1dc2d0ce30583bb571772d00494e4a1efd672") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1560878434;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 1426828;
		

	//MineGenesis(genesis);
   //std::cout << genesis.ToString() << std::endl;
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("000004266b062cfc9690fe3e4ca4d1219a504a048a2ffed7446b9b6f280192e0"));
        assert(genesis.hashMerkleRoot == uint256("76fb3c41b18a6fcbd3fec89afcd4d47b133ee0adacb07a7a3a8a9cde17ee0835"));

        vSeeds.push_back(CDNSSeedData("165.22.91.118", "165.22.91.118")); 
        vSeeds.push_back(CDNSSeedData("165.22.87.13", "165.22.87.13")); 
		 vSeeds.push_back(CDNSSeedData("165.22.89.154", "165.22.89.154")); 
        vSeeds.push_back(CDNSSeedData("165.22.92.144", "165.22.92.144"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 28); // P
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 8);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 118); // p
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 9984
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x27)(0x00).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        strDevFeeAddress = "CbBhjdpQN98oWKVy2rNZkRKJQtRUAEqmMu";//devfee address
		nStakeInputMinimal = 1 * COIN;//Stake Input Minimal

        nPoolMaxTransactions = 3;
        strSporkKey = "040bcdf8a4f16977719bb9b10a256243e262aae2adc56de3b0a9e7855ca8c166f9eccd68efd58993b7d857740d36b690d5926906c395189c69b7f04b76ea3e0f00";
        strObfuscationPoolDummyAddress = "PSDj2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1534859399; //Wed, 25 Jun 2014 20:36:16 GMT

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x47;
        pchMessageStart[1] = 0x77;
        pchMessageStart[2] = 0x66;
        pchMessageStart[3] = 0xbb;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnStartWork = bnProofOfWorkLimit;

        vAlertPubKey = ParseHex("04cdb69c28dab825d75ecf43875bc639dfc81465823659a68371962c835056d1093297047ba06554a5a5009366f7631fa1239feec97a2b5ab4b91d1898438e55f4");
        nDefaultPort = 11555;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 30;  // Crca: 1 minute
        nLastPOWBlock = std::numeric_limits<decltype(nLastPOWBlock)>::max();
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();
        nMaxMoneyOut = 21000000 * COIN;
        strDevFeeAddress = "x8gCKTiW3fPa1Zp4KcyGSssgbgLpwxgu2s";
		nStakeInputMinimal = 500 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1560878434;
        genesis.nNonce = 1426828;

        hashGenesisBlock = genesis.GetHash();

        //assert(hashGenesisBlock == uint256("00000dff081a9486786e9240df20c96a3758e8b1969bb41716d3bf731f4648a2"));

        vSeeds.push_back(CDNSSeedData("tests1.crca.im", "tests1.crca.im")); 
        vSeeds.push_back(CDNSSeedData("tests2.crca.im", "tests2.crca.im")); 
		vSeeds.push_back(CDNSSeedData("tests3.crca.im", "tests3.crca.im")); 
        vSeeds.push_back(CDNSSeedData("tests4.crca.im", "tests4.crca.im"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 137); // Testnet Crca addresses start with 'x'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet Crca script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet Crca BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet Crca BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet Crca BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04f501e9db3ae69cc31c46c8ceeed0728cca4ef14d3f294a3743738b5eabc70c30245157373f30b8a012d55c0ff6ee2bc2e8b48ba6eac4f6202ac6760294ab8e77";
        strObfuscationPoolDummyAddress = "x57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1534859399; //Fri, 09 Jan 2015 21:05:58 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;

        bnStartWork = ~uint256(0) >> 20;

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;        // Crca: 1 minute
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1560878434;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 1426828;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;

        //assert(hashGenesisBlock == uint256("300552a9db8b2921c3c07e5bbf8694df5099db579742e243daeaf5008b1e74de"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fMineBlocksOnDemand = true;


    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
