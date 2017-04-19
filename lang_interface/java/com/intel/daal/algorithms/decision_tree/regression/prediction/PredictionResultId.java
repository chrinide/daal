/* file: PredictionResultId.java */
/*******************************************************************************
* Copyright 2014-2017 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
 * @ingroup decision_tree_regression_prediction
 * @{
 */
package com.intel.daal.algorithms.decision_tree.regression.prediction;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_TREE__REGRESSION__PREDICTION__PREDICTIONRESULTID"></a>
 * @brief Available identifiers of the result for making decision tree model-based prediction
 */
public final class PredictionResultId {
    private int _value;

    /** Default constructor */
    public PredictionResultId(int value) {
        _value = value;
    }

    /**
     * Returns a value corresponding to the identifier of the object
     * \return Value corresponding to the identifier
     */
    public int getValue() {
        return _value;
    }

    private static final int predictionId = 0;

    public static final PredictionResultId prediction = new PredictionResultId(predictionId); /*!< Result of decision tree model-based prediction */

}
/** @} */