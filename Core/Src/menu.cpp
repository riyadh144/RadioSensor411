#include "menu.hpp"

menu::menu(oled* oled_, uart* uart_,wav_player* wav_player_,sa818* sa818_)
{
    oled1=oled_;
    uart1=uart_;
    wav_player1 = wav_player_;
    sa818A = sa818_;
}

void menu::menu_print() //Menu Print will be called every time the screen is updated in main via timmer interrupt
{
    char temp [22];
    switch (menu_value)
    {
        case MENU_HOME:
            sprintf(temp, "C%.2s S%.2s  ", ch,sq);
            oled1->oled_print(temp, Font_16x26, 0,16);
            break; 
        case MENU_CH:
            sprintf(temp, "CHAN     ");
            oled1->oled_print(temp, Font_16x26, 0,16);
            break;
        case MENU_SQ:
            sprintf(temp, "SQUASH    ");
            oled1->oled_print(temp, Font_16x26, 0,16);
            break;
        case MENU_TMO: //Display Timeout in seconds
            sprintf(temp, "Disp TMO"); 
            oled1->oled_print(temp, Font_16x26, 0,16);
            break;
        case MENU_CH_IN:
            sprintf(temp, "CH:%.2s    ",ch);
            cursorOn=1;
            oled1->oled_print(temp,Font_16x26, 0,16);
            break;
        case MENU_SQ_IN:
            cursorOn=1;
            sprintf(temp, "SQ:%.2s    ",sq);
            oled1->oled_print(temp, Font_16x26, 0,16);
            break;
        case MENU_TMO_IN:
            cursorOn=1;
            sprintf(temp, "DS:%.2s    ",tmo);
            oled1->oled_print(temp, Font_16x26, 0,16);
            break;
        default:
            break;
    }
    oled1->oled_print("menu back   ", Font_11x18,0,46);   

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
    case MENU_CH_IN:
        cursorOn^=1;//switch the state of the cursor
        if(!cursorOn)
        {
            chVal=atoi(ch);
            if(chVal >= 22)
            {   
                chVal = 22;
            }
            cursorPos=0;
            sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);
        }
        break;
    case MENU_MIC_REC:
        menu_value = MENU_MIC_REC_IN;
        break;
    case MENU_MIC_PLAY:
        menu_value = MENU_MIC_PLAY_IN;
        break;
    case MENU_SQ_IN:
        cursorOn^=1;
        if(!cursorOn)
        {
            sqVal=atoi(sq);
            if(sqVal >= 8)
            {
                sqVal = 8;
            }
            cursorPos=0; //reset the cursor positon
            sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);

        }
        break;
    case MENU_TMO_IN:
        cursorOn^=1;
        if(!cursorOn)
        {
            tmoVal=atoi(tmo); //OK is meant to set the value
            if(tmoVal >= 8)
            {
                tmoVal = 8;
            }
            oled1->oled_setTimer(tmoVal);
            cursorPos=0;
            //TODO:Implement the display timer and use tmoVal to modify the prescaler
        }
        break;
    case MENU_MIC_REC_IN:
        oled1->oled_print("Recording!",Font_11x18,10,10);
        AUDIO_Main(); /*  START RECORDING IMMEDIATLY */
        // TODO : IMPLEMENT A FUNCTION TO RECORD IF WANTED        
        break;
    case MENU_MIC_PLAY_IN:
    {
        oled1->oled_print("Track Play: ",Font_11x18,0,15);
        fr = f_findfirst(dp,&fno,SDPath,"????????.wav");
        if( fr == FR_OK && fno.fname[0] ) /*if the file is valid*/
        {
            int file_er;
            oled1->oled_print(fno.fname,Font_11x18,20,30);
		    file_er=wav_player1->file_select(fno.fname);
            wav_player1->play();
            while(!wav_player1->isEndOfFile())
            {
            wav_player1->process();
            }
        }
        wav_player1->stop();
    }
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
        break;
    case MENU_TMO:
        menu_value=MENU_MIC_REC;
        break;
    case MENU_MIC_REC:
        menu_value = MENU_MIC_PLAY;
        break;
    case MENU_MIC_PLAY:
        menu_value = MENU_CH;
        break;
    case MENU_CH_IN:
        cursorPos^=cursorOn; // If the cursor is on switch position
        break;
    case MENU_SQ_IN:
        cursorPos^=cursorOn;//TODO: Add a Draw cursor function 
        break;
    case MENU_TMO_IN:
        cursorPos^=cursorOn;
        break;
    case MENU_MIC_PLAY_IN:
        wav_player1->stop();
        oled1->oled_print("Track Play: ",Font_11x18,0,15);
        fr = f_findnext(dp,&fno); // start searching for .wav files to play
        if( fr == FR_OK && fno.fname[0]  ) /*if the file is valid*/
        {
            int file_er;
            oled1->oled_print(fno.fname,Font_11x18,20,30);
		    file_er=wav_player1->file_select(fno.fname);
            wav_player1->play();
            while(!wav_player1->isEndOfFile())
            {
            wav_player1->process();
            }
        }
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
    wav_player1->stop();
	f_mount(0, "", 1);
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    menu_value = (enum menu_state)(menu_value >> 3);
}

void menu::menu_prev()
{
    wav_player1->stop();
	f_mount(0, "", 1);
    if(oled1->oled_isOledOn()) //If the oled is on reset the timer
    {
        oled1->oled_resetTimer();
    }
    switch (menu_value)
    {
    case MENU_HOME:
        break;
    case MENU_CH:
        menu_value=MENU_MIC_PLAY;
        break;
    case MENU_SQ:
        menu_value=MENU_CH;
        break;
    case MENU_TMO:
        menu_value=MENU_SQ;
        break;
    case MENU_MIC_REC:
        menu_value = MENU_TMO;
        break;
    case MENU_MIC_PLAY:
        menu_value = MENU_MIC_REC;
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
            tmoVal=atoi(tmo); //OK is meant to set the value
            oled1->oled_setTimer(tmoVal);
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
        chVal = atoi(ch);
        if(chVal < 22) // Current max for ch is 22
        {
            chVal++;
        }
        sprintf(ch,"%2d",chVal);
            sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);

        break;
    case MENU_SQ_IN:
        sqVal = atoi(sq);
        if(sqVal < 9) // Current max for Sq is 9
        {
            sqVal++;
        }
        sprintf(sq,"%2d",sqVal);
            sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);
        break;
    case MENU_MIC_PLAY_IN:
        //switch to the previous track in the list using the cursor function
        break;

    default:
        volVal = atoi(vol);
        if(volVal < 8) // Current max for volume is 8
        {
            volVal++;
        }
        sprintf(vol,"%2d",volVal);
        sa818A->sa818_set_volume(volVal);
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
        chVal = atoi(ch);
        if(chVal > 1)
        {
        chVal--;
        }
        sprintf(ch,"%2d",chVal);
        sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);

        break;
    case MENU_SQ_IN:
        sqVal = atoi(sq);
        if(sqVal > 0)
        {
        sqVal--;
        }
        sprintf(sq,"%2d",sqVal);
        sa818A->sa818_configure(0,CHANNEL[chVal],CHANNEL[chVal],sqVal);

        break;   
    case MENU_MIC_PLAY_IN:
        wav_player1->stop(); // stop the previous track that was playing 
        oled1->oled_print("Track Play: ",Font_11x18,0,15);
        fr = f_findnext(dp,&fno); // start searching for .wav files to play
        if( fr == FR_OK && fno.fname[0]  ) /*if the file is valid*/
        {
            int file_er;
            oled1->oled_print(fno.fname,Font_11x18,20,30);
		    file_er=wav_player1->file_select(fno.fname);
            wav_player1->play();
            while(!wav_player1->isEndOfFile())
            {
            wav_player1->process();
            }
        }
        wav_player1->stop();
        break;
    default:
        volVal = atoi(vol);
        if(volVal > 0)
        {
            volVal--;
        }
        sprintf(vol,"%2d",volVal);
        sa818A->sa818_set_volume(volVal);

        break;
    }

}



void menu::keyboard_poll()
{
    int sds=0;
    char x[20];

    uint16_t pin = GPIO_PIN_11;
    //Set The KEY1(PD11) One on

    GPIOD->ODR|=pin;
    while(sds<10)
    {
        sds++;
    }
    sds=0;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_next();//R
            sprintf(x,"next");
            uart1->send_recive(x,"next");   
            break;
        case GPIO_PIN_10:
            menu_number('6');
            sprintf(x,"6");
            uart1->send_recive(x,"6");   
            break;
        case GPIO_PIN_9:
            menu_number('5');
            sprintf(x,"5");
            uart1->send_recive(x,"5");   
            break;
        case GPIO_PIN_8:
            menu_number('4');
            sprintf(x,"4");
            uart1->send_recive(x,"4");   
            break;
        default:
            break;
    }
    GPIOD->ODR&=(~pin);

    pin = GPIO_PIN_12;//PD12
    GPIOD->ODR|=pin;
    
    while(sds<10)
    {
        sds++;
    }
    sds=0;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_prev();//L
            sprintf(x,"prev");
            uart1->send_recive(x,"prev");
            break;
        case GPIO_PIN_10:
            menu_number('8');
            sprintf(x,"8");
            uart1->send_recive(x,"8");   
            break;
        case GPIO_PIN_9:
            menu_number('7');
            sprintf(x,"7");
            uart1->send_recive(x,"7");   
            break;
        case GPIO_PIN_8:
            menu_number('9');
            sprintf(x,"9");
            uart1->send_recive(x,"9");   
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);


    pin = GPIO_PIN_13;//PD13
    GPIOD->ODR|=pin;
    while(sds<10)
    {
        sds++;
    }
    //uart1->send_recive("PD13","PD13");
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_down();//D
            sprintf(x,"down");
            uart1->send_recive(x,"down");
            break;
        case GPIO_PIN_10:
            menu_menu();//M
            sprintf(x,"MENUs");
            uart1->send_recive(x,"MENU");
            break;
        case GPIO_PIN_9:
            menu_number('0');
            sprintf(x,"0");
            uart1->send_recive(x,"0");   
            break;
        case GPIO_PIN_8:
            menu_back();//B
            sprintf(x,"Back");
            uart1->send_recive(x,"Back");
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);

    pin = GPIO_PIN_14;//PD14
    GPIOD->ODR|=pin;
    while(sds<10)
    {
        sds++;
    }
    sds=0;
        //Scan to see which port is on filter out PD8,PD9,PD10,PD15
    switch (GPIOD->IDR& (GPIO_PIN_15|GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8))
    {
        case GPIO_PIN_15:
            menu_up();//U
            sprintf(x,"up");
            uart1->send_recive(x,"up");   
            break;
        case GPIO_PIN_10:
            menu_number('3');
            sprintf(x,"3");
            uart1->send_recive(x,"3");   
            break;
        case GPIO_PIN_9:
            menu_number('2');
            sprintf(x,"2");
            uart1->send_recive(x,"2");   
            break;
        case GPIO_PIN_8:
            menu_number('1');
            sprintf(x,"1");
            uart1->send_recive(x,"1");   
            break;
        default:
            break;

    }
    GPIOD->ODR&=(~pin);

}
