/**
 ******************************************************************************
 * @file    motion_md.h
 * @author  MEMS Application Team
 * @version V1.0.0
 * @brief   Header for motion_md module
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
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MOTION_MD_H_
#define _MOTION_MD_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_MD MOTION_MD
 *  @{
 */

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float acc[3];  /* Accelerometer sensor output [g] */
  float prs;     /* Pressure sensor output [hPa] */
} MMD_input_t;

typedef struct
{
  float alt;             /* Threshold on altitude difference [m] */
  float shock;           /* Threshold on shock measured by accelerometer [g] */
  float shock_time;      /* Threshold on shock duration [s] */
  float alt_var_steady;  /* Altitude variance threshold [m^2] */
  float acc_var_steady;  /* Accelerometer variance threshold [g^2] */
  float steady_time;     /* Threshold on steady duration [s] */
} MMD_conf_t;

typedef enum
{
  MMD_NONE,
  MMD_MANDOWN
} MMD_output_t;

/**
 *  @}
 */

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_MD_Exported_Functions MOTION_MD_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionMD engine
 * @retval none
 */
void MotionMD_initialize(void);

/**
 * @brief  Get the algorithm configuration.
 * @param  conf pointer to the structure where the current configuration will be written to
 * @retval none
 */
void MotionMD_get_configuration(MMD_conf_t *conf);

/**
 * @brief  Set the algorithm configuration.
 * @param  conf pointer to the structure containing the configuration to be set
 * @retval none
 */
void MotionMD_set_configuration(MMD_conf_t *conf);

/**
 * @brief  Run mandown algorithm.
 * @param  data_in pointer to the structure containing the input data
 * @param  data_out pointer to the enum containing the output data
 * @retval none 
 */
void MotionMD_update(MMD_output_t *data_out, MMD_input_t *data_in);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionMD_get_lib_version(char *version);

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

#endif /* _MOTION_MD_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

