/* file: qr_distr_step3_result.cpp */
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
//  Implementation of qr classes.
//--
*/

#include "algorithms/qr/qr_types.h"
#include "serialization_utils.h"
#include "daal_strings.h"

using namespace daal::data_management;
using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace qr
{
namespace interface1
{
__DAAL_REGISTER_SERIALIZATION_CLASS(DistributedPartialResultStep3, SERIALIZATION_QR_DISTRIBUTED_PARTIAL_RESULT_STEP3_ID);

/** Default constructor */
DistributedPartialResultStep3::DistributedPartialResultStep3() : daal::algorithms::PartialResult(lastDistributedPartialResultStep3Id + 1) {}

/**
 * Returns the result of the QR decomposition algorithm with the matrix Q calculated
 * \param[in] id    Identifier of the result
 * \return          Result that corresponds to the given identifier
 */
ResultPtr DistributedPartialResultStep3::get(DistributedPartialResultStep3Id id) const
{
    return staticPointerCast<Result, SerializationIface>(Argument::get(id));
}

/**
 * Sets Result object to store the result of the QR decomposition algorithm
 * \param[in] id    Identifier of the result
 * \param[in] value Pointer to the Result object
 */
void DistributedPartialResultStep3::set(DistributedPartialResultStep3Id id, const ResultPtr &value)
{
    Argument::set(id, staticPointerCast<SerializationIface, Result>(value));
}

/**
 * Checks partial results of the algorithm
 * \param[in] input     Pointer to the input object
 * \param[in] parameter Pointer to parameters
 * \param[in] method Computation method
 */
Status DistributedPartialResultStep3::check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, int method) const
{
    DistributedStep3Input *qrInput   = static_cast<DistributedStep3Input *>(const_cast<daal::algorithms::Input *>(input  ));
    int unexpectedLayouts = (int)packed_mask;
    DataCollectionPtr qCollection = qrInput->get(inputOfStep3FromStep1);
    size_t nFeatures = 0;
    size_t nVectors = 0;
    size_t qSize = qCollection->size();
    for(size_t i = 0 ; i < qSize ; i++)
    {
        NumericTable  *numTableInQCollection = static_cast<NumericTable *>((*qCollection)[i].get());
        nFeatures  = numTableInQCollection->getNumberOfColumns();
        nVectors += numTableInQCollection->getNumberOfRows();
    }
    if(get(finalResultFromStep3))
    {
        Status s = checkNumericTable(get(finalResultFromStep3)->get(matrixQ).get(), matrixQStr(), unexpectedLayouts, 0, nFeatures, nVectors);
        if(!s) { return s; }
    }
    return Status();
}

/**
 * Checks partial results of the algorithm
 * \param[in] parameter Pointer to parameters
 * \param[in] method Computation method
 */
Status DistributedPartialResultStep3::check(const daal::algorithms::Parameter *parameter, int method) const
{
    int unexpectedLayouts = (int)packed_mask;
    if(get(finalResultFromStep3))
    {
        Status s =  checkNumericTable(get(finalResultFromStep3)->get(matrixQ).get(), matrixQStr(), unexpectedLayouts);
        if(!s) { return s; }
    }
    return Status();
}

} // namespace interface1
} // namespace qr
} // namespace algorithm
} // namespace daal
