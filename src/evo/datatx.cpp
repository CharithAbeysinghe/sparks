// Copyright (c) 2018-2024 The Sparks Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <evo/datatx.h>
#include <evo/specialtx.h>

#include <chain.h>
#include <chainparams.h>
#include <consensus/merkle.h>
#include <validation.h>

bool CheckDataTx(const CTransaction& tx, const CBlockIndex* pindexPrev, CValidationState& state)
{    
    if (tx.nType != TRANSACTION_DATA) {
        return state.DoS(100, false, REJECT_INVALID, "bad-datatx-type");
    }

    CDataTx datatx;
    if (!GetTxPayload(tx, datatx)) {
        return state.DoS(100, false, REJECT_INVALID, "bad-datatx-payload");
    }
    
    bool fDataTXActive;
    {
        LOCK(cs_main);
        fDataTXActive = VersionBitsState(pindexPrev, Params().GetConsensus(), Consensus::DEPLOYMENT_DATATX, versionbitscache) == ThresholdState::ACTIVE;
    }

    if (!fDataTXActive) {
        return state.DoS(100, false, REJECT_INVALID, "bad-datatx-not-active");
    }

    return true;
}