/* Define to prevent recursive inclusion */
#ifndef __INC_AUDIO_H_
#define __INC_AUDIO_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "fatfs.h"


void AUDIO_ADC_Config(void);
void AUDIO_Timer_Config(void);
void AUDIO_Main(void);
void AUDIO_DMA2_Stream0_Callback(void);
void AUDIO_DMA1_Stream6_Callback(void);
void Write_Record(static uint16_t recordedSound* Recorded_sound,FIL *fp)


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /*__INC_AUDIO_H_ */
