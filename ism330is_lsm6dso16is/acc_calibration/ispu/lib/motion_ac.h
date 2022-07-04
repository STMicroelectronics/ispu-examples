/**
  ******************************************************************************
  * @file    motion_ac.h
  * @author  MEMS Application Team
  * @version V2.6.0
  * @date    21-April-2021
  * @brief   Header for motion_ac module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2021 STMicroelectronics</center></h2>
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
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_AC_H_
#define _MOTION_AC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AC MOTION_AC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_AC_Exported_Types MOTION_AC_Exported_Types
  * @{
  */

typedef enum
{
  MAC_CALQSTATUSUNKNOWN = 0,
  MAC_CALQSTATUSPOOR,
  MAC_CALQSTATUSOK,
  MAC_CALQSTATUSGOOD
} MAC_cal_quality_t;

typedef struct
{
  float Acc[3];   /* Acceleration in X, Y, Z axis in [g] */
  int TimeStamp;  /* Time stamp for accelerometer sensor output in [ms] */
} MAC_input_t;

typedef struct
{
  float AccBias[3];              /* Accelerometer offset value in [g] */
  float SF_Matrix[3][3];         /* Scale factor correction matrix (diagonal matrix) */
  MAC_cal_quality_t CalQuality;  /* Quality factor */
} MAC_output_t;

typedef struct
{
  uint32_t Sample_ms;    /* Accelerometer sensor sample period in ms */
  float MoveThresh_g;    /* Recommended between 0.15 - 0.30g, higher value will relax condition on data selection
                            for calibration but reduce the accuracy which will be around (moveThresh_g / 10) */
} MAC_knobs_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AC_Exported_Functions MOTION_AC_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionAC engine
  * @param  enable  enable or disable the library
  * @retval None
  */
void MotionAC_Initialize(uint8_t enable);

/**
  * @brief  Run accelerometer calibration algorithm
  * @param  data_in  pointer to acceleration [g] and timestamp values [ms]
  * @param  is_calibrated  pointer value returns 1 if calibration is done with current sample, 0 otherwise
  * @retval None
  */
void MotionAC_Update(MAC_input_t *data_in, uint8_t *is_calibrated);

/**
  * @brief  Get the accelerometer compensation parameters
  * @param  data_out  pointer to actual accelerometer offset [g], scale factor correction matrix [-] and quality factor
  * @retval None
  */
void MotionAC_GetCalParams(MAC_output_t *data_out);

/**
  * @brief  Get current knobs settings
  * @param  knobs  pointer to current knobs settings
  * @retval None
  */
void MotionAC_GetKnobs(MAC_knobs_t *knobs);

/**
  * @brief  Set new knobs settings
  * @param  knobs  pointer to new knobs settings
  * @retval 1 if success => library is running, 0 otherwise
  */
uint8_t MotionAC_SetKnobs(MAC_knobs_t *knobs);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionAC_GetLibVersion(char *version);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _MOTION_AC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
