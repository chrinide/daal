/* file: dtrees_train_data_helper.i */
/*******************************************************************************
* Copyright 2014-2017 Intel Corporation
* All Rights Reserved.
*
* If this  software was obtained  under the  Intel Simplified  Software License,
* the following terms apply:
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*
*
* If this  software  was obtained  under the  Apache License,  Version  2.0 (the
* "License"), the following terms apply:
*
* You may  not use this  file except  in compliance  with  the License.  You may
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*
* Unless  required  by   applicable  law  or  agreed  to  in  writing,  software
* distributed under the License  is distributed  on an  "AS IS"  BASIS,  WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
* See the   License  for the   specific  language   governing   permissions  and
* limitations under the License.
*******************************************************************************/

/*
//++
//  Implementation of auxiliary functions for decision trees train algorithms
//  (defaultDense) method.
//--
*/

#ifndef __DTREES_TRAIN_DATA_HELPER_I__
#define __DTREES_TRAIN_DATA_HELPER_I__

#include "service_memory.h"
#include "daal_defines.h"
#include "service_memory.h"
#include "service_rng.h"
#include "service_numeric_table.h"
#include "service_data_utils.h"
#include "service_sort.h"
#include "service_math.h"
#include "dtrees_feature_type_helper.i"
#include "data_utils.h"

typedef int IndexType;

using namespace daal::internal;
using namespace daal::services::internal;
using namespace daal::algorithms::dtrees::internal;

