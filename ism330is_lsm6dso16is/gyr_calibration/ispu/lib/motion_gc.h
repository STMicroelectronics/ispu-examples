/**
  ******************************************************************************
  * @file    motion_gc.h
  * @author  MEMS Application Team
  * @version V2.5.1
  * @date    10-August-2021
  * @brief   Header for motion_gc module
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
#ifndef _MOTION_GC_H_
#define _MOTION_GC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_GC MOTION_GC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup MOTION_GC_Exported_Types MOTION_GC_Exported_Types
  * @{
  */

typedef enum
{
  MGC_MCU_STM32 = 0,
  MGC_MCU_BLUE_NRG1,
  MGC_MCU_BLUE_NRG2,
  MGC_MCU_BLUE_NRG_LP,
  MGC_MCU_ISPU,
} MGC_mcu_type_t;

typedef struct
{
  float Acc[3];         /* Acceleration in X, Y, Z axis in [g] */
  float Gyro[3];        /* Angular rate along X, Y, Z axis in [dps] */
} MGC_input_t;

typedef struct
{
  float GyroBiasX;     /* Gyroscope offset value in X axis in [dps] */
  float GyroBiasY;     /* Gyroscope offset value in Y axis in [dps] */
  float GyroBiasZ;     /* Gyroscope offset value in Z axis in [dps] */
} MGC_output_t;


typedef struct
{
  float AccThr;         /* Accelerometer threshold to detect steady state in [g] in range 0.003-0.05 g,
                           default value 0.004 g */
  float GyroThr;        /* Gyroscope threshold to detect steady state in [dps] in range 0.008-0.4 dps,
                           default value 0.1 dps */
  float FilterConst;    /* Constant for internal filter [0..1], default value 0.002 */
  int   FastStart;      /* Set to 1 for fast convergence at the initialization, default value 1 */
  float MaxGyro;        /* Maximum expected angular rate offset when still in [dps], default value 15 dps */
  float MaxAcc;         /* Maximum acceleration module when still in [g], default value 1.3g */
} MGC_knobs_t;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_GC_Exported_Functions MOTION_GC_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize the MotionGC engine
  * @param  mcu_type MCU type
  * @param  freq  sampling frequency
  * @retval none
  */
void MotionGC_Initialize(MGC_mcu_type_t mcu_type, float *freq);

/**
  * @brief  Get the knobs setting of the library
  * @param  knobs  pointer to knobs setting structure
  * @retval none
  */
void MotionGC_GetKnobs(MGC_knobs_t *knobs);

/**
  * @brief  Set the knobs setting of the library
  * @param  knobs  pointer to knobs setting structure
  * @retval none
  */
void MotionGC_SetKnobs(MGC_knobs_t *knobs);

/**
  * @brief  Run gyroscope calibration algorithm and return compensation parameters
  * @param  data_in  pointer to acceleration [g] and angular rate values [dps]
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @param  bias_update  pointer to an integer that is set to 1 if the gyroscope bias was updated, 0 otherwise
  * @retval none
  */
void MotionGC_Update(MGC_input_t *data_in, MGC_output_t *gyro_bias, uint8_t *bias_update);

/**
  * @brief  Get the gyroscope compensation parameters
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @retval none
  */
void MotionGC_GetCalParams(MGC_output_t *gyro_bias);

/**
  * @brief  Set the initial gyroscope compensation parameters
  * @param  gyro_bias  pointer to actual gyroscope offset value in [dps]
  * @retval none
  */
void MotionGC_SetCalParams(MGC_output_t *gyro_bias);

/**
  * @brief  Set new sample frequency
  * @param  freq  new sample frequency in [Hz]
  * @retval none
  */
void MotionGC_SetFrequency(float *freq);


/**
  * @brief  Get the library version
  * @param  version  pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionGC_GetLibVersion(char *version);

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

#endif /* _MOTION_GC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
