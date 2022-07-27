/**
 *******************************************************************************
 * @file    motion_pm.h
 * @author  MEMS Application Team
 * @version V1.0.0
 * @brief   Header for motion_pm module
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2022 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef _MOTION_PM_H_
#define _MOTION_PM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_PM MOTION_PM
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef struct
{
  float acc[3];  /* Accelerometer sensor output [g] */
} MPM_input_t;

typedef struct
{
  float taps_coeff[5];      /* filter coefficients */
  uint8_t dectree_en;       /* decision tree disable (0) or enable (1) */
  uint16_t debounce_steps;  /* debounce steps */
  float ths_min;            /* minimum ths for step counter */
  float coeff_peak_2_peak;  /* peak2peak coefficient */
  float max_coeff_up;       /* ths increase coefficient */
  float max_coeff_dn;       /* ths decrease coefficient */
} MPM_conf_t;

typedef struct
{
  uint16_t steps;  /* Steps [#] */
} MPM_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_PM_Exported_Functions MOTION_PM_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionPM engine
 * @retval none
 */
void MotionPM_initialize(void);

/**
 * @brief  Get the algorithm configuration.
 * @param  conf pointer to the structure where the current configuration will be written to
 * @retval none
 */
void MotionPM_get_configuration(MPM_conf_t *conf);

/**
 * @brief  Set the algorithm configuration.
 * @param  conf pointer to the structure containing the configuration to be set
 * @retval none
 */
void MotionPM_set_configuration(MPM_conf_t *conf);

/**
 * @brief  Run pedometer algorithm (to be called at 26 Hz pace).
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @retval none
 */
void MotionPM_update(MPM_output_t *data_out, MPM_input_t *data_in);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionPM_get_lib_version(char *version);

/**
 *  @}
 */

/**
 *  @}
 */

/**
 *  @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _MOTION_PM_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