namespace daal
{
namespace algorithms
{
namespace dtrees
{
namespace training
{
namespace internal
{

//////////////////////////////////////////////////////////////////////////////////////////
// Service functions, compare real values with tolerance
//////////////////////////////////////////////////////////////////////////////////////////
template <typename algorithmFPType, CpuType cpu>
bool isPositive(algorithmFPType val, algorithmFPType eps = algorithmFPType(10)*daal::data_feature_utils::internal::EpsilonVal<algorithmFPType, cpu>::get())
{
    return (val > eps);
}

template <typename algorithmFPType, CpuType cpu>
bool isZero(algorithmFPType val, algorithmFPType eps = algorithmFPType(10)*daal::data_feature_utils::internal::EpsilonVal<algorithmFPType, cpu>::get())
{
    return (val <= eps) && (val >= -eps);
}

template <typename algorithmFPType, CpuType cpu>
bool isGreater(algorithmFPType val1, algorithmFPType val2)
{
    return isPositive<algorithmFPType, cpu>(val1 - val2);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Service function, memcopy src to dst
//////////////////////////////////////////////////////////////////////////////////////////
template<typename T, CpuType cpu>
inline void tmemcpy(T* dst, const T* src, size_t n)
{
    for(size_t i = 0; i < n; ++i)
        dst[i] = src[i];
}

//////////////////////////////////////////////////////////////////////////////////////////
// Service function, randomly permutes given array
//////////////////////////////////////////////////////////////////////////////////////////
template <CpuType cpu>
void shuffle(void* state, size_t n, IndexType* dst)
{
    RNGs<IndexType, cpu> rng;
    IndexType idx[2];
    for(size_t i = 0; i < n; ++i)
    {
        rng.uniform(2, idx, state, 0, n);
        daal::services::internal::swap<cpu, IndexType>(dst[idx[0]], dst[idx[1]]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Service structure, keeps response-dependent split data
//////////////////////////////////////////////////////////////////////////////////////////
template <typename algorithmFPType, typename TImpurityData>
struct SplitData
{
    TImpurityData left;
    algorithmFPType featureValue;
    algorithmFPType impurityDecrease;
    size_t nLeft;
    size_t iStart;
    bool featureUnordered;
    SplitData() : impurityDecrease(-data_management::data_feature_utils::getMaxVal<algorithmFPType>()) {}
    SplitData(algorithmFPType impDecr, bool bFeatureUnordered) : impurityDecrease(impDecr), featureUnordered(bFeatureUnordered){}
    SplitData(const SplitData& o) = delete;
    void copyTo(SplitData& o) const
    {
        o.featureValue = featureValue;
        o.nLeft = nLeft;
        o.iStart = iStart;
        o.left = left;
        o.featureUnordered = featureUnordered;
        o.impurityDecrease = impurityDecrease;
    }
};

template <typename TResponse>
struct SResponse
{
    TResponse val;
    IndexType idx;
};

//////////////////////////////////////////////////////////////////////////////////////////
// DataHelper. Base class for response-specific services classes.
// Keeps indices of the bootstrap samples and provides optimal access to columns in case
// of homogenious numeric table
//////////////////////////////////////////////////////////////////////////////////////////
template <typename algorithmFPType, typename TResponse, CpuType cpu>
class DataHelper
{
public:
    typedef SResponse<TResponse> Response;

public:
    DataHelper(const dtrees::internal::SortedFeaturesHelper* sortedFeatHelper):
        _sortedFeatHelper(sortedFeatHelper), _data(nullptr), _dataDirect(nullptr), _nCols(0){}
    const NumericTable* data() const { return _data; }
    size_t size() const { return _aResponse.size(); }
    TResponse response(size_t i) const { return _aResponse[i].val; }
    const Response* responses() const { return _aResponse.get(); }
    bool reset(size_t n)
    {
        _aResponse.reset(n);
        return _aResponse.get() != nullptr;
    }

    virtual bool init(const NumericTable* data, const NumericTable* resp, const IndexType* aSample)
    {
        DAAL_ASSERT(_aResponse.size());
        _data = const_cast<NumericTable*>(data);
        _nCols = data->getNumberOfColumns();
        const HomogenNumericTable<algorithmFPType>* hmg = dynamic_cast<const HomogenNumericTable<algorithmFPType>*>(data);
        _dataDirect = (hmg ? hmg->getArray() : nullptr);
        const IndexType firstRow = aSample[0];
        const IndexType lastRow = aSample[_aResponse.size() - 1];
        ReadRows<algorithmFPType, cpu> bd(const_cast<NumericTable*>(resp), firstRow, lastRow - firstRow + 1);
        for(size_t i = 0; i < _aResponse.size(); ++i)
        {
            _aResponse[i].idx = aSample[i];
            _aResponse[i].val = TResponse(bd.get()[aSample[i] - firstRow]);
        }
        return true;
    }

    algorithmFPType getValue(size_t iCol, size_t iRow) const
    {
        if(_dataDirect)
            return _dataDirect[iRow*_nCols + iCol];

        data_management::BlockDescriptor<algorithmFPType> bd;
        _data->getBlockOfColumnValues(iCol, iRow, 1, readOnly, bd);
        algorithmFPType val = *bd.getBlockPtr();
        _data->releaseBlockOfColumnValues(bd);
        return val;
    }

    void getColumnValues(size_t iCol, const IndexType* aIdx, size_t n, algorithmFPType* aVal) const
    {
        if(_dataDirect)
        {
            for(size_t i = 0; i < n; ++i)
            {
                const IndexType iRow = getObsIdx(aIdx[i]);
                aVal[i] = _dataDirect[iRow*_nCols + iCol];
            }
        }
        else
        {
            data_management::BlockDescriptor<algorithmFPType> bd;
            for(size_t i = 0; i < n; ++i)
            {
                _data->getBlockOfColumnValues(iCol, getObsIdx(aIdx[i]), 1, readOnly, bd);
                aVal[i] = *bd.getBlockPtr();
                _data->releaseBlockOfColumnValues(bd);
            }
        }
    }

    size_t getNumOOBIndices() const
    {
        if(!_aResponse.size())
            return 0;

        size_t count = _aResponse[0].idx;
        size_t prev = count;
        for(size_t i = 1; i < _aResponse.size(); prev = _aResponse[i++].idx)
            count += (_aResponse[i].idx > (prev + 1) ? (_aResponse[i].idx - prev - 1) : 0);
        const size_t nRows = _data->getNumberOfRows();
        count += (nRows > (prev + 1) ? (nRows - prev - 1) : 0);
        return count;
    }

    void getOOBIndices(IndexType* dst) const
    {
        if(!_aResponse.size())
            return;

        const IndexType* savedDst = dst;
        size_t idx = _aResponse[0].idx;
        size_t iDst = 0;
        for(; iDst < idx; dst[iDst] = iDst, ++iDst);

        for(size_t iResp = 1; iResp < _aResponse.size(); idx = _aResponse[iResp].idx, ++iResp)
        {
            for(++idx; idx < _aResponse[iResp].idx; ++idx, ++iDst)
                dst[iDst] = idx;
        }

        const size_t nRows = _data->getNumberOfRows();
        for(++idx; idx < nRows; ++idx, ++iDst)
            dst[iDst] = idx;
        DAAL_ASSERT(iDst == getNumOOBIndices());
    }
    const dtrees::internal::SortedFeaturesHelper& sortedFeatures() const { DAAL_ASSERT(_sortedFeatHelper); return *_sortedFeatHelper; }

    bool hasDiffFeatureValues(IndexType iFeature, const IndexType* aIdx, size_t n) const
    {
        const SortedFeaturesHelper::IndexType* sortedFeaturesIdx = this->sortedFeatures().data(iFeature);
        const auto aResponse = this->_aResponse.get();
        const SortedFeaturesHelper::IndexType idx0 = sortedFeaturesIdx[aResponse[aIdx[0]].idx];
        size_t i = 1;
        PRAGMA_IVDEP
        PRAGMA_VECTOR_ALWAYS
        for(; i < n; ++i)
        {
            const Response& r = aResponse[aIdx[i]];
            const SortedFeaturesHelper::IndexType idx = sortedFeaturesIdx[r.idx];
            if(idx != idx0)
                break;
        }
        return (i != n);
    }

protected:
    IndexType getObsIdx(size_t i) const { DAAL_ASSERT(i < _aResponse.size());  return _aResponse.get()[i].idx; }

protected:
    const dtrees::internal::SortedFeaturesHelper* _sortedFeatHelper;
    TArray<Response, cpu> _aResponse;
    const algorithmFPType* _dataDirect;
    NumericTable* _data;
    size_t _nCols;
};

//partition given set of indices into the left and right parts
//corresponding to the split feature value (cut value)
//given as the index in the sorted feature values array
//returns index of the row in the dataset corresponding to the split feature value (cut value)
template <typename ResponseType, typename IndexType, typename FeatureIndexType, typename SizeType, CpuType cpu>
int doPartition(SizeType n, const IndexType* aIdx, const ResponseType* aResponse,
    const FeatureIndexType* sortedFeaturesIdx, bool featureUnordered,
    IndexType idxFeatureValueBestSplit,
    IndexType* bestSplitIdxRight, IndexType* bestSplitIdx,
    SizeType nLeft) //for DAAL_ASSERT only
{
    SizeType iLeft = 0;
    SizeType iRight = 0;
    int iRowSplitVal = -1;

    PRAGMA_IVDEP
    PRAGMA_VECTOR_ALWAYS
    for(SizeType i = 0; i < n; ++i)
    {
        const IndexType iSample = aIdx[i];
        const IndexType iRow = aResponse[iSample].idx;
        const FeatureIndexType idx = sortedFeaturesIdx[iRow];

        if((featureUnordered && (idx != idxFeatureValueBestSplit)) || ((!featureUnordered) && (idx > idxFeatureValueBestSplit)))
        {
            bestSplitIdxRight[iRight++] = iSample;
        }
        else
        {
            if(idx == idxFeatureValueBestSplit)
                iRowSplitVal = iRow;
            bestSplitIdx[iLeft++] = iSample;
        }
    }
    DAAL_ASSERT(iRight == n - nLeft);
    DAAL_ASSERT(iLeft == nLeft);
    return iRowSplitVal;
}

} /* namespace internal */
} /* namespace training */
} /* namespace dtrees */
} /* namespace algorithms */
} /* namespace daal */

#endif
