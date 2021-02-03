#include "adc.hpp"

adc::adc(ADC_HandleTypeDef* hadc_)
{
hadc=hadc_;
}
float adc::adc_getValue()
{
    float f;
    HAL_ADC_Start (hadc);
    HAL_ADC_PollForConversion (hadc, 1000);
    f = eq_a*HAL_ADC_GetValue (hadc)+eq_b; //y=a*x+b 
    HAL_ADC_Stop (hadc);
    return f;
}
void adc::adc_setEquation(float a, float b)
{
    eq_a=a;
    eq_b=b;
}