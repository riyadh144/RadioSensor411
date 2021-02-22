
/* Includes */
#include "audio.h"
#include "SysTick.h"

/**
 * @brief   Timeout value in ms for the PB to remain pressed
 *          for switch to recording mode
 */
#define PB_PRESSED_TIMEOUT                   ((uint32_t) 3000)

/**
 * @brief   Number of audio samples, recording length = 8.19s
 */
#define NUM_OF_AUDIO_SAMPLES                 ((uint32_t) 0xFFFF)


/**
 * @brief   Current audio mode
 */
static AUDIO_ModeType currentMode = AUDIO_MODE_IDLE;

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
 * @note    Microphone input -> PC3 -> ADC1_IN13
 *          Speaker output   -> DAC_OUT2 -> PA5
 * @param
 * @retval
 */
void AUDIO_ADC_Config(void)
{
  /* GPIO Configuration for PC3 */
  /* ********************************** */
  /* Enable port C clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

  /* Select Analog mode */
  GPIOC->MODER |= GPIO_MODER_MODER3;

  /* Select no pull-up, pull-down (reset state) */
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR3;

  /* ADC Configuration for PC3 -> ADC1_IN13 */
  /* ************************************** */
  /* Enable ADC1 clock */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  /* Enable ADC */
  ADC1->CR2 |= ADC_CR2_ADON;

  /* Select Timer 2 TRGO event as external event for regular group */
  ADC1->CR2 &= ~ADC_CR2_EXTSEL;
  ADC1->CR2 |= (ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);

  /* Select ADC1_IN13 regular */
  ADC1->SQR1 &= ~ADC_SQR1_L;
  ADC1->SQR3 &= ~ADC_SQR3_SQ1;
  ADC1->SQR3 |= (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_3);

  /* Continuous DMA requests */
  ADC1->CR2 |= ADC_CR2_DDS;

  /* Enable DMA mode in ADC */
  ADC1->CR2 |= ADC_CR2_DMA;

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
void AUDIO_Main(void)
{
  /* Check current mode */
  switch (currentMode)
  {
    case AUDIO_MODE_IDLE:
      break;

    case AUDIO_MODE_START_PLAYING:
      /* Turn on green led */
      GPIO_TurnON_LED(EVAL_GREEN_LED);

      /* Enable DMA1_Stream6 */
      DMA1_Stream6->CR |= DMA_SxCR_EN;

      /* Enable external trigger */
      DAC->CR |= DAC_CR_TEN2;

      /* Change current mode */
      currentMode = AUDIO_MODE_PLAYING;
      break;

    case AUDIO_MODE_PLAYING:
      /* Check playing finished flag */
      if(1 == playingFinished)
      {
        /* Disable external trigger */
        DAC->CR &= ~DAC_CR_TEN2;

        GPIO_TurnOFF_LED(EVAL_GREEN_LED);

        /* Reset playing finished flag */
        playingFinished = 0;

        /* Change current mode */
        currentMode = AUDIO_MODE_IDLE;
      }
      else
      {
        /* Do nothing, still playing */
      }
      break;

    case AUDIO_MODE_START_RECORDING:
      /* Turn on red led */
      GPIO_TurnON_LED(EVAL_RED_LED);

      /* Enable DMA2_Stream0 */
      DMA2_Stream0->CR |= DMA_SxCR_EN;

      /* Enable external trigger */
      ADC1->CR2 |= ADC_CR2_EXTEN_0;

      /* Change current mode */
      currentMode = AUDIO_MODE_RECORDING;
      break;

    case AUDIO_MODE_RECORDING:
      /* Check recording finished flag */
      if(1 == recordingFinished)
      {
        /* Disable external trigger */
        ADC1->CR2 &= ~ADC_CR2_EXTEN;

        GPIO_TurnOFF_LED(EVAL_RED_LED);

        /* Reset recording finished flag */
        recordingFinished = 0;

        /* Change current mode */
        currentMode = AUDIO_MODE_IDLE;
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
 * @brief   PB Callback function
 * @note
 * @param
 * @retval
 */
void AUDIO_PB_Callback(void)
{
  /* Current system tick */
  uint32_t currentTick = 0;

  /* Clear pending bit */
  EXTI->PR |= EXTI_PR_PR0;

  /* Get start tick */
  uint32_t startTick = SysTick_GetCurrentTick();

  /* Set current mode to playing */
  currentMode = AUDIO_MODE_START_PLAYING;

  /* Check if the button is still pressed for 5 seconds */
  while(GPIO_IDR_IDR_0 == (GPIOA->IDR & GPIO_IDR_IDR_0))
  {
    /* Get current tick */
    currentTick = SysTick_GetCurrentTick();

    /* Check Timeout */
    if((currentTick - startTick) >= PB_PRESSED_TIMEOUT)
    {
      /* Switch current mode to recording */
      currentMode = AUDIO_MODE_START_RECORDING;
    }
    else
    {
      /* Do nothing */
    }
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
/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */