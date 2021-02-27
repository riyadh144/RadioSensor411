
/* Includes */
#include "wav_Recorder.hpp"
#include "SysTick.h"
#include "ff.h"

typedef enum AUDIO_ModeType{
  AUDIO_MODE_IDLE,
  AUDIO_MODE_START_RECORDING,
  AUDIO_MODE_RECORDING
} currentMode;

/* Change the pint you want to use here*/
pin pcx(GPIOC,pin::PIN0,pin::in,pin::NoPull,pin::SPEED_HIGH);

/* Change the adc you want to use here */
adc adcx(adc::ADC_1);


/**
 * @brief   Number of audio samples, recording length = 8.19s
 */
#define NUM_OF_AUDIO_SAMPLES                 ((uint32_t) 0xFFFF)/2


/**
 * @brief   Current audio mode
 */
static AUDIO_ModeType currentMode_ = AUDIO_MODE_START_RECORDING;

/**
 * @brief   Recorded sound data
 */
static uint16_t recordedSound[NUM_OF_AUDIO_SAMPLES] = {0};

/**
 * @brief   Recording finished flag
 */
static uint8_t recordingFinished = 0;

/**
 * @brief   Playing finished flag
 */
static uint8_t playingFinished = 0;



/**
 * @brief   Audio Configuration function
 * @note    Microphone input -> PC1 -> ADC1_IN13
 *          Speaker output   -> DAC_OUT2 -> PA5
 * @param
 * @retval
 */
void AUDIO_ADC_Config(void) //TODO : optimize for all gpio pins
{
  //initialize pins
  pcx.init();
  adcx.init();
  

  /* GPIO Configuration for PC3 */
  /* ********************************** */
  /* Enable port C clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  /* Select Analog mode */
  GPIOC->MODER |= GPIO_MODER_MODER3;

  /* ADC Configuration for PCx -> ADC1_IN13 */
  /* ************************************** */
  /* Enable ADCx clock */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  /* Enable ADC */
  adcx.hadc.Instance->CR2 |= ADC_CR2_ADON;

  /* Select Timer 2 TRGO event as external event for regular group */
  adcx.hadc.Instance->CR2 &= ~ADC_CR2_EXTSEL;
  adcx.hadc.Instance->CR2 |= (ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);

  /* Select ADC1_IN13 regular */
  adcx.hadc.Instance->SQR1 &= ~ADC_SQR1_L;
  adcx.hadc.Instance->SQR3 &= ~ADC_SQR3_SQ1;
  adcx.hadc.Instance->SQR3 |= (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_3);

  /* Continuous DMA requests */
  adcx.hadc.Instance->CR2 |= ADC_CR2_DDS;

  /* Enable DMA mode in ADC */
  adcx.hadc.Instance->CR2 |= ADC_CR2_DMA;

  /* DMA configuration ADC1 -> DMA2_Stream0 (Channel 0) */
  /* ************************************************** */
  /* Enable DMA clock in RCC */
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

  /* Make sure that the DMA2_Stream0 is disabled */
  if(DMA_SxCR_EN == (DMA_SxCR_EN & DMA2_Stream0->CR))
  {
    /* DMA2_Stream0 is enabled, shall be disabled first */
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;

    /* Wait until EN bit is cleared */
    while(DMA_SxCR_EN == (DMA_SxCR_EN & DMA2_Stream0->CR))
    {
      /* Do nothing until EN bit is cleared */
    }
  }
  else
  {
    /* Do nothing, DMA2_Stream0 is not enabled */
  }

  /* Select the DMA channel 0 in CHSEL[2:0] in the DMA_SxCR */
  DMA2_Stream0->CR &= ~DMA_SxCR_CHSEL;

  /* Select stream priority very high */
  DMA2_Stream0->CR |= DMA_SxCR_PL;

  /* Select the data transfer direction peripheral(ADC)-to-memory */
  DMA2_Stream0->CR &= ~DMA_SxCR_DIR;

  /* Select memory and peripherals sizes 2 bytes*/
  DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
  DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

  /* Select FIFO mode */
  DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;

  /* Select FIFO threshold level half FIFO */
  DMA2_Stream0->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);

  /* Select memory incremented mode, peripheral shall has fixed address */
  DMA2_Stream0->CR |= DMA_SxCR_MINC;

  /* Enable DMA transfer complete interrupt */
  DMA2_Stream0->CR |= DMA_SxCR_TCIE;

  /* Set peripheral address */
  DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;

  /* Set memory address */
  DMA2_Stream0->M0AR = (uint32_t)recordedSound;

  /* Set number of data items */
  DMA2_Stream0->NDTR = NUM_OF_AUDIO_SAMPLES;
}



/**
 * @brief   Audio main function
 * @note
 * @param
 * @retval
 */
void AUDIO_Main(char* fileName_)
{
  switch (currentMode_)
  {
    case AUDIO_MODE_START_RECORDING:

      /* Enable DMA2_Stream0 */
      DMA2_Stream0->CR |= DMA_SxCR_EN;

      /* Enable external trigger */
      adcx.hadc.Instance->CR2 |= ADC_CR2_EXTEN_0;

      /* Change current mode */
      currentMode_ = AUDIO_MODE_RECORDING;
      break;

    case AUDIO_MODE_RECORDING:
      /* Check recording finished flag */
      if(1 == recordingFinished)
      {
        /* Disable external trigger */
        adcx.hadc.Instance->CR2 &= ~ADC_CR2_EXTEN;

        /* Reset recording finished flag */
        recordingFinished = 0;

        /* Change current mode */
        currentMode_ = AUDIO_MODE_IDLE;
      }
      else
      {
        /* Do nothing, still recording */
      }
      break;

    default:
      break;
  }
}

/**
 * @brief   IRQ callback function
 * @note
 * @param   None
 * @retval  None
 */
void AUDIO_DMA2_Stream0_Callback(void)
{
  /* Check transfer complete flag */
  if(DMA_LISR_TCIF0 == (DMA_LISR_TCIF0 & DMA2->LISR))
  {
    /* Clear all interrupt flags */
    DMA2->LIFCR = (DMA_LIFCR_CFEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CTEIF0
        | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0);

    /* Set Recording finished flag */
    recordingFinished = 1;
  }
  else
  {
    /* Do nothing, this interrupt is not handled */
  }
}

/**
 * @brief   IRQ callback function
 * @note
 * @param   None
 * @retval  None
 */
void AUDIO_DMA1_Stream6_Callback(void)
{
  /* Check transfer complete flag */
  if(DMA_HISR_TCIF6 == (DMA_HISR_TCIF6 & DMA1->HISR))
  {
    /* Clear all interrupt flags */
    DMA1->HIFCR = (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6
        | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);

    /* Set playing finished flag */
    playingFinished = 1;
  }
  else
  {
    /* Do nothing, this interrupt is not handled */
  }
}

void Write_Record(uint16_t* recordedSound,FIL *fp, char* fileName)
{
  //File object
  FRESULT res=f_mount(&SDFatFS, SDPath, 1);
	FIL fil;
  // CONTINUE BUILDING THIS FUNCTION TO INSURE IT WORKS
  int file_er;
	if ( res == FR_OK) {
    res = f_open(fp,fileName,0);
    if(res != FR_OK)
    {
      return;
    }
    f_write(fp,&recordedSound,0xFFFFFFFF,0); /* write to the file created */

	}
  f_close(fp);
}

void AUDIO_Timer_Config(void)
{
  __NOP();//TODO Implement
}