#include "menu.hpp"

menu::menu(oled* oled_)
{
    oled1=oled_;
}

void menu::menu_print() //Menu Print will be called every time the screen is updated in main via timmer interrupt
{
    char temp [13];

    switch (menu_value)
    {
        case MENU_HOME:
            sprintf(temp, "C:%.2s SQ:%.2s", ch,sq);
            oled1->oled_print(temp, Font_16x26, 0,19);
            break; 
        case MENU_CH:
            sprintf(temp, "    CHAN    ");
            oled1->oled_print(temp, Font_16x26, 0,19);
            break;
        case MENU_SQ:
            sprintf(temp, "   SQUASH   ");
            oled1->oled_print(temp, Font_16x26, 0,19);
            break;
        case MENU_TMO: //Display Timeout in seconds
            sprintf(temp, " Disp TMO "); 
            oled1->oled_print(temp, Font_16x26, 0,19);
            break;
        case MENU_CH_IN:
            sprintf(temp, "   CH:%.2s   ",ch);
            oled1->oled_print(temp,Font_16x26, 0,19);
            break;
        case MENU_SQ_IN:
            sprintf(temp, "   SQ:%.2s   ",sq);
            oled1->oled_print(temp, Font_16x26, 0,19);
            break;
        case MENU_TMO_IN:
            sprintf(temp, "   DS:%.2s   ",tmo);
            oled1->oled_print(temp, Font_16x26, 0,19);
            break;
        default:
            break;
    }
    oled1->oled_print("menu  back", Font_11x18,0,46);   

}

void menu::menu_ok()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    else
    {
        oled1->oled_on();
        oled1->oled_resetTimer();
    }

    switch (menu_value) //TODO: Maybe we could optimize the performance on this but low priority 
    {
    case MENU_HOME:
        break;
    case MENU_CH:
        menu_value=MENU_CH_IN;
        break;
    case MENU_SQ:
        menu_value=MENU_SQ_IN;
        break;
    case MENU_TMO:
        menu_value=MENU_TMO_IN;
        break;
    case MENU_MIC:
        menu_value = MENU_MIC_IN;
    case MENU_CH_IN:
        cursorOn^=1;//switch the state of the cursor
        if(!cursorOn)
        {
            chVal=atoi(ch);
            cursorPos=0;
            //TODO:Call the Modules set channel 
        }
        break;
    case MENU_SQ_IN:
        cursorOn^=1;
        if(!cursorOn)
        {
            chVal=atoi(sq);
            cursorPos=0; //reset the cursor positon
            //TODO:Call the Modules set squash function
        }
        break;
    case MENU_TMO_IN:
        cursorOn%=1;
        if(!cursorOn)
        {
            tmoVal=atoi(tmo); //OK is meant to set the value
            cursorPos=0;
            //TODO:Implement the display timer and use tmoVal to modify the prescaler
        }
        break;
    case MENU_MIC_IN:
        switch(menu_value)
        {
            case MENU_MIC_IN_PLAY:
                menu_value = MENU_MIC_IN_PLAY;
                break;
            case MENU_MIC_IN_REC:
                menu_value = MENU_MIC_IN_REC;
                break;
        }
        break;
    case MENU_MIC_IN_PLAY:
         oled1->oled_print("Choose track to play", Font_16x26, 0,19);
         //TODO: call track list from SD card
         break;
    case MENU_MIC_IN_REC:
        oled1->oled_print("Press Okay to start recording\n\tClick again to stop",Font_16x26,0,19);
        //TODO : call record function to start recording
        break;
    default:
        break;
    }

    
}

void menu::menu_next()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    switch (menu_value)
    {
    case MENU_HOME:
        break;
    case MENU_CH:
        menu_value=MENU_SQ;
        break;
    case MENU_SQ:
        menu_value=MENU_TMO;
    case MENU_TMO:
        menu_value=MENU_CH;
    case MENU_CH_IN:
        cursorPos^=cursorOn; // If the cursor is on switch position
        break;
    case MENU_SQ_IN:
        cursorPos^=cursorOn;//TODO: Add a Draw cursor function 
        break;
    case MENU_TMO_IN:
        cursorPos^=cursorOn;
        break;

    default:
        break;
    }
}

void menu::menu_menu()
{
    if(oled1->oled_isOledOn())
    {
        oled1->oled_resetTimer();
    }
    menu_value = MENU_CH;
}

void menu::menu_back()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    menu_value = (enum menu_state)(menu_value >> 3);
}

void menu::menu_prev()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    switch (menu_value)
    {
    case MENU_HOME:
        break;
    case MENU_CH:
        menu_value=MENU_TMO;
        break;
    case MENU_SQ:
        menu_value=MENU_CH;
        break;
    case MENU_TMO:
        menu_value=MENU_SQ;
        break;
    case MENU_CH_IN:
        cursorPos^=cursorOn; 
        break;
    case MENU_SQ_IN:
        cursorPos^=cursorOn;
        break;
    case MENU_TMO_IN:
        cursorPos^=cursorOn;
        break;

    default:
        break;
    }
}
void menu::menu_number(char num)
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    if(cursorOn) //Only accept an input when the cursor is on
    {
        switch (menu_value)
        {
        case MENU_CH_IN:
            ch[cursorPos]=num; //Changes the value 
            cursorPos^=cursorOn; //Goes to the next position
            break;
        case MENU_SQ_IN:
            sq[cursorPos]=num;
            cursorPos^=cursorOn; 
            break;
        case MENU_TMO_IN:
            tmo[cursorPos]=num;
            cursorPos^=cursorOn; 
            break;

        default:
            break;
        }
    }
}
void menu::menu_up()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    switch (menu_value)
    {
    case MENU_CH_IN:
        if(chVal < 22) // Current max for ch is 22
        {
            chVal++;
        }
        break;
    case MENU_SQ_IN:
        if(sqVal < 9) // Current max for Sq is 9
        {
            sqVal++;
        }
        break;   
    default:
        if(volVal < 8) // Current max for volume is 8
        {
            volVal++;
        }
        break;
    }
}

void menu::menu_down()
{
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    switch (menu_value)
    {
    case MENU_CH_IN:
        if(chVal > 0)
        {
        chVal--;
        }
        break;
    case MENU_SQ_IN:
        if(sqVal > 0)
        {
        sqVal--;
        }
        break;   
    default:
        if(volVal > 0)
        {
            volVal--;
        }
        break;
    }

}


void menu::keyboard_poll()
{
    uint16_t pin = GPIO_PIN_11;
    //Set The KEY1(PD11) One on

    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_next();//R
            break;
        case GPIO_PIN_10:
            menu_number('6');
            break;
        case GPIO_PIN_9:
            menu_number('5');
            break;
        case GPIO_PIN_8:
            menu_number('4');
            break;
        default:
            break;
    }
    GPIOD->ODR&=(~pin);

    pin = GPIO_PIN_12;//PD12
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_prev();//L
            break;
        case GPIO_PIN_10:
            menu_number('8');
            break;
        case GPIO_PIN_9:
            menu_number('7');
            break;
        case GPIO_PIN_8:
            menu_number('9');
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);


    pin = GPIO_PIN_13;//PD13
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_down();//D
            break;
        case GPIO_PIN_10:
            menu_menu();//M
            break;
        case GPIO_PIN_9:
            menu_number('0');
            break;
        case GPIO_PIN_8:
            menu_back();//B
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);

    pin = GPIO_PIN_14;//PD14
    GPIOD->ODR|=pin;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_up();//U
            break;
        case GPIO_PIN_10:
            menu_number('3');
            break;
        case GPIO_PIN_9:
            menu_number('2');
            break;
        case GPIO_PIN_8:
            menu_number('1');
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);

}
