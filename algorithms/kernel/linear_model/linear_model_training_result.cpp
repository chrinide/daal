/* file: linear_model_training_result.cpp */
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
//  Implementation of the class defining the result of the regression training algorithm
//--
*/

#include "services/daal_defines.h"
#include "algorithms/linear_model/linear_model_training_types.h"
#include "serialization_utils.h"
#include "daal_strings.h"

namespace daal
{
namespace algorithms
{
namespace linear_model
{
namespace training
{
namespace interface1
{
using namespace daal::data_management;
using namespace daal::services;

Result::Result(size_t nElements) : regression::training::Result(nElements)
{}

linear_model::ModelPtr Result::get(ResultId id) const
{
    return linear_model::Model::cast(regression::training::Result::get(regression::training::ResultId(id)));
}

void Result::set(ResultId id, const linear_model::ModelPtr &value)
{
    regression::training::Result::set(regression::training::ResultId(id), value);
}

Status Result::check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *par, int method) const
{
    Status s;
    DAAL_CHECK_STATUS(s, regression::training::Result::check(input, par, method));

    const Input *in = static_cast<const Input*>(input);
    const linear_model::ModelPtr model = get(training::model);
    const size_t nFeatures = in->get(data)->getNumberOfColumns();
    DAAL_CHECK_EX(model->getNumberOfFeatures() == nFeatures, ErrorIncorrectNumberOfFeatures, services::ArgumentName, modelStr())

    const size_t nBeta = nFeatures + 1;
    const size_t nResponses = in->get(dependentVariables)->getNumberOfColumns();

    DAAL_CHECK_STATUS(s, linear_model::checkModel(model.get(), *par, nBeta, nResponses));

    return s;
}

}
}
}
}
}
