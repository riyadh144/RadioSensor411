#include "i2s.hpp"

i2s::i2s(uint8_t i2sNum, uint32_t audioFreq)
{
    switch(i2sNum)
    {
        case I2S2:
            i2sx.Instance = SPI2;
            break;
        case I2S3:
            i2sx.Instance = SPI3;
            break;
        case I2S4:
            i2sx.Instance = SPI4;
            break;
    }
    __HAL_I2S_DISABLE(&i2sx);
    set_audiofreq(audioFreq);

    config_pll_clock(audioFreq);
    if (HAL_I2S_Init(&i2sx) != HAL_OK)
    {
        error_handler();
    }
}

uint8_t i2s::set_audiofreq(uint32_t audioFreq)
{
  /* Initialize the hAudioOutI2s Instance parameter */

 /* Disable I2S block */
  __HAL_I2S_DISABLE(&i2sx);

  /* I2S3 peripheral configuration */
  i2sx.Init.AudioFreq   = audioFreq;
  i2sx.Init.ClockSource = I2S_CLOCK_PLL;
  i2sx.Init.CPOL        = I2S_CPOL_LOW;
  i2sx.Init.DataFormat  = I2S_DATAFORMAT_16B;
  i2sx.Init.MCLKOutput  = I2S_MCLKOUTPUT_ENABLE;
  i2sx.Init.Mode        = I2S_MODE_MASTER_TX;
  i2sx.Init.Standard    = I2S_STANDARD_PHILIPS;
  
  /* Initialize the I2S peripheral with the structure above */
  if(HAL_I2S_Init(&i2sx) != HAL_OK)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void i2s::config_pll_clock(uint32_t audioFreq)
{
    RCC_PeriphCLKInitTypeDef rccclkinit;
    uint8_t index = 0, freqindex = 0xFF;

    for(index = 0; index < 8; index++)
    {
    if(I2SFreq[index] == audioFreq)
    {
        freqindex = index;
    }
    }
    //PLLI2SCLK=50Mhz
    /* Enable PLLI2S clock */
    HAL_RCCEx_GetPeriphCLKConfig(&rccclkinit);
    /* PLLI2S_VCO Input = HSE_VALUE/PLL_M = 2 Mhz */
    if ((freqindex & 0x7) == 0)
    {
    /* I2S clock config
    PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) × (PLLI2SN/PLLM)
    I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */

    rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    rccclkinit.PLLI2S.PLLI2SN = I2SPLLN[freqindex];
    rccclkinit.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
    HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
    }
    else
    {
    /* I2S clock config
    PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) × (PLLI2SN/PLLM)
    I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
    rccclkinit.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    rccclkinit.PLLI2S.PLLI2SN = 258;
    rccclkinit.PLLI2S.PLLI2SR = 3;
    HAL_RCCEx_PeriphCLKConfig(&rccclkinit);
    }
}

void i2s::play(uint16_t* audioBuffer, uint32_t len)
{
    HAL_I2S_Transmit_DMA(&i2sx, audioBuffer, DMA_MAX(len/AUDIODATA_SIZE));
}

void i2s::change_buffer(uint16_t* audioBuffer, uint32_t len)
{
    HAL_I2S_Transmit_DMA(&i2sx, audioBuffer, DMA_MAX(len));
    //TODO:Understand what is going on here
    //For now it is just copied from an example
}

void i2s::pause()
{
    HAL_I2S_DMAPause(&i2sx);
}
void i2s::stop()
{
    HAL_I2S_DMAStop(&i2sx);
}
void i2s::error_handler()
{
    
}