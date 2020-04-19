/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri Dec  6 18:43:02 2019
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */


#include "network.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "layers.h"

#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#define AI_TOOLS_VERSION_MAJOR 5
#define AI_TOOLS_VERSION_MINOR 0
#define AI_TOOLS_VERSION_MICRO 0


#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 3
#define AI_TOOLS_API_VERSION_MICRO 0

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "ed0f5c96578a005e889b88030283a176"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     "(rev-5.0.0)"
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri Dec  6 18:43:02 2019"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array quantize_conv_preds_scratch0_array;   /* Array #0 */
AI_STATIC ai_array quantize_conv_pw_11_scratch1_array;   /* Array #1 */
AI_STATIC ai_array quantize_conv_pw_11_scratch0_array;   /* Array #2 */
AI_STATIC ai_array quantize_conv_dw_11_scratch0_array;   /* Array #3 */
AI_STATIC ai_array quantize_conv_pw_10_scratch0_array;   /* Array #4 */
AI_STATIC ai_array quantize_conv_dw_10_scratch0_array;   /* Array #5 */
AI_STATIC ai_array quantize_conv_pw_9_scratch0_array;   /* Array #6 */
AI_STATIC ai_array quantize_conv_dw_9_scratch0_array;   /* Array #7 */
AI_STATIC ai_array quantize_conv_pw_8_scratch0_array;   /* Array #8 */
AI_STATIC ai_array quantize_conv_dw_8_scratch0_array;   /* Array #9 */
AI_STATIC ai_array quantize_conv_pw_7_scratch0_array;   /* Array #10 */
AI_STATIC ai_array quantize_conv_dw_7_scratch0_array;   /* Array #11 */
AI_STATIC ai_array quantize_conv_pw_6_scratch0_array;   /* Array #12 */
AI_STATIC ai_array quantize_conv_dw_6_scratch0_array;   /* Array #13 */
AI_STATIC ai_array quantize_conv_pw_5_scratch0_array;   /* Array #14 */
AI_STATIC ai_array quantize_conv_dw_5_scratch0_array;   /* Array #15 */
AI_STATIC ai_array quantize_conv_pw_4_scratch0_array;   /* Array #16 */
AI_STATIC ai_array quantize_conv_dw_4_scratch0_array;   /* Array #17 */
AI_STATIC ai_array quantize_conv_pw_3_scratch0_array;   /* Array #18 */
AI_STATIC ai_array quantize_conv_dw_3_scratch0_array;   /* Array #19 */
AI_STATIC ai_array quantize_conv_pw_2_scratch0_array;   /* Array #20 */
AI_STATIC ai_array quantize_conv_dw_2_scratch0_array;   /* Array #21 */
AI_STATIC ai_array quantize_conv_pw_1_scratch0_array;   /* Array #22 */
AI_STATIC ai_array quantize_conv_dw_1_scratch0_array;   /* Array #23 */
AI_STATIC ai_array quantize_conv1_scratch0_array;   /* Array #24 */
AI_STATIC ai_array quantize_conv_preds_bias_array;   /* Array #25 */
AI_STATIC ai_array quantize_conv_preds_weights_array;   /* Array #26 */
AI_STATIC ai_array quantize_conv_pw_11_bias_array;   /* Array #27 */
AI_STATIC ai_array quantize_conv_pw_11_weights_array;   /* Array #28 */
AI_STATIC ai_array quantize_conv_dw_11_bias_array;   /* Array #29 */
AI_STATIC ai_array quantize_conv_dw_11_weights_array;   /* Array #30 */
AI_STATIC ai_array quantize_conv_pw_10_bias_array;   /* Array #31 */
AI_STATIC ai_array quantize_conv_pw_10_weights_array;   /* Array #32 */
AI_STATIC ai_array quantize_conv_dw_10_bias_array;   /* Array #33 */
AI_STATIC ai_array quantize_conv_dw_10_weights_array;   /* Array #34 */
AI_STATIC ai_array quantize_conv_pw_9_bias_array;   /* Array #35 */
AI_STATIC ai_array quantize_conv_pw_9_weights_array;   /* Array #36 */
AI_STATIC ai_array quantize_conv_dw_9_bias_array;   /* Array #37 */
AI_STATIC ai_array quantize_conv_dw_9_weights_array;   /* Array #38 */
AI_STATIC ai_array quantize_conv_pw_8_bias_array;   /* Array #39 */
AI_STATIC ai_array quantize_conv_pw_8_weights_array;   /* Array #40 */
AI_STATIC ai_array quantize_conv_dw_8_bias_array;   /* Array #41 */
AI_STATIC ai_array quantize_conv_dw_8_weights_array;   /* Array #42 */
AI_STATIC ai_array quantize_conv_pw_7_bias_array;   /* Array #43 */
AI_STATIC ai_array quantize_conv_pw_7_weights_array;   /* Array #44 */
AI_STATIC ai_array quantize_conv_dw_7_bias_array;   /* Array #45 */
AI_STATIC ai_array quantize_conv_dw_7_weights_array;   /* Array #46 */
AI_STATIC ai_array quantize_conv_pw_6_bias_array;   /* Array #47 */
AI_STATIC ai_array quantize_conv_pw_6_weights_array;   /* Array #48 */
AI_STATIC ai_array quantize_conv_dw_6_bias_array;   /* Array #49 */
AI_STATIC ai_array quantize_conv_dw_6_weights_array;   /* Array #50 */
AI_STATIC ai_array quantize_conv_pw_5_bias_array;   /* Array #51 */
AI_STATIC ai_array quantize_conv_pw_5_weights_array;   /* Array #52 */
AI_STATIC ai_array quantize_conv_dw_5_bias_array;   /* Array #53 */
AI_STATIC ai_array quantize_conv_dw_5_weights_array;   /* Array #54 */
AI_STATIC ai_array quantize_conv_pw_4_bias_array;   /* Array #55 */
AI_STATIC ai_array quantize_conv_pw_4_weights_array;   /* Array #56 */
AI_STATIC ai_array quantize_conv_dw_4_bias_array;   /* Array #57 */
AI_STATIC ai_array quantize_conv_dw_4_weights_array;   /* Array #58 */
AI_STATIC ai_array quantize_conv_pw_3_bias_array;   /* Array #59 */
AI_STATIC ai_array quantize_conv_pw_3_weights_array;   /* Array #60 */
AI_STATIC ai_array quantize_conv_dw_3_bias_array;   /* Array #61 */
AI_STATIC ai_array quantize_conv_dw_3_weights_array;   /* Array #62 */
AI_STATIC ai_array quantize_conv_pw_2_bias_array;   /* Array #63 */
AI_STATIC ai_array quantize_conv_pw_2_weights_array;   /* Array #64 */
AI_STATIC ai_array quantize_conv_dw_2_bias_array;   /* Array #65 */
AI_STATIC ai_array quantize_conv_dw_2_weights_array;   /* Array #66 */
AI_STATIC ai_array quantize_conv_pw_1_bias_array;   /* Array #67 */
AI_STATIC ai_array quantize_conv_pw_1_weights_array;   /* Array #68 */
AI_STATIC ai_array quantize_conv_dw_1_bias_array;   /* Array #69 */
AI_STATIC ai_array quantize_conv_dw_1_weights_array;   /* Array #70 */
AI_STATIC ai_array quantize_conv1_bias_array;   /* Array #71 */
AI_STATIC ai_array quantize_conv1_weights_array;   /* Array #72 */
AI_STATIC ai_array quantize_input_2_output_array;   /* Array #73 */
AI_STATIC ai_array quantize_conv1_output_array;   /* Array #74 */
AI_STATIC ai_array quantize_conv_dw_1_output_array;   /* Array #75 */
AI_STATIC ai_array quantize_conv_pw_1_output_array;   /* Array #76 */
AI_STATIC ai_array quantize_conv_dw_2_output_array;   /* Array #77 */
AI_STATIC ai_array quantize_conv_pw_2_output_array;   /* Array #78 */
AI_STATIC ai_array quantize_conv_dw_3_output_array;   /* Array #79 */
AI_STATIC ai_array quantize_conv_pw_3_output_array;   /* Array #80 */
AI_STATIC ai_array quantize_conv_dw_4_output_array;   /* Array #81 */
AI_STATIC ai_array quantize_conv_pw_4_output_array;   /* Array #82 */
AI_STATIC ai_array quantize_conv_dw_5_output_array;   /* Array #83 */
AI_STATIC ai_array quantize_conv_pw_5_output_array;   /* Array #84 */
AI_STATIC ai_array quantize_conv_dw_6_output_array;   /* Array #85 */
AI_STATIC ai_array quantize_conv_pw_6_output_array;   /* Array #86 */
AI_STATIC ai_array quantize_conv_dw_7_output_array;   /* Array #87 */
AI_STATIC ai_array quantize_conv_pw_7_output_array;   /* Array #88 */
AI_STATIC ai_array quantize_conv_dw_8_output_array;   /* Array #89 */
AI_STATIC ai_array quantize_conv_pw_8_output_array;   /* Array #90 */
AI_STATIC ai_array quantize_conv_dw_9_output_array;   /* Array #91 */
AI_STATIC ai_array quantize_conv_pw_9_output_array;   /* Array #92 */
AI_STATIC ai_array quantize_conv_dw_10_output_array;   /* Array #93 */
AI_STATIC ai_array quantize_conv_pw_10_output_array;   /* Array #94 */
AI_STATIC ai_array quantize_conv_dw_11_output_array;   /* Array #95 */
AI_STATIC ai_array quantize_conv_pw_11_output_array;   /* Array #96 */
AI_STATIC ai_array quantize_conv_preds_output_array;   /* Array #97 */
AI_STATIC ai_array quantize_conv_preds_fmt_output_array;   /* Array #98 */
AI_STATIC ai_array act_softmax_output_array;   /* Array #99 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor quantize_conv_preds_scratch0;   /* Tensor #0 */
AI_STATIC ai_tensor quantize_conv_pw_11_scratch1;   /* Tensor #1 */
AI_STATIC ai_tensor quantize_conv_pw_11_scratch0;   /* Tensor #2 */
AI_STATIC ai_tensor quantize_conv_dw_11_scratch0;   /* Tensor #3 */
AI_STATIC ai_tensor quantize_conv_pw_10_scratch0;   /* Tensor #4 */
AI_STATIC ai_tensor quantize_conv_dw_10_scratch0;   /* Tensor #5 */
AI_STATIC ai_tensor quantize_conv_pw_9_scratch0;   /* Tensor #6 */
AI_STATIC ai_tensor quantize_conv_dw_9_scratch0;   /* Tensor #7 */
AI_STATIC ai_tensor quantize_conv_pw_8_scratch0;   /* Tensor #8 */
AI_STATIC ai_tensor quantize_conv_dw_8_scratch0;   /* Tensor #9 */
AI_STATIC ai_tensor quantize_conv_pw_7_scratch0;   /* Tensor #10 */
AI_STATIC ai_tensor quantize_conv_dw_7_scratch0;   /* Tensor #11 */
AI_STATIC ai_tensor quantize_conv_pw_6_scratch0;   /* Tensor #12 */
AI_STATIC ai_tensor quantize_conv_dw_6_scratch0;   /* Tensor #13 */
AI_STATIC ai_tensor quantize_conv_pw_5_scratch0;   /* Tensor #14 */
AI_STATIC ai_tensor quantize_conv_dw_5_scratch0;   /* Tensor #15 */
AI_STATIC ai_tensor quantize_conv_pw_4_scratch0;   /* Tensor #16 */
AI_STATIC ai_tensor quantize_conv_dw_4_scratch0;   /* Tensor #17 */
AI_STATIC ai_tensor quantize_conv_pw_3_scratch0;   /* Tensor #18 */
AI_STATIC ai_tensor quantize_conv_dw_3_scratch0;   /* Tensor #19 */
AI_STATIC ai_tensor quantize_conv_pw_2_scratch0;   /* Tensor #20 */
AI_STATIC ai_tensor quantize_conv_dw_2_scratch0;   /* Tensor #21 */
AI_STATIC ai_tensor quantize_conv_pw_1_scratch0;   /* Tensor #22 */
AI_STATIC ai_tensor quantize_conv_dw_1_scratch0;   /* Tensor #23 */
AI_STATIC ai_tensor quantize_conv1_scratch0;   /* Tensor #24 */
AI_STATIC ai_tensor quantize_conv_preds_bias;   /* Tensor #25 */
AI_STATIC ai_tensor quantize_conv_preds_weights;   /* Tensor #26 */
AI_STATIC ai_tensor quantize_conv_pw_11_bias;   /* Tensor #27 */
AI_STATIC ai_tensor quantize_conv_pw_11_weights;   /* Tensor #28 */
AI_STATIC ai_tensor quantize_conv_dw_11_bias;   /* Tensor #29 */
AI_STATIC ai_tensor quantize_conv_dw_11_weights;   /* Tensor #30 */
AI_STATIC ai_tensor quantize_conv_pw_10_bias;   /* Tensor #31 */
AI_STATIC ai_tensor quantize_conv_pw_10_weights;   /* Tensor #32 */
AI_STATIC ai_tensor quantize_conv_dw_10_bias;   /* Tensor #33 */
AI_STATIC ai_tensor quantize_conv_dw_10_weights;   /* Tensor #34 */
AI_STATIC ai_tensor quantize_conv_pw_9_bias;   /* Tensor #35 */
AI_STATIC ai_tensor quantize_conv_pw_9_weights;   /* Tensor #36 */
AI_STATIC ai_tensor quantize_conv_dw_9_bias;   /* Tensor #37 */
AI_STATIC ai_tensor quantize_conv_dw_9_weights;   /* Tensor #38 */
AI_STATIC ai_tensor quantize_conv_pw_8_bias;   /* Tensor #39 */
AI_STATIC ai_tensor quantize_conv_pw_8_weights;   /* Tensor #40 */
AI_STATIC ai_tensor quantize_conv_dw_8_bias;   /* Tensor #41 */
AI_STATIC ai_tensor quantize_conv_dw_8_weights;   /* Tensor #42 */
AI_STATIC ai_tensor quantize_conv_pw_7_bias;   /* Tensor #43 */
AI_STATIC ai_tensor quantize_conv_pw_7_weights;   /* Tensor #44 */
AI_STATIC ai_tensor quantize_conv_dw_7_bias;   /* Tensor #45 */
AI_STATIC ai_tensor quantize_conv_dw_7_weights;   /* Tensor #46 */
AI_STATIC ai_tensor quantize_conv_pw_6_bias;   /* Tensor #47 */
AI_STATIC ai_tensor quantize_conv_pw_6_weights;   /* Tensor #48 */
AI_STATIC ai_tensor quantize_conv_dw_6_bias;   /* Tensor #49 */
AI_STATIC ai_tensor quantize_conv_dw_6_weights;   /* Tensor #50 */
AI_STATIC ai_tensor quantize_conv_pw_5_bias;   /* Tensor #51 */
AI_STATIC ai_tensor quantize_conv_pw_5_weights;   /* Tensor #52 */
AI_STATIC ai_tensor quantize_conv_dw_5_bias;   /* Tensor #53 */
AI_STATIC ai_tensor quantize_conv_dw_5_weights;   /* Tensor #54 */
AI_STATIC ai_tensor quantize_conv_pw_4_bias;   /* Tensor #55 */
AI_STATIC ai_tensor quantize_conv_pw_4_weights;   /* Tensor #56 */
AI_STATIC ai_tensor quantize_conv_dw_4_bias;   /* Tensor #57 */
AI_STATIC ai_tensor quantize_conv_dw_4_weights;   /* Tensor #58 */
AI_STATIC ai_tensor quantize_conv_pw_3_bias;   /* Tensor #59 */
AI_STATIC ai_tensor quantize_conv_pw_3_weights;   /* Tensor #60 */
AI_STATIC ai_tensor quantize_conv_dw_3_bias;   /* Tensor #61 */
AI_STATIC ai_tensor quantize_conv_dw_3_weights;   /* Tensor #62 */
AI_STATIC ai_tensor quantize_conv_pw_2_bias;   /* Tensor #63 */
AI_STATIC ai_tensor quantize_conv_pw_2_weights;   /* Tensor #64 */
AI_STATIC ai_tensor quantize_conv_dw_2_bias;   /* Tensor #65 */
AI_STATIC ai_tensor quantize_conv_dw_2_weights;   /* Tensor #66 */
AI_STATIC ai_tensor quantize_conv_pw_1_bias;   /* Tensor #67 */
AI_STATIC ai_tensor quantize_conv_pw_1_weights;   /* Tensor #68 */
AI_STATIC ai_tensor quantize_conv_dw_1_bias;   /* Tensor #69 */
AI_STATIC ai_tensor quantize_conv_dw_1_weights;   /* Tensor #70 */
AI_STATIC ai_tensor quantize_conv1_bias;   /* Tensor #71 */
AI_STATIC ai_tensor quantize_conv1_weights;   /* Tensor #72 */
AI_STATIC ai_tensor quantize_input_2_output;   /* Tensor #73 */
AI_STATIC ai_tensor quantize_conv1_output;   /* Tensor #74 */
AI_STATIC ai_tensor quantize_conv_dw_1_output;   /* Tensor #75 */
AI_STATIC ai_tensor quantize_conv_pw_1_output;   /* Tensor #76 */
AI_STATIC ai_tensor quantize_conv_dw_2_output;   /* Tensor #77 */
AI_STATIC ai_tensor quantize_conv_pw_2_output;   /* Tensor #78 */
AI_STATIC ai_tensor quantize_conv_dw_3_output;   /* Tensor #79 */
AI_STATIC ai_tensor quantize_conv_pw_3_output;   /* Tensor #80 */
AI_STATIC ai_tensor quantize_conv_dw_4_output;   /* Tensor #81 */
AI_STATIC ai_tensor quantize_conv_pw_4_output;   /* Tensor #82 */
AI_STATIC ai_tensor quantize_conv_dw_5_output;   /* Tensor #83 */
AI_STATIC ai_tensor quantize_conv_pw_5_output;   /* Tensor #84 */
AI_STATIC ai_tensor quantize_conv_dw_6_output;   /* Tensor #85 */
AI_STATIC ai_tensor quantize_conv_pw_6_output;   /* Tensor #86 */
AI_STATIC ai_tensor quantize_conv_dw_7_output;   /* Tensor #87 */
AI_STATIC ai_tensor quantize_conv_pw_7_output;   /* Tensor #88 */
AI_STATIC ai_tensor quantize_conv_dw_8_output;   /* Tensor #89 */
AI_STATIC ai_tensor quantize_conv_pw_8_output;   /* Tensor #90 */
AI_STATIC ai_tensor quantize_conv_dw_9_output;   /* Tensor #91 */
AI_STATIC ai_tensor quantize_conv_pw_9_output;   /* Tensor #92 */
AI_STATIC ai_tensor quantize_conv_dw_10_output;   /* Tensor #93 */
AI_STATIC ai_tensor quantize_conv_pw_10_output;   /* Tensor #94 */
AI_STATIC ai_tensor quantize_conv_dw_11_output;   /* Tensor #95 */
AI_STATIC ai_tensor quantize_conv_pw_11_output;   /* Tensor #96 */
AI_STATIC ai_tensor quantize_conv_preds_output;   /* Tensor #97 */
AI_STATIC ai_tensor quantize_conv_preds_fmt_output;   /* Tensor #98 */
AI_STATIC ai_tensor act_softmax_output;   /* Tensor #99 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain quantize_conv1_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_1_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_1_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_2_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_2_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_3_chain;   /* Chain #5 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_3_chain;   /* Chain #6 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_4_chain;   /* Chain #7 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_4_chain;   /* Chain #8 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_5_chain;   /* Chain #9 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_5_chain;   /* Chain #10 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_6_chain;   /* Chain #11 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_6_chain;   /* Chain #12 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_7_chain;   /* Chain #13 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_7_chain;   /* Chain #14 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_8_chain;   /* Chain #15 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_8_chain;   /* Chain #16 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_9_chain;   /* Chain #17 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_9_chain;   /* Chain #18 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_10_chain;   /* Chain #19 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_10_chain;   /* Chain #20 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_dw_11_chain;   /* Chain #21 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_pw_11_chain;   /* Chain #22 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_preds_chain;   /* Chain #23 */
AI_STATIC_CONST ai_tensor_chain quantize_conv_preds_fmt_chain;   /* Chain #24 */
AI_STATIC_CONST ai_tensor_chain act_softmax_chain;   /* Chain #25 */


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_conv2d quantize_conv1_layer; /* Layer #0 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_1_layer; /* Layer #1 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_1_layer; /* Layer #2 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_2_layer; /* Layer #3 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_2_layer; /* Layer #4 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_3_layer; /* Layer #5 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_3_layer; /* Layer #6 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_4_layer; /* Layer #7 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_4_layer; /* Layer #8 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_5_layer; /* Layer #9 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_5_layer; /* Layer #10 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_6_layer; /* Layer #11 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_6_layer; /* Layer #12 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_7_layer; /* Layer #13 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_7_layer; /* Layer #14 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_8_layer; /* Layer #15 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_8_layer; /* Layer #16 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_9_layer; /* Layer #17 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_9_layer; /* Layer #18 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_10_layer; /* Layer #19 */
AI_STATIC ai_layer_conv2d quantize_conv_pw_10_layer; /* Layer #20 */
AI_STATIC ai_layer_conv2d quantize_conv_dw_11_layer; /* Layer #21 */
AI_STATIC ai_layer_conv2d_nl_pool quantize_conv_pw_11_layer; /* Layer #22 */
AI_STATIC ai_layer_conv2d quantize_conv_preds_layer; /* Layer #23 */
AI_STATIC ai_layer_nl quantize_conv_preds_fmt_layer; /* Layer #24 */
AI_STATIC ai_layer_nl act_softmax_layer; /* Layer #25 */


/**  Array declarations section  **********************************************/
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_preds_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 1024,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_11_scratch1_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 12544,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_11_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_11_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 3457,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_10_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_10_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 3457,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_9_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_9_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 3457,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_8_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_8_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 3457,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_7_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_7_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 3457,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_6_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 256,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_6_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 1729,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_5_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 256,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_5_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 1729,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_4_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_4_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 865,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_3_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_3_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 865,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_2_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_2_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 433,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_1_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_1_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 217,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv1_scratch0_array, AI_ARRAY_FORMAT_S8,
    NULL, NULL, 140,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_preds_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 18,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_preds_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 4608,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_11_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 256,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_11_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 32768,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_11_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_11_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1152,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_10_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_10_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_10_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_10_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1152,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_9_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_9_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_9_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_9_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1152,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_8_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_8_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_8_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_8_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1152,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_7_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_7_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 16384,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_7_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_7_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1152,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_6_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_6_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 8192,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_6_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_6_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 576,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_5_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_5_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 4096,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_5_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_5_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 576,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_4_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 64,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_4_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 2048,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_4_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_4_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 288,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_3_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_3_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 1024,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_3_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_3_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 288,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_2_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 32,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_2_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 512,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_2_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 16,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_2_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 144,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_1_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 16,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_1_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 128,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_1_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 8,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_1_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 72,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv1_bias_array, AI_ARRAY_FORMAT_S32,
    NULL, NULL, 8,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv1_weights_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 216,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_input_2_output_array, AI_ARRAY_FORMAT_U8|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 150528,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv1_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 100352,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_1_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 25088,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_1_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 50176,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_2_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 12544,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_2_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 25088,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_3_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 25088,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_3_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 25088,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_4_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_4_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 12544,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_5_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 12544,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_5_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 12544,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_6_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 3136,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_6_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_7_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_7_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_8_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_8_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_9_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_9_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_10_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_10_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_dw_11_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 6272,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_pw_11_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 256,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_preds_output_array, AI_ARRAY_FORMAT_U8,
    NULL, NULL, 18,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    quantize_conv_preds_fmt_output_array, AI_ARRAY_FORMAT_FLOAT,
    NULL, NULL, 18,
     AI_STATIC)
AI_ARRAY_OBJ_DECLARE(
    act_softmax_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
    NULL, NULL, 18,
     AI_STATIC)


AI_STATIC ai_intq_info_list quantize_conv_pw_11_scratch1_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.06917480019962087f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #0 */
AI_STATIC ai_intq_info_list quantize_conv_preds_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00018723314787981685f), AI_PACK_INTQ_ZP(0)));   /* Int quant #1 */
AI_STATIC ai_intq_info_list quantize_conv_preds_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.009781416724709904f), AI_PACK_UINTQ_ZP(149)));   /* Int quant #2 */
AI_STATIC ai_intq_info_list quantize_conv_pw_11_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0007486270238771819f), AI_PACK_INTQ_ZP(0)));   /* Int quant #3 */
AI_STATIC ai_intq_info_list quantize_conv_pw_11_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0050823321529463224f), AI_PACK_UINTQ_ZP(127)));   /* Int quant #4 */
AI_STATIC ai_intq_info_list quantize_conv_dw_11_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.000342264864688652f), AI_PACK_INTQ_ZP(0)));   /* Int quant #5 */
AI_STATIC ai_intq_info_list quantize_conv_dw_11_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0070540493609858495f), AI_PACK_UINTQ_ZP(104)));   /* Int quant #6 */
AI_STATIC ai_intq_info_list quantize_conv_pw_10_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.000367628158337646f), AI_PACK_INTQ_ZP(0)));   /* Int quant #7 */
AI_STATIC ai_intq_info_list quantize_conv_pw_10_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003648709549623377f), AI_PACK_UINTQ_ZP(136)));   /* Int quant #8 */
AI_STATIC ai_intq_info_list quantize_conv_dw_10_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0003988102225821614f), AI_PACK_INTQ_ZP(0)));   /* Int quant #9 */
AI_STATIC ai_intq_info_list quantize_conv_dw_10_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007860970029643938f), AI_PACK_UINTQ_ZP(129)));   /* Int quant #10 */
AI_STATIC ai_intq_info_list quantize_conv_pw_9_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00038425282307842423f), AI_PACK_INTQ_ZP(0)));   /* Int quant #11 */
AI_STATIC ai_intq_info_list quantize_conv_pw_9_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003779377306208891f), AI_PACK_UINTQ_ZP(128)));   /* Int quant #12 */
AI_STATIC ai_intq_info_list quantize_conv_dw_9_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00040090085733371155f), AI_PACK_INTQ_ZP(0)));   /* Int quant #13 */
AI_STATIC ai_intq_info_list quantize_conv_dw_9_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.009203201181748335f), AI_PACK_UINTQ_ZP(130)));   /* Int quant #14 */
AI_STATIC ai_intq_info_list quantize_conv_pw_8_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00036168844992939966f), AI_PACK_INTQ_ZP(0)));   /* Int quant #15 */
AI_STATIC ai_intq_info_list quantize_conv_pw_8_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004248616625280941f), AI_PACK_UINTQ_ZP(145)));   /* Int quant #16 */
AI_STATIC ai_intq_info_list quantize_conv_dw_8_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00037109557346228525f), AI_PACK_INTQ_ZP(0)));   /* Int quant #17 */
AI_STATIC ai_intq_info_list quantize_conv_dw_8_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.009086904338761873f), AI_PACK_UINTQ_ZP(117)));   /* Int quant #18 */
AI_STATIC ai_intq_info_list quantize_conv_pw_7_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00048069371142322163f), AI_PACK_INTQ_ZP(0)));   /* Int quant #19 */
AI_STATIC ai_intq_info_list quantize_conv_pw_7_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0053149405647726616f), AI_PACK_UINTQ_ZP(118)));   /* Int quant #20 */
AI_STATIC ai_intq_info_list quantize_conv_dw_7_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0004537492683455916f), AI_PACK_INTQ_ZP(0)));   /* Int quant #21 */
AI_STATIC ai_intq_info_list quantize_conv_dw_7_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.009372077268712661f), AI_PACK_UINTQ_ZP(117)));   /* Int quant #22 */
AI_STATIC ai_intq_info_list quantize_conv_pw_6_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0004972378319845428f), AI_PACK_INTQ_ZP(0)));   /* Int quant #23 */
AI_STATIC ai_intq_info_list quantize_conv_pw_6_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0038148716384289313f), AI_PACK_UINTQ_ZP(120)));   /* Int quant #24 */
AI_STATIC ai_intq_info_list quantize_conv_dw_6_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.000310288435370154f), AI_PACK_INTQ_ZP(0)));   /* Int quant #25 */
AI_STATIC ai_intq_info_list quantize_conv_dw_6_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006019722246656231f), AI_PACK_UINTQ_ZP(115)));   /* Int quant #26 */
AI_STATIC ai_intq_info_list quantize_conv_pw_5_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0006016379761348094f), AI_PACK_INTQ_ZP(0)));   /* Int quant #27 */
AI_STATIC ai_intq_info_list quantize_conv_pw_5_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00600513406828338f), AI_PACK_UINTQ_ZP(111)));   /* Int quant #28 */
AI_STATIC ai_intq_info_list quantize_conv_dw_5_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0004153574661426993f), AI_PACK_INTQ_ZP(0)));   /* Int quant #29 */
AI_STATIC ai_intq_info_list quantize_conv_dw_5_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008096037658990599f), AI_PACK_UINTQ_ZP(125)));   /* Int quant #30 */
AI_STATIC ai_intq_info_list quantize_conv_pw_4_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0012961421680124322f), AI_PACK_INTQ_ZP(0)));   /* Int quant #31 */
AI_STATIC ai_intq_info_list quantize_conv_pw_4_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007368923869787478f), AI_PACK_UINTQ_ZP(116)));   /* Int quant #32 */
AI_STATIC ai_intq_info_list quantize_conv_dw_4_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0005110000169024582f), AI_PACK_INTQ_ZP(0)));   /* Int quant #33 */
AI_STATIC ai_intq_info_list quantize_conv_dw_4_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00672833662407071f), AI_PACK_UINTQ_ZP(120)));   /* Int quant #34 */
AI_STATIC ai_intq_info_list quantize_conv_pw_3_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.001247229671783745f), AI_PACK_INTQ_ZP(0)));   /* Int quant #35 */
AI_STATIC ai_intq_info_list quantize_conv_pw_3_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007141224075766171f), AI_PACK_UINTQ_ZP(142)));   /* Int quant #36 */
AI_STATIC ai_intq_info_list quantize_conv_dw_3_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0009139555349285575f), AI_PACK_INTQ_ZP(0)));   /* Int quant #37 */
AI_STATIC ai_intq_info_list quantize_conv_dw_3_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.009301249186197917f), AI_PACK_UINTQ_ZP(131)));   /* Int quant #38 */
AI_STATIC ai_intq_info_list quantize_conv_pw_2_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0018250455203283756f), AI_PACK_INTQ_ZP(0)));   /* Int quant #39 */
AI_STATIC ai_intq_info_list quantize_conv_pw_2_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.022798767276838713f), AI_PACK_UINTQ_ZP(134)));   /* Int quant #40 */
AI_STATIC ai_intq_info_list quantize_conv_dw_2_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0002316667044240261f), AI_PACK_INTQ_ZP(0)));   /* Int quant #41 */
AI_STATIC ai_intq_info_list quantize_conv_dw_2_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005415800038506003f), AI_PACK_UINTQ_ZP(126)));   /* Int quant #42 */
AI_STATIC ai_intq_info_list quantize_conv_pw_1_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0008536075449810372f), AI_PACK_INTQ_ZP(0)));   /* Int quant #43 */
AI_STATIC ai_intq_info_list quantize_conv_pw_1_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0068471618727141735f), AI_PACK_UINTQ_ZP(125)));   /* Int quant #44 */
AI_STATIC ai_intq_info_list quantize_conv_dw_1_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00033000601085600385f), AI_PACK_INTQ_ZP(0)));   /* Int quant #45 */
AI_STATIC ai_intq_info_list quantize_conv_dw_1_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00546043784010644f), AI_PACK_UINTQ_ZP(134)));   /* Int quant #46 */
AI_STATIC ai_intq_info_list quantize_conv1_bias_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0001127437553420428f), AI_PACK_INTQ_ZP(0)));   /* Int quant #47 */
AI_STATIC ai_intq_info_list quantize_conv1_weights_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.028749657612220914f), AI_PACK_UINTQ_ZP(145)));   /* Int quant #48 */
AI_STATIC ai_intq_info_list quantize_input_2_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00392156862745098f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #49 */
AI_STATIC ai_intq_info_list quantize_conv1_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.06043581495098039f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #50 */
AI_STATIC ai_intq_info_list quantize_conv_dw_1_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.12466589235791974f), AI_PACK_UINTQ_ZP(102)));   /* Int quant #51 */
AI_STATIC ai_intq_info_list quantize_conv_pw_1_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.042776081608790976f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #52 */
AI_STATIC ai_intq_info_list quantize_conv_dw_2_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.08005018421247893f), AI_PACK_UINTQ_ZP(137)));   /* Int quant #53 */
AI_STATIC ai_intq_info_list quantize_conv_pw_2_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.09826159009746477f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #54 */
AI_STATIC ai_intq_info_list quantize_conv_dw_3_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.17465208464977788f), AI_PACK_UINTQ_ZP(151)));   /* Int quant #55 */
AI_STATIC ai_intq_info_list quantize_conv_pw_3_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07594745112400429f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #56 */
AI_STATIC ai_intq_info_list quantize_conv_dw_4_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.17589300567028568f), AI_PACK_UINTQ_ZP(120)));   /* Int quant #57 */
AI_STATIC ai_intq_info_list quantize_conv_pw_4_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.051303796207203586f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #58 */
AI_STATIC ai_intq_info_list quantize_conv_dw_5_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10018726797664866f), AI_PACK_UINTQ_ZP(115)));   /* Int quant #59 */
AI_STATIC ai_intq_info_list quantize_conv_pw_5_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.05154530768300973f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #60 */
AI_STATIC ai_intq_info_list quantize_conv_dw_6_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.13034195619470934f), AI_PACK_UINTQ_ZP(118)));   /* Int quant #61 */
AI_STATIC ai_intq_info_list quantize_conv_pw_6_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04841501572552849f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #62 */
AI_STATIC ai_intq_info_list quantize_conv_dw_7_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.09044197306913489f), AI_PACK_UINTQ_ZP(121)));   /* Int quant #63 */
AI_STATIC ai_intq_info_list quantize_conv_pw_7_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04083850337009804f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #64 */
AI_STATIC ai_intq_info_list quantize_conv_dw_8_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.08513087478338503f), AI_PACK_UINTQ_ZP(108)));   /* Int quant #65 */
AI_STATIC ai_intq_info_list quantize_conv_pw_8_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04356102288938036f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #66 */
AI_STATIC ai_intq_info_list quantize_conv_dw_9_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10167093463972503f), AI_PACK_UINTQ_ZP(137)));   /* Int quant #67 */
AI_STATIC ai_intq_info_list quantize_conv_pw_9_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.05073295294069776f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #68 */
AI_STATIC ai_intq_info_list quantize_conv_dw_10_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10075566534902536f), AI_PACK_UINTQ_ZP(135)));   /* Int quant #69 */
AI_STATIC ai_intq_info_list quantize_conv_pw_10_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.04852033876905254f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #70 */
AI_STATIC ai_intq_info_list quantize_conv_dw_11_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.14729990117690142f), AI_PACK_UINTQ_ZP(148)));   /* Int quant #71 */
AI_STATIC ai_intq_info_list quantize_conv_pw_11_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.019141720790489047f), AI_PACK_UINTQ_ZP(0)));   /* Int quant #72 */
AI_STATIC ai_intq_info_list quantize_conv_preds_output_intq = AI_INTQ_INFO_LIST_OBJ_INIT(
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_U8, 1, AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.19155238282446768f), AI_PACK_UINTQ_ZP(157)));   /* Int quant #73 */


/**  Tensor declarations section  *********************************************/
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_preds_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1024, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1024, 1024),
  1, &quantize_conv_preds_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_11_scratch1, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 7, 7), AI_STRIDE_INIT(4, 1, 1, 256, 1792),
  1, &quantize_conv_pw_11_scratch1_array, &quantize_conv_pw_11_scratch1_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_11_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &quantize_conv_pw_11_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_11_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &quantize_conv_dw_11_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_10_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &quantize_conv_pw_10_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_10_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &quantize_conv_dw_10_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_9_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &quantize_conv_pw_9_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_9_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &quantize_conv_dw_9_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_8_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &quantize_conv_pw_8_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_8_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &quantize_conv_dw_8_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_7_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 512, 1, 1), AI_STRIDE_INIT(4, 1, 1, 512, 512),
  1, &quantize_conv_pw_7_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_7_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3457, 1, 1), AI_STRIDE_INIT(4, 1, 1, 3457, 3457),
  1, &quantize_conv_dw_7_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_6_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 1, 1, 256, 256),
  1, &quantize_conv_pw_6_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_6_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1729, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1729, 1729),
  1, &quantize_conv_dw_6_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_5_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 1, 1, 256, 256),
  1, &quantize_conv_pw_5_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_5_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 1729, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1729, 1729),
  1, &quantize_conv_dw_5_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_4_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &quantize_conv_pw_4_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_4_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 865, 1, 1), AI_STRIDE_INIT(4, 1, 1, 865, 865),
  1, &quantize_conv_dw_4_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_3_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &quantize_conv_pw_3_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_3_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 865, 1, 1), AI_STRIDE_INIT(4, 1, 1, 865, 865),
  1, &quantize_conv_dw_3_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_2_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 1, 1, 64, 64),
  1, &quantize_conv_pw_2_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_2_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 433, 1, 1), AI_STRIDE_INIT(4, 1, 1, 433, 433),
  1, &quantize_conv_dw_2_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_1_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &quantize_conv_pw_1_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_1_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 217, 1, 1), AI_STRIDE_INIT(4, 1, 1, 217, 217),
  1, &quantize_conv_dw_1_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv1_scratch0, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 140, 1, 1), AI_STRIDE_INIT(4, 1, 1, 140, 140),
  1, &quantize_conv1_scratch0_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_preds_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 18, 1, 1), AI_STRIDE_INIT(4, 4, 4, 72, 72),
  1, &quantize_conv_preds_bias_array, &quantize_conv_preds_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_preds_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 256, 1, 1, 18), AI_STRIDE_INIT(4, 1, 256, 256, 256),
  1, &quantize_conv_preds_weights_array, &quantize_conv_preds_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_11_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1024, 1024),
  1, &quantize_conv_pw_11_bias_array, &quantize_conv_pw_11_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_11_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 1, 1, 256), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &quantize_conv_pw_11_weights_array, &quantize_conv_pw_11_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_11_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_dw_11_bias_array, &quantize_conv_dw_11_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_11_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 384, 1152),
  1, &quantize_conv_dw_11_weights_array, &quantize_conv_dw_11_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_10_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_pw_10_bias_array, &quantize_conv_pw_10_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_10_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &quantize_conv_pw_10_weights_array, &quantize_conv_pw_10_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_10_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_dw_10_bias_array, &quantize_conv_dw_10_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_10_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 384, 1152),
  1, &quantize_conv_dw_10_weights_array, &quantize_conv_dw_10_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_9_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_pw_9_bias_array, &quantize_conv_pw_9_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_9_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &quantize_conv_pw_9_weights_array, &quantize_conv_pw_9_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_9_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_dw_9_bias_array, &quantize_conv_dw_9_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_9_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 384, 1152),
  1, &quantize_conv_dw_9_weights_array, &quantize_conv_dw_9_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_8_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_pw_8_bias_array, &quantize_conv_pw_8_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_8_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &quantize_conv_pw_8_weights_array, &quantize_conv_pw_8_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_8_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_dw_8_bias_array, &quantize_conv_dw_8_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_8_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 384, 1152),
  1, &quantize_conv_dw_8_weights_array, &quantize_conv_dw_8_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_7_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_pw_7_bias_array, &quantize_conv_pw_7_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_7_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 1, 1, 128), AI_STRIDE_INIT(4, 1, 128, 128, 128),
  1, &quantize_conv_pw_7_weights_array, &quantize_conv_pw_7_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_7_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_dw_7_bias_array, &quantize_conv_dw_7_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_7_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 128, 3, 3, 1), AI_STRIDE_INIT(4, 1, 128, 384, 1152),
  1, &quantize_conv_dw_7_weights_array, &quantize_conv_dw_7_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_6_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &quantize_conv_pw_6_bias_array, &quantize_conv_pw_6_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_6_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 1, 1, 128), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &quantize_conv_pw_6_weights_array, &quantize_conv_pw_6_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_6_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &quantize_conv_dw_6_bias_array, &quantize_conv_dw_6_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_6_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 3, 3, 1), AI_STRIDE_INIT(4, 1, 64, 192, 576),
  1, &quantize_conv_dw_6_weights_array, &quantize_conv_dw_6_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_5_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &quantize_conv_pw_5_bias_array, &quantize_conv_pw_5_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_5_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 1, 1, 64), AI_STRIDE_INIT(4, 1, 64, 64, 64),
  1, &quantize_conv_pw_5_weights_array, &quantize_conv_pw_5_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_5_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &quantize_conv_dw_5_bias_array, &quantize_conv_dw_5_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_5_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 64, 3, 3, 1), AI_STRIDE_INIT(4, 1, 64, 192, 576),
  1, &quantize_conv_dw_5_weights_array, &quantize_conv_dw_5_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_4_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &quantize_conv_pw_4_bias_array, &quantize_conv_pw_4_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_4_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 1, 1, 64), AI_STRIDE_INIT(4, 1, 32, 32, 32),
  1, &quantize_conv_pw_4_weights_array, &quantize_conv_pw_4_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_4_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &quantize_conv_dw_4_bias_array, &quantize_conv_dw_4_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_4_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 3, 3, 1), AI_STRIDE_INIT(4, 1, 32, 96, 288),
  1, &quantize_conv_dw_4_weights_array, &quantize_conv_dw_4_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_3_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &quantize_conv_pw_3_bias_array, &quantize_conv_pw_3_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_3_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 1, 1, 32), AI_STRIDE_INIT(4, 1, 32, 32, 32),
  1, &quantize_conv_pw_3_weights_array, &quantize_conv_pw_3_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_3_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &quantize_conv_dw_3_bias_array, &quantize_conv_dw_3_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_3_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 32, 3, 3, 1), AI_STRIDE_INIT(4, 1, 32, 96, 288),
  1, &quantize_conv_dw_3_weights_array, &quantize_conv_dw_3_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_2_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &quantize_conv_pw_2_bias_array, &quantize_conv_pw_2_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_2_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 16, 1, 1, 32), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &quantize_conv_pw_2_weights_array, &quantize_conv_pw_2_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_2_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &quantize_conv_dw_2_bias_array, &quantize_conv_dw_2_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_2_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 16, 3, 3, 1), AI_STRIDE_INIT(4, 1, 16, 48, 144),
  1, &quantize_conv_dw_2_weights_array, &quantize_conv_dw_2_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &quantize_conv_pw_1_bias_array, &quantize_conv_pw_1_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_1_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 8, 1, 1, 16), AI_STRIDE_INIT(4, 1, 8, 8, 8),
  1, &quantize_conv_pw_1_weights_array, &quantize_conv_pw_1_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &quantize_conv_dw_1_bias_array, &quantize_conv_dw_1_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_1_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 8, 3, 3, 1), AI_STRIDE_INIT(4, 1, 8, 24, 72),
  1, &quantize_conv_dw_1_weights_array, &quantize_conv_dw_1_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv1_bias, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &quantize_conv1_bias_array, &quantize_conv1_bias_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv1_weights, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 3, 3, 3, 8), AI_STRIDE_INIT(4, 1, 3, 9, 27),
  1, &quantize_conv1_weights_array, &quantize_conv1_weights_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_input_2_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 3, 224, 224), AI_STRIDE_INIT(4, 1, 1, 3, 672),
  1, &quantize_input_2_output_array, &quantize_input_2_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 8, 112, 112), AI_STRIDE_INIT(4, 1, 1, 8, 896),
  1, &quantize_conv1_output_array, &quantize_conv1_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 8, 56, 56), AI_STRIDE_INIT(4, 1, 1, 8, 448),
  1, &quantize_conv_dw_1_output_array, &quantize_conv_dw_1_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_1_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 16, 56, 56), AI_STRIDE_INIT(4, 1, 1, 16, 896),
  1, &quantize_conv_pw_1_output_array, &quantize_conv_pw_1_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_2_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 16, 28, 28), AI_STRIDE_INIT(4, 1, 1, 16, 448),
  1, &quantize_conv_dw_2_output_array, &quantize_conv_dw_2_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_2_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 28, 28), AI_STRIDE_INIT(4, 1, 1, 32, 896),
  1, &quantize_conv_pw_2_output_array, &quantize_conv_pw_2_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_3_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 28, 28), AI_STRIDE_INIT(4, 1, 1, 32, 896),
  1, &quantize_conv_dw_3_output_array, &quantize_conv_dw_3_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_3_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 28, 28), AI_STRIDE_INIT(4, 1, 1, 32, 896),
  1, &quantize_conv_pw_3_output_array, &quantize_conv_pw_3_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_4_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 32, 14, 14), AI_STRIDE_INIT(4, 1, 1, 32, 448),
  1, &quantize_conv_dw_4_output_array, &quantize_conv_dw_4_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_4_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 1, 1, 64, 896),
  1, &quantize_conv_pw_4_output_array, &quantize_conv_pw_4_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_5_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 1, 1, 64, 896),
  1, &quantize_conv_dw_5_output_array, &quantize_conv_dw_5_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_5_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 14, 14), AI_STRIDE_INIT(4, 1, 1, 64, 896),
  1, &quantize_conv_pw_5_output_array, &quantize_conv_pw_5_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_6_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 64, 7, 7), AI_STRIDE_INIT(4, 1, 1, 64, 448),
  1, &quantize_conv_dw_6_output_array, &quantize_conv_dw_6_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_6_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_pw_6_output_array, &quantize_conv_pw_6_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_7_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_dw_7_output_array, &quantize_conv_dw_7_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_7_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_pw_7_output_array, &quantize_conv_pw_7_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_8_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_dw_8_output_array, &quantize_conv_dw_8_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_8_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_pw_8_output_array, &quantize_conv_pw_8_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_9_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_dw_9_output_array, &quantize_conv_dw_9_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_9_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_pw_9_output_array, &quantize_conv_pw_9_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_10_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_dw_10_output_array, &quantize_conv_dw_10_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_10_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_pw_10_output_array, &quantize_conv_pw_10_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_dw_11_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 128, 7, 7), AI_STRIDE_INIT(4, 1, 1, 128, 896),
  1, &quantize_conv_dw_11_output_array, &quantize_conv_dw_11_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_pw_11_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 256, 1, 1), AI_STRIDE_INIT(4, 1, 1, 256, 256),
  1, &quantize_conv_pw_11_output_array, &quantize_conv_pw_11_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_preds_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 18, 1, 1), AI_STRIDE_INIT(4, 1, 1, 18, 18),
  1, &quantize_conv_preds_output_array, &quantize_conv_preds_output_intq)
AI_TENSOR_OBJ_DECLARE(
  quantize_conv_preds_fmt_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 18, 1, 1), AI_STRIDE_INIT(4, 4, 4, 72, 72),
  1, &quantize_conv_preds_fmt_output_array, NULL)
AI_TENSOR_OBJ_DECLARE(
  act_softmax_output, AI_STATIC,
  0x0, 0x0, AI_SHAPE_INIT(4, 1, 18, 1, 1), AI_STRIDE_INIT(4, 4, 4, 72, 72),
  1, &act_softmax_output_array, NULL)


/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_input_2_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv1_weights, &quantize_conv1_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv1_layer, 1,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_1_layer, AI_STATIC,
  .tensors = &quantize_conv1_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_1_weights, &quantize_conv_dw_1_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_1_layer, 3,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_1_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_1_chain, 
  .groups = 8, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_1_weights, &quantize_conv_pw_1_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_1_layer, 4,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_2_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_1_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_1_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_2_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_2_weights, &quantize_conv_dw_2_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_2_layer, 6,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_2_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_2_chain, 
  .groups = 16, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_2_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_2_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_2_weights, &quantize_conv_pw_2_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_2_layer, 7,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_3_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_2_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_2_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_3_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_3_weights, &quantize_conv_dw_3_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_3_layer, 9,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_3_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_3_chain, 
  .groups = 32, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_3_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_3_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_3_weights, &quantize_conv_pw_3_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_3_layer, 10,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_4_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_3_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_3_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_4_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_4_weights, &quantize_conv_dw_4_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_4_layer, 12,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_4_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_4_chain, 
  .groups = 32, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_4_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_4_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_4_weights, &quantize_conv_pw_4_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_4_layer, 13,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_5_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_4_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_4_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_5_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_5_weights, &quantize_conv_dw_5_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_5_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_5_layer, 15,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_5_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_5_chain, 
  .groups = 64, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_5_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_5_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_5_weights, &quantize_conv_pw_5_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_5_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_5_layer, 16,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_6_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_5_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_5_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_6_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_6_weights, &quantize_conv_dw_6_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_6_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_6_layer, 18,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_6_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_6_chain, 
  .groups = 64, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_6_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_6_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_6_weights, &quantize_conv_pw_6_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_6_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_6_layer, 19,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_7_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_6_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_6_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_7_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_7_weights, &quantize_conv_dw_7_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_7_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_7_layer, 21,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_7_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_7_chain, 
  .groups = 128, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_7_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_7_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_7_weights, &quantize_conv_pw_7_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_7_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_7_layer, 22,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_8_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_7_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_7_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_8_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_8_weights, &quantize_conv_dw_8_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_8_layer, 24,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_8_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_8_chain, 
  .groups = 128, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_8_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_8_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_8_weights, &quantize_conv_pw_8_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_8_layer, 25,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_9_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_8_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_9_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_8_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_9_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_9_weights, &quantize_conv_dw_9_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_9_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_9_layer, 27,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_9_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_9_chain, 
  .groups = 128, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_9_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_9_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_9_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_9_weights, &quantize_conv_pw_9_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_9_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_9_layer, 28,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_10_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_9_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_9_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_10_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_10_weights, &quantize_conv_dw_10_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_10_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_10_layer, 30,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_10_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_10_chain, 
  .groups = 128, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_10_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_10_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_10_weights, &quantize_conv_pw_10_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_10_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_10_layer, 31,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_dw_11_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_10_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_dw_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_10_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_11_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_11_weights, &quantize_conv_dw_11_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_11_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_dw_11_layer, 33,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_pw_11_layer, AI_STATIC,
  .tensors = &quantize_conv_dw_11_chain, 
  .groups = 128, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_pw_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_dw_11_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_11_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_11_weights, &quantize_conv_pw_11_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_11_scratch0, &quantize_conv_pw_11_scratch1)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_pw_11_layer, 34,
  OPTIMIZED_CONV2D_TYPE,
  conv2d_nl_pool, forward_conv2d_nl_pool_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_preds_layer, AI_STATIC,
  .tensors = &quantize_conv_pw_11_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_size = AI_SHAPE_2D_INIT(7, 7), 
  .pool_stride = AI_SHAPE_2D_INIT(7, 7), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .pool_func = pool_func_ap_array_integer, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_preds_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_pw_11_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_weights, &quantize_conv_preds_bias, NULL),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_preds_layer, 39,
  CONV2D_TYPE,
  conv2d, forward_conv2d_integer,
  &AI_NET_OBJ_INSTANCE, &quantize_conv_preds_fmt_layer, AI_STATIC,
  .tensors = &quantize_conv_preds_chain, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  quantize_conv_preds_fmt_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_output),
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_fmt_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  quantize_conv_preds_fmt_layer, 39,
  NL_TYPE,
  nl, node_convert,
  &AI_NET_OBJ_INSTANCE, &act_softmax_layer, AI_STATIC,
  .tensors = &quantize_conv_preds_fmt_chain, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  act_softmax_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_ENTRY(&quantize_conv_preds_fmt_output),
  AI_TENSOR_LIST_ENTRY(&act_softmax_output),
  AI_TENSOR_LIST_EMPTY,
  AI_TENSOR_LIST_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  act_softmax_layer, 40,
  NL_TYPE,
  nl, forward_sm,
  &AI_NET_OBJ_INSTANCE, &act_softmax_layer, AI_STATIC,
  .tensors = &act_softmax_chain, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 134776, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 167436, 1,
                     NULL),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &quantize_input_2_output),
  AI_TENSOR_LIST_IO_ENTRY(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &act_softmax_output),
  &quantize_conv1_layer, 0, NULL)



AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT(activations)
  AI_UNUSED(net_ctx)

  {
    /* Updating activations (byte) offsets */
    quantize_conv_preds_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_preds_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_11_scratch1_array.data = AI_PTR(activations + 151432);
    quantize_conv_pw_11_scratch1_array.data_start = AI_PTR(activations + 151432);
    quantize_conv_pw_11_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_11_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_11_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_11_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_10_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_10_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_10_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_10_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_9_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_9_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_9_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_9_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_8_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_8_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_8_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_8_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_7_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_7_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_7_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_7_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_6_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_6_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_6_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_6_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_5_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_5_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_5_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_5_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_4_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_4_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_4_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_4_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_3_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_3_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_3_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_3_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_2_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_2_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_2_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_2_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_pw_1_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_pw_1_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv_dw_1_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv_dw_1_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_conv1_scratch0_array.data = AI_PTR(activations + 163976);
    quantize_conv1_scratch0_array.data_start = AI_PTR(activations + 163976);
    quantize_input_2_output_array.data = AI_PTR(activations + 904);
    quantize_input_2_output_array.data_start = AI_PTR(activations + 904);
    quantize_conv1_output_array.data = AI_PTR(activations + 0);
    quantize_conv1_output_array.data_start = AI_PTR(activations + 0);
    quantize_conv_dw_1_output_array.data = AI_PTR(activations + 126344);
    quantize_conv_dw_1_output_array.data_start = AI_PTR(activations + 126344);
    quantize_conv_pw_1_output_array.data = AI_PTR(activations + 100344);
    quantize_conv_pw_1_output_array.data_start = AI_PTR(activations + 100344);
    quantize_conv_dw_2_output_array.data = AI_PTR(activations + 99880);
    quantize_conv_dw_2_output_array.data_start = AI_PTR(activations + 99880);
    quantize_conv_pw_2_output_array.data = AI_PTR(activations + 86408);
    quantize_conv_pw_2_output_array.data_start = AI_PTR(activations + 86408);
    quantize_conv_dw_3_output_array.data = AI_PTR(activations + 84552);
    quantize_conv_dw_3_output_array.data_start = AI_PTR(activations + 84552);
    quantize_conv_pw_3_output_array.data = AI_PTR(activations + 83624);
    quantize_conv_pw_3_output_array.data_start = AI_PTR(activations + 83624);
    quantize_conv_dw_4_output_array.data = AI_PTR(activations + 83144);
    quantize_conv_dw_4_output_array.data_start = AI_PTR(activations + 83144);
    quantize_conv_pw_4_output_array.data = AI_PTR(activations + 75912);
    quantize_conv_pw_4_output_array.data_start = AI_PTR(activations + 75912);
    quantize_conv_dw_5_output_array.data = AI_PTR(activations + 73992);
    quantize_conv_dw_5_output_array.data_start = AI_PTR(activations + 73992);
    quantize_conv_pw_5_output_array.data = AI_PTR(activations + 73032);
    quantize_conv_pw_5_output_array.data_start = AI_PTR(activations + 73032);
    quantize_conv_dw_6_output_array.data = AI_PTR(activations + 72520);
    quantize_conv_dw_6_output_array.data_start = AI_PTR(activations + 72520);
    quantize_conv_pw_6_output_array.data = AI_PTR(activations + 68360);
    quantize_conv_pw_6_output_array.data_start = AI_PTR(activations + 68360);
    quantize_conv_dw_7_output_array.data = AI_PTR(activations + 66312);
    quantize_conv_dw_7_output_array.data_start = AI_PTR(activations + 66312);
    quantize_conv_pw_7_output_array.data = AI_PTR(activations + 65288);
    quantize_conv_pw_7_output_array.data_start = AI_PTR(activations + 65288);
    quantize_conv_dw_8_output_array.data = AI_PTR(activations + 63240);
    quantize_conv_dw_8_output_array.data_start = AI_PTR(activations + 63240);
    quantize_conv_pw_8_output_array.data = AI_PTR(activations + 62216);
    quantize_conv_pw_8_output_array.data_start = AI_PTR(activations + 62216);
    quantize_conv_dw_9_output_array.data = AI_PTR(activations + 60168);
    quantize_conv_dw_9_output_array.data_start = AI_PTR(activations + 60168);
    quantize_conv_pw_9_output_array.data = AI_PTR(activations + 59144);
    quantize_conv_pw_9_output_array.data_start = AI_PTR(activations + 59144);
    quantize_conv_dw_10_output_array.data = AI_PTR(activations + 57096);
    quantize_conv_dw_10_output_array.data_start = AI_PTR(activations + 57096);
    quantize_conv_pw_10_output_array.data = AI_PTR(activations + 56072);
    quantize_conv_pw_10_output_array.data_start = AI_PTR(activations + 56072);
    quantize_conv_dw_11_output_array.data = AI_PTR(activations + 54024);
    quantize_conv_dw_11_output_array.data_start = AI_PTR(activations + 54024);
    quantize_conv_pw_11_output_array.data = AI_PTR(activations + 53768);
    quantize_conv_pw_11_output_array.data_start = AI_PTR(activations + 53768);
    quantize_conv_preds_output_array.data = AI_PTR(activations + 53748);
    quantize_conv_preds_output_array.data_start = AI_PTR(activations + 53748);
    quantize_conv_preds_fmt_output_array.data = AI_PTR(activations + 53640);
    quantize_conv_preds_fmt_output_array.data_start = AI_PTR(activations + 53640);
    act_softmax_output_array.data = AI_PTR(NULL);
    act_softmax_output_array.data_start = AI_PTR(NULL);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT(weights)
  AI_UNUSED(net_ctx)

  {
    /* Updating weights (byte) offsets */
    
    quantize_conv_preds_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_preds_bias_array.data = AI_PTR(weights + 134704);
    quantize_conv_preds_bias_array.data_start = AI_PTR(weights + 134704);
    quantize_conv_preds_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_preds_weights_array.data = AI_PTR(weights + 130096);
    quantize_conv_preds_weights_array.data_start = AI_PTR(weights + 130096);
    quantize_conv_pw_11_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_11_bias_array.data = AI_PTR(weights + 129072);
    quantize_conv_pw_11_bias_array.data_start = AI_PTR(weights + 129072);
    quantize_conv_pw_11_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_11_weights_array.data = AI_PTR(weights + 96304);
    quantize_conv_pw_11_weights_array.data_start = AI_PTR(weights + 96304);
    quantize_conv_dw_11_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_11_bias_array.data = AI_PTR(weights + 95792);
    quantize_conv_dw_11_bias_array.data_start = AI_PTR(weights + 95792);
    quantize_conv_dw_11_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_11_weights_array.data = AI_PTR(weights + 94640);
    quantize_conv_dw_11_weights_array.data_start = AI_PTR(weights + 94640);
    quantize_conv_pw_10_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_10_bias_array.data = AI_PTR(weights + 94128);
    quantize_conv_pw_10_bias_array.data_start = AI_PTR(weights + 94128);
    quantize_conv_pw_10_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_10_weights_array.data = AI_PTR(weights + 77744);
    quantize_conv_pw_10_weights_array.data_start = AI_PTR(weights + 77744);
    quantize_conv_dw_10_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_10_bias_array.data = AI_PTR(weights + 77232);
    quantize_conv_dw_10_bias_array.data_start = AI_PTR(weights + 77232);
    quantize_conv_dw_10_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_10_weights_array.data = AI_PTR(weights + 76080);
    quantize_conv_dw_10_weights_array.data_start = AI_PTR(weights + 76080);
    quantize_conv_pw_9_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_9_bias_array.data = AI_PTR(weights + 75568);
    quantize_conv_pw_9_bias_array.data_start = AI_PTR(weights + 75568);
    quantize_conv_pw_9_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_9_weights_array.data = AI_PTR(weights + 59184);
    quantize_conv_pw_9_weights_array.data_start = AI_PTR(weights + 59184);
    quantize_conv_dw_9_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_9_bias_array.data = AI_PTR(weights + 58672);
    quantize_conv_dw_9_bias_array.data_start = AI_PTR(weights + 58672);
    quantize_conv_dw_9_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_9_weights_array.data = AI_PTR(weights + 57520);
    quantize_conv_dw_9_weights_array.data_start = AI_PTR(weights + 57520);
    quantize_conv_pw_8_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_8_bias_array.data = AI_PTR(weights + 57008);
    quantize_conv_pw_8_bias_array.data_start = AI_PTR(weights + 57008);
    quantize_conv_pw_8_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_8_weights_array.data = AI_PTR(weights + 40624);
    quantize_conv_pw_8_weights_array.data_start = AI_PTR(weights + 40624);
    quantize_conv_dw_8_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_8_bias_array.data = AI_PTR(weights + 40112);
    quantize_conv_dw_8_bias_array.data_start = AI_PTR(weights + 40112);
    quantize_conv_dw_8_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_8_weights_array.data = AI_PTR(weights + 38960);
    quantize_conv_dw_8_weights_array.data_start = AI_PTR(weights + 38960);
    quantize_conv_pw_7_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_7_bias_array.data = AI_PTR(weights + 38448);
    quantize_conv_pw_7_bias_array.data_start = AI_PTR(weights + 38448);
    quantize_conv_pw_7_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_7_weights_array.data = AI_PTR(weights + 22064);
    quantize_conv_pw_7_weights_array.data_start = AI_PTR(weights + 22064);
    quantize_conv_dw_7_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_7_bias_array.data = AI_PTR(weights + 21552);
    quantize_conv_dw_7_bias_array.data_start = AI_PTR(weights + 21552);
    quantize_conv_dw_7_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_7_weights_array.data = AI_PTR(weights + 20400);
    quantize_conv_dw_7_weights_array.data_start = AI_PTR(weights + 20400);
    quantize_conv_pw_6_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_6_bias_array.data = AI_PTR(weights + 19888);
    quantize_conv_pw_6_bias_array.data_start = AI_PTR(weights + 19888);
    quantize_conv_pw_6_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_6_weights_array.data = AI_PTR(weights + 11696);
    quantize_conv_pw_6_weights_array.data_start = AI_PTR(weights + 11696);
    quantize_conv_dw_6_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_6_bias_array.data = AI_PTR(weights + 11440);
    quantize_conv_dw_6_bias_array.data_start = AI_PTR(weights + 11440);
    quantize_conv_dw_6_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_6_weights_array.data = AI_PTR(weights + 10864);
    quantize_conv_dw_6_weights_array.data_start = AI_PTR(weights + 10864);
    quantize_conv_pw_5_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_5_bias_array.data = AI_PTR(weights + 10608);
    quantize_conv_pw_5_bias_array.data_start = AI_PTR(weights + 10608);
    quantize_conv_pw_5_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_5_weights_array.data = AI_PTR(weights + 6512);
    quantize_conv_pw_5_weights_array.data_start = AI_PTR(weights + 6512);
    quantize_conv_dw_5_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_5_bias_array.data = AI_PTR(weights + 6256);
    quantize_conv_dw_5_bias_array.data_start = AI_PTR(weights + 6256);
    quantize_conv_dw_5_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_5_weights_array.data = AI_PTR(weights + 5680);
    quantize_conv_dw_5_weights_array.data_start = AI_PTR(weights + 5680);
    quantize_conv_pw_4_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_4_bias_array.data = AI_PTR(weights + 5424);
    quantize_conv_pw_4_bias_array.data_start = AI_PTR(weights + 5424);
    quantize_conv_pw_4_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_4_weights_array.data = AI_PTR(weights + 3376);
    quantize_conv_pw_4_weights_array.data_start = AI_PTR(weights + 3376);
    quantize_conv_dw_4_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_4_bias_array.data = AI_PTR(weights + 3248);
    quantize_conv_dw_4_bias_array.data_start = AI_PTR(weights + 3248);
    quantize_conv_dw_4_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_4_weights_array.data = AI_PTR(weights + 2960);
    quantize_conv_dw_4_weights_array.data_start = AI_PTR(weights + 2960);
    quantize_conv_pw_3_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_3_bias_array.data = AI_PTR(weights + 2832);
    quantize_conv_pw_3_bias_array.data_start = AI_PTR(weights + 2832);
    quantize_conv_pw_3_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_3_weights_array.data = AI_PTR(weights + 1808);
    quantize_conv_pw_3_weights_array.data_start = AI_PTR(weights + 1808);
    quantize_conv_dw_3_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_3_bias_array.data = AI_PTR(weights + 1680);
    quantize_conv_dw_3_bias_array.data_start = AI_PTR(weights + 1680);
    quantize_conv_dw_3_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_3_weights_array.data = AI_PTR(weights + 1392);
    quantize_conv_dw_3_weights_array.data_start = AI_PTR(weights + 1392);
    quantize_conv_pw_2_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_2_bias_array.data = AI_PTR(weights + 1264);
    quantize_conv_pw_2_bias_array.data_start = AI_PTR(weights + 1264);
    quantize_conv_pw_2_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_2_weights_array.data = AI_PTR(weights + 752);
    quantize_conv_pw_2_weights_array.data_start = AI_PTR(weights + 752);
    quantize_conv_dw_2_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_2_bias_array.data = AI_PTR(weights + 688);
    quantize_conv_dw_2_bias_array.data_start = AI_PTR(weights + 688);
    quantize_conv_dw_2_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_2_weights_array.data = AI_PTR(weights + 544);
    quantize_conv_dw_2_weights_array.data_start = AI_PTR(weights + 544);
    quantize_conv_pw_1_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_1_bias_array.data = AI_PTR(weights + 480);
    quantize_conv_pw_1_bias_array.data_start = AI_PTR(weights + 480);
    quantize_conv_pw_1_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_pw_1_weights_array.data = AI_PTR(weights + 352);
    quantize_conv_pw_1_weights_array.data_start = AI_PTR(weights + 352);
    quantize_conv_dw_1_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_1_bias_array.data = AI_PTR(weights + 320);
    quantize_conv_dw_1_bias_array.data_start = AI_PTR(weights + 320);
    quantize_conv_dw_1_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv_dw_1_weights_array.data = AI_PTR(weights + 248);
    quantize_conv_dw_1_weights_array.data_start = AI_PTR(weights + 248);
    quantize_conv1_bias_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv1_bias_array.data = AI_PTR(weights + 216);
    quantize_conv1_bias_array.data_start = AI_PTR(weights + 216);
    quantize_conv1_weights_array.format |= AI_FMT_FLAG_CONST;
    quantize_conv1_weights_array.data = AI_PTR(weights + 0);
    quantize_conv1_weights_array.data_start = AI_PTR(weights + 0);
  }

  return true;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = {AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR,
                            AI_TOOLS_API_VERSION_MICRO, 0x0},

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 11801396,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .activations       = AI_STRUCT_INIT,
      .params            = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if ( !ai_platform_api_get_network_report(network, &r) ) return false;

    *report = r;
    return true;
  }

  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, &params->params);
  ok &= network_configure_activations(net_ctx, &params->activations);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}

#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_VERSION_MAJOR
#undef AI_TOOLS_VERSION_MINOR
#undef AI_TOOLS_VERSION_MICRO
#undef AI_TOOLS_API_VERSION_MAJOR
#undef AI_TOOLS_API_VERSION_MINOR
#undef AI_TOOLS_API_VERSION_MICRO
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

