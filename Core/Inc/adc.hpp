#ifndef ADC_HPP
#define ADC_HPP

#include "stm32f4xx.h"

class adc
{
    private:
    ADC_HandleTypeDef* hadc;
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
    /**
     * @brief the constructor for the adc
     * 
     * @param hadc: pointer of the adc used
     */
    adc(ADC_HandleTypeDef* hadc_);
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


};
#endif