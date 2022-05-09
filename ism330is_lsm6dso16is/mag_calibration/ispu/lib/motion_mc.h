/**
 *******************************************************************************
 * @file    motion_mc.h
 * @author  MEMS Application Team
 * @version V1.0.0
 * @brief   Header for motion_mc module
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
#ifndef _MOTION_MC_H_
#define _MOTION_MC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_MC MOTION_MC
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef enum
{
  MMC_HI_ONLY = 0,
  MMC_HI_AND_SI = 1
} MMC_mode_t;

typedef enum
{
  MMC_CALQSTATUSUNCALIBRATED = 0,
  MMC_CALQSTATUSPOOR = 1,
  MMC_CALQSTATUSGOOD = 2,
  MMC_CALQSTATUSBEST = 3
} MMC_cal_quality_t;

typedef struct
{
  float dtime;   /* Delta time since previous call [s] */
  float mag[3];  /* Magnetometer sensor output [uT/50] */
} MMC_input_t;

typedef struct
{
  float HI_bias[3];               /* Hard iron offset array [uT/50] */
  float SI_matrix[3][3];          /* Soft iron correction matrix [-] */
  float B;                        /* Estimated magnetic field [uT/50]
                                   (to be used in HI only mode) */
  MMC_cal_quality_t cal_quality;  /* Calibration quality factor */
} MMC_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_MC_Exported_Functions MOTION_MC_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionMC engine
 * @param  mode calibration mode to set, hard-iron only mode (faster) or hard-iron + soft-iron
 * @retval pointer to the new algorithm instance
 */
void *MotionMC_initialize(MMC_mode_t mode);

/**
 * @brief  Deinitialize the MotionMC engine
 * @param  mmc pointer to the algorithm instance
 * @retval none
 */
void MotionMC_deinitialize(void *mmc);

/**
 * @brief  This function runs one step of the magnetometer calibration algorithm
 * @param  mmc pointer to the algorithm instance
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @retval none
 */
void MotionMC_update(void *mmc, MMC_output_t *data_out, MMC_input_t *data_in);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval number of characters in the version string
 */
uint8_t MotionMC_get_lib_version(char *version);

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

#endif /* _MOTION_MC_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

