/* file: linear_regression_training_online.h */
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
//  Implementation of the interface for linear regression model-based training
//  in the online processing mode
//--
*/

#ifndef __LINEAR_REGRESSION_TRAINING_ONLINE_H__
#define __LINEAR_REGRESSION_TRAINING_ONLINE_H__

#include "algorithms/algorithm.h"
#include "algorithms/linear_regression/linear_regression_training_types.h"
#include "algorithms/linear_model/linear_model_training_online.h"

namespace daal
{
namespace algorithms
{
namespace linear_regression
{
namespace training
{

namespace interface1
{
/**
 * @defgroup linear_regression_online Online
 * @ingroup linear_regression_training
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__LINEAR_REGRESSION__TRAINING__ONLINECONTAINER"></a>
 * \brief Class containing methods for linear regression model-based training
 * in the online processing mode
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT OnlineContainer : public TrainingContainerIface<online>
{
public:
    /**
     * Constructs a container for linear regression model-based training with a specified environment
     * in the online processing mode
     * \param[in] daalEnv   Environment object
     */
    OnlineContainer(daal::services::Environment::env *daalEnv);
    /** Default destructor */
    ~OnlineContainer();

    /**
     * Computes a partial result of linear regression model-based training
     * in the online processing mode
     *
     * \return Status of computations
     */
    services::Status compute() DAAL_C11_OVERRIDE;
    /**
     * Computes the result of linear regression model-based training
     * in the online processing mode
     *
     * \return Status of computations
     */
    services::Status finalizeCompute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__LINEAR_REGRESSION__TRAINING__ONLINE"></a>
 * \brief Provides methods for linear regression model-based training in the online processing mode
 * <!-- \n<a href="DAAL-REF-LINEARREGRESSION-ALGORITHM">Linear regression algorithm description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for
 *                          linear regression model-based training , double or float
 * \tparam method           Linear regression training method, \ref Method
 *
 * \par Enumerations
 *      - \ref Method  Computation methods
 *
 * \par References
 *      - \ref linear_regression::interface1::Model "linear_regression::Model" class
 *      - \ref linear_regression::interface1::ModelNormEq "linear_regression::ModelNormEq" class
 *      - \ref linear_regression::interface1::ModelQR "linear_regression::ModelQR" class
 *      - \ref prediction::interface1::Batch "prediction::Batch" class
 */
template<typename algorithmFPType = DAAL_ALGORITHM_FP_TYPE, Method method = normEqDense>
class DAAL_EXPORT Online : public linear_model::training::Online
{
public:
    Input input; /*!< %Input data structure */
    Parameter parameter; /*!< %Training \ref interface1::Parameter "parameters" */

    /** Default constructor */
    Online()
    {
        initialize();
    }

    /**
     * Constructs a linear regression training algorithm by copying input objects and parameters
     * of another linear regression training algorithm in the online processing mode
     * \param[in] other Algorithm to use as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Online(const Online<algorithmFPType, method> &other) :
        linear_model::training::Online(other), input(other.input), parameter(other.parameter)
    {
        initialize();
    }

    ~Online() {}

    virtual regression::training::Input* getInput() DAAL_C11_OVERRIDE { return &input; }

    /**
     * Returns the method of the algorithm
     * \return Method of the algorithm
     */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int)method; }

    /**
     * Returns the structure that contains a partial result of linear regression model-based training
     * \return Structure that contains a partial result of linear regression model-based training
     */
    PartialResultPtr getPartialResult() { return PartialResult::cast(_partialResult); }

    /**
     * Returns the structure that contains the result of linear regression model-based training
     * \return Structure that contains the result of linear regression model-based training
     */
    ResultPtr getResult() { return Result::cast(_result); }

    /**
     * Returns a pointer to a newly allocated linear regression training algorithm
     * with a copy of the input objects and parameters of this linear regression training algorithm
     * in the online processing mode
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Online<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Online<algorithmFPType, method> >(cloneImpl());
    }

protected:
    virtual Online<algorithmFPType, method> * cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Online<algorithmFPType, method>(*this);
    }

    services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = getResult()->template allocate<algorithmFPType>(&input, &parameter, method);
        _res = _result.get();
        return s;
    }

    services::Status allocatePartialResult() DAAL_C11_OVERRIDE
    {
        services::Status s = getPartialResult()->template allocate<algorithmFPType>(&input, &parameter, method);
        _pres = _partialResult.get();
        return s;
    }

    services::Status initializePartialResult() DAAL_C11_OVERRIDE
    {
        services::Status s = getPartialResult()->template initialize<algorithmFPType>(&input, &parameter, method);
        _pres = _partialResult.get();
        return s;
    }

    void initialize()
    {
        _ac = new __DAAL_ALGORITHM_CONTAINER(online, OnlineContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _par = &parameter;
        _partialResult.reset(new PartialResult());
        _result.reset(new Result());
    }
}; // class  : public Training
/** @} */
} // namespace interface1
using interface1::OnlineContainer;
using interface1::Online;

}
}
}
}
#endif
