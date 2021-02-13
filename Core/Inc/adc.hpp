#ifndef ADC_HPP
#define ADC_HPP

#include "stm32f4xx.h"

class adc
{
    private:
    ADC_HandleTypeDef hadc;
    /**
     * @brief Slope of the ADC equation
     * 
     */
    float eq_a=3.3*2/4096;
    /**
     * @brief offset of adc equation
     * 
     */
    float eq_b=0;
    public:
    typedef enum 
    {
        ADC_1=1,
        ADC_2=2,
        ADC_3=3

    }adc_enum;
    /**
     * @brief the constructor for the adc
     * 
     * @param hadc: pointer of the adc used
     */
    adc(adc_enum adc_);
    /**
     * @brief set the conversion equation for adc
     * @note  ax+b is the equation of the line
     * @param a: the slop of the linear
     * @param b: ofset
     */
    void adc_setEquation(float a, float b);

    /**
     * @brief get scaled value of the ADC
     * 
     * @return float 
     */
    float adc_getValue();
    /**
     * @brief Initialize ADC
     * 
     */
    void init(void);

    /**
     * @brief Handle ADC error
     * 
     */
    void Error_Handler();

};
#endif