#ifndef I2S_HPP
#define I2S_HPP

#include "stm32f4xx.h"

#define AUDIODATA_SIZE 2 // 2 bytes for 16 bit audio
#define DMA_MAX_SZE                 0xFFFF
#define DMA_MAX(_X_)                (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)

class i2s
{
private:
I2S_HandleTypeDef i2sx;

const uint32_t I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};
const uint32_t I2SPLLN[8] = {256, 429, 213, 429, 426, 271, 258, 344};
const uint32_t I2SPLLR[8] = {5, 4, 4, 4, 4, 6, 3, 1};

typedef enum 
{
    I2S2=2,
    I2S3=3,
    I2S4=4

}i2s_num;

public:
/**
 * @brief Construct a new i2s object
 * 
 * @param i2sNum : which interface you want to use
 */
i2s(uint8_t i2sNum, uint32_t audioFreq);

/**
 * @brief initialize the PLLs clocks and sampling
 * 
 * @param audioFreq 
 */
void init(uint32_t audioFreq);

/**
 * @brief Set the audiofreq i2s parameter
 * 
 * @param audioFreq 
 * @return uint8_t : 1:successful 0:failed
 */
uint8_t set_audiofreq(uint32_t audioFreq);

/**
 * @brief set the PLL clock
 * 
 * @param audioFreq 
 */
void config_pll_clock(uint32_t audioFreq);

/**
 * @brief Set the sampling freq object
 * 
 * @param audioFreq 
 */
void set_sampling_freq(uint32_t audioFreq);

/**
 * @brief play the data in the buffer
 * 
 * @param dataBuffer 
 * @param len 
 */
void play(uint16_t* audioBuffer, uint32_t len);

/**
 * @brief change the buffer to be played
 * 
 * @param dataBuffer 
 * @param len 
 */
void change_buffer(uint16_t* audioBuffer, uint32_t len);

/**
 * @brief pause playing
 * 
 */
void pause();

/**
 * @brief resume from a pause
 * 
 */
void resume();
/**
 * @brief completely stop playing
 * 
 */
void stop();

void error_handler();
void half_transfer_callback(void);
void cplt_transfer_callback(void);
};

#endif