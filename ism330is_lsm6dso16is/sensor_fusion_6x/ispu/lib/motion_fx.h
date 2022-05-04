/**
  ******************************************************************************
  * @file    motion_fx.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_fx module
  ******************************************************************************
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
  ********************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_FX_H_
#define _MOTION_FX_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_FX MOTION_FX
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MFX_ENGINE_DISABLE = 0,
  MFX_ENGINE_ENABLE = 1
} MFX_engine_state_t;

typedef enum
{
  MFX_6X = 0,
  MFX_9X = 1
} MFX_mode_t;

typedef struct
{
  float mag[3];   /* Calibrated mag [uT/50] */
  float acc[3];   /* Acceleration in [g] */
  float gyro[3];  /* Angular rate [dps] */
} MFX_input_t;

typedef struct
{
  float quaternion[4];           /* Quaternion [-] */
  float gravity[3];              /* Device frame gravity [g] */
  float linear_acceleration[3];  /* Device frame linear acceleration [g] */
  float rotation[3];             /* Yaw, pitch and roll [deg] */
} MFX_output_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_FX_Exported_Functions MOTION_FX_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFX engine
 * @param  mode algorithm mode to set
 * @retval algorithm instance
 */
void *MotionFX_initialize(MFX_mode_t mode);

/**
 * @brief  Deinitialize the MotionFX engine
 * @param  mfx algorithm instance
 * @retval none
 */
void MotionFX_deinitialize(void *mfx);

/**
 * @brief  Set sensor orientation, default orientation is ENU (x - east, y - north, z - up)
 * @param  mfx algorithm instance
 * @param  acc_orientation string with reference to set
 * @param  gyr_orientation string with reference to set
 * @param  mag_orientation string with reference to set
 * @retval none
 */
void MotionFX_setOrientation(void *mfx, const char acc_orientation[4], const char gyro_orientation[4], const char mag_orientation[4]);

/**
 * @brief  Get the status of the euler angles calculation
 * @param  mfx algorithm instance
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_getStatus_euler(void *mfx);

/**
 * @brief  Get the status of the gyroscope calibration
 * @param  mfx algorithm instance
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_getStatus_gbias(void *mfx);

/**
 * @brief  Enable or disable euler angles calculation
 * @param  mfx algorithm instance
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_euler(void *mfx, MFX_engine_state_t enable);

/**
 * @brief  Enable or disable gyroscope calibration
 * @param  mfx algorithm instance
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_gbias(void *mfx, MFX_engine_state_t enable);

/**
 * @brief  Set the initial gbias
 * @param  mfx algorithm instance
 * @param  gbias pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_setGbias(void *mfx, float *gbias);

/**
 * @brief  Get the initial gbias
 * @param  mfx algorithm instance
 * @param  pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_getGbias(void *mfx, float *gbias);

/**
 * @brief  This function runs one step of the sensor fusion algorithm
 * @param  mfx algorithm instance
 * @param  data_out pointer to the MFX_output structure
 * @param  data_in pointer to the MFX_input structure
 * @param  deltatime delta time between two calls [s]
 * @retval none
 */
void MotionFX_update(void *mfx, MFX_output_t *data_out, MFX_input_t *data_in, float deltatime);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval Number of characters in the version string
  */
uint8_t MotionFX_GetLibVersion(char *version);

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

#endif /* _MOTION_FX_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
