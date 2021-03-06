/* file: gbt_regression_training_batch.h */
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
//  Implementation of the interface for model-based training
//  in the batch processing mode
//--
*/

#ifndef __GBT_REGRESSSION_TRAINING_BATCH_H__
#define __GBT_REGRESSSION_TRAINING_BATCH_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "services/daal_defines.h"
#include "services/daal_memory.h"
#include "algorithms/gradient_boosted_trees/gbt_regression_training_types.h"
#include "algorithms/gradient_boosted_trees/gbt_regression_model.h"
#include "algorithms/regression/regression_training_batch.h"

namespace daal
{
namespace algorithms
{
namespace gbt
{
namespace regression
{
namespace training
{
namespace interface1
{
/**
 * @defgroup gbt_regression_training_batch Batch
 * @ingroup gbt_regression_training
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__GBT__REGRESSION__TRAINING__BATCHCONTAINER"></a>
 * \brief Class containing methods for gradient boosted trees regression
 *        model-based training using algorithmFPType precision arithmetic
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public TrainingContainerIface<batch>
{
public:
    /**
     * Constructs a container for model-based training with a specified environment
     * in the batch processing mode
     * \param[in] daalEnv   Environment object
     */
    BatchContainer(daal::services::Environment::env *daalEnv);
    /** Default destructor */
    ~BatchContainer();
    /**
     * Computes the result of model-based training in the batch processing mode
     * \return Status of computations
     */
    services::Status compute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__GBT__REGRESSION__TRAINING__BATCH"></a>
 * \brief Provides methods for model-based training in the batch processing mode
 * <!-- \n<a href="DAAL-REF-GBT__REGRESSION__TRAINING-ALGORITHM">gradient boosted trees algorithm description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for model-based training, double or float
 * \tparam method           gradient boosted trees training method, \ref Method
 *
 * \par Enumerations
 *      - \ref Method  Computation methods
 *
 * \par References
 *      - \ref gbt::regression::interface1::Model "Model" class
 *      - \ref prediction::interface1::Batch "prediction::Batch" class
 */
template<typename algorithmFPType = DAAL_ALGORITHM_FP_TYPE, Method method = defaultDense>
class DAAL_EXPORT Batch : public algorithms::regression::training::Batch
{
public:
    Input input; /*!< %Input data structure */

    /** Default constructor */
    Batch()
    {
        _par = new Parameter();
        initialize();
    }

    /**
     * Constructs a gradient boosted trees training algorithm by copying input objects
     * and parameters of another gradient boosted trees training algorithm in the batch processing mode
     * \param[in] other Algorithm to use as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other) : input(other.input)
    {
        _par = new Parameter(other.parameter());
        initialize();
    }

    /** Destructor */
    ~Batch()
    {
        delete _par;
    }

    /**
    * Gets parameter of the algorithm
    * \return parameter of the algorithm
    */
    Parameter& parameter() { return *static_cast<Parameter*>(_par); }

    /**
    * Gets parameter of the algorithm
    * \return parameter of the algorithm
    */
    const Parameter& parameter() const { return *static_cast<const Parameter*>(_par); }

    /**
    * Get input objects for the algorithm
    * \return input objects of the algorithm
    */
    virtual algorithms::regression::training::Input* getInput() DAAL_C11_OVERRIDE{ return &input; }

    /**
     * Returns the method of the algorithm
     * \return Method of the algorithm
     */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int)method; }

    /**
     * Returns the structure that contains the result of model-based training
     * \return Structure that contains the result of model-based training
     */
    ResultPtr getResult() { return Result::cast(_result); }

    /**
     * Returns a pointer to a newly allocated gradient boosted trees training algorithm
     * with a copy of the input objects and parameters for this gradient boosted trees training algorithm
     * in the batch processing mode
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

protected:

    virtual Batch<algorithmFPType, method> * cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = getResult()->template allocate<algorithmFPType>(&input, &parameter(), method);
        _res = _result.get();
        return s;
    }

    void initialize()
    {
        _ac  = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _result.reset(new Result());
    }
};
/** @} */
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;
}
}
}
}
}
#endif
