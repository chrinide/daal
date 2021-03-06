@echo off
rem ============================================================================
rem Copyright 2017 Intel Corporation
rem All Rights Reserved.
rem
rem If this software  was obtained under the Intel  Simplified Software License,
rem the following terms apply:
rem
rem The source code,  information and material ("Material")  contained herein is
rem owned by Intel Corporation or its suppliers or licensors,  and title to such
rem Material remains with Intel Corporation or its suppliers  or licensors.  The
rem Material contains  proprietary  information  of Intel  or its  suppliers and
rem licensors.  The Material is protected by worldwide copyright laws and treaty
rem provisions.  No part  of the  Material   may be  used,  copied,  reproduced,
rem modified,  published,   uploaded,   posted,   transmitted,   distributed  or
rem disclosed in any way without Intel's prior  express written  permission.  No
rem license under any patent,  copyright  or other intellectual  property rights
rem in the Material is granted to or conferred upon you,  either  expressly,  by
rem implication,  inducement,  estoppel  or otherwise.  Any  license  under such
rem intellectual  property  rights must  be  express and  approved  by  Intel in
rem writing.
rem
rem Unless otherwise  agreed by  Intel in writing,  you may not  remove or alter
rem this notice or  any other notice  embedded in Materials by  Intel or Intel's
rem suppliers or licensors in any way.
rem
rem
rem If this software was obtained  under the Apache  License,  Version  2.0 (the
rem "License"), the following terms apply:
rem
rem You may not  use this file  except in compliance  with the License.  You may
rem obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
rem
rem
rem Unless  required  by  applicable  law  or  agreed  to in  writing,  software
rem distributed under the License is distributed  on an "AS IS"  BASIS,  WITHOUT
rem WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
rem
rem See the  License  for  the  specific  language  governing   permissions  and
rem limitations under the License.
rem ============================================================================

::  Content:
::     Intel(R) Data Analytics Acceleration Library samples list
::******************************************************************************

set MPI_SAMPLE_LIST=svd_fast_distributed_mpi                      ^
                    qr_fast_distributed_mpi                       ^
                    linear_regression_norm_eq_distributed_mpi     ^
                    linear_regression_qr_distributed_mpi          ^
                    pca_correlation_dense_distributed_mpi         ^
                    pca_correlation_csr_distributed_mpi           ^
                    pca_svd_distributed_mpi                       ^
                    covariance_dense_distributed_mpi              ^
                    covariance_csr_distributed_mpi                ^
                    multinomial_naive_bayes_dense_distributed_mpi ^
                    multinomial_naive_bayes_csr_distributed_mpi   ^
                    kmeans_dense_distributed_mpi                  ^
                    kmeans_csr_distributed_mpi                    ^
                    kmeans_init_dense_distributed_mpi             ^
                    kmeans_init_csr_distributed_mpi               ^
                    low_order_moments_csr_distributed_mpi         ^
                    low_order_moments_dense_distributed_mpi       ^
                    implicit_als_csr_distributed_mpi              ^
                    ridge_regression_norm_eq_distributed_mpi      ^
                    neural_net_dense_distributed_mpi              ^
                    neural_net_dense_allgather_distributed_mpi    ^
                    neural_net_dense_asynch_distributed_mpi
