#include "adc.hpp"

adc::adc(adc_enum adc_)
{
    switch(adc_)
    {
        case ADC_1:
            hadc.Instance = ADC1;
            break;

    }
}
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void adc::init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.ScanConvMode = ENABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.NbrOfConversion = 1;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }



}


float adc::adc_getValue()
{
    float f=4.0;
    // HAL_ADC_Start (&hadc);
    // HAL_ADC_PollForConversion (&hadc, 5);
    // f = eq_a*HAL_ADC_GetValue (&hadc)+eq_b; //y=a*x+b 
    // HAL_ADC_Stop (&hadc);
    return f;
}
void adc::adc_setEquation(float a, float b)
{
    eq_a=a;
    eq_b=b;
}
void adc::Error_Handler()
{
    __NOP();
}