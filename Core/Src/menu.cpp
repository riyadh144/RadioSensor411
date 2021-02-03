#include "menu.hpp"

menu::menu(oled* oled_)
{
    oled1=oled_;
}

void menu::menu_print()
{
    char temp [12];

    switch (menu_value)
    {
        case MENU_HOME:
            sprintf(temp, "C:%s SQ:%s", ch,sq);
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break; 
        case MENU_CH:
            sprintf(temp, "    CHAN    ");
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        case MENU_SQ:
            sprintf(temp, "   SQUASH   ");
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        case MENU_TMO: //Display Timeout in seconds
            sprintf(temp, " Disp TMO "); 
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        case MENU_CH_IN:
            sprintf(temp, "   CH:%s   ",ch);
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        case MENU_SQ_IN:
            sprintf(temp, "   SQ:%s   ",sq);
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        case MENU_TMO_IN:
            sprintf(temp, "   DS:%s   ",tmo);
            oled1->oled_print(temp,12, Font_16x26, 0,19);
            break;
        default:
            break;
    }
    oled1->oled_print("menu  back",10, Font_11x18,0,46);   

}

void menu::menu_ok()
{
    switch (menu_value) //TODO: Maybe we could optimize the performance on this but low priority 
    {
    case MENU_HOME:
        oled1->oled_on();
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

    default:
        break;
    }
}

void menu::menu_next()
{
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

void menu::menu_prev()
{
    switch (menu_value)
    {
    case MENU_HOME:
        break;
    case MENU_CH:
        menu_value=MENU_TMO;
        break;
    case MENU_SQ:
        menu_value=MENU_CH;
    case MENU_TMO:
        menu_value=MENU_SQ;
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
void menu::menu_number(uint8_t num)
{
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