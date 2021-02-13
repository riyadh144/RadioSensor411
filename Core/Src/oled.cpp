#include "oled.h"
#include "stdlib.h"


#define OLED_RIGHT_HORIZONTAL_SCROLL              0x26
#define OLED_LEFT_HORIZONTAL_SCROLL               0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define OLED_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define OLED_ACTIVATE_SCROLL                      0x2F // Start scroll
#define OLED_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define OLED_NORMALDISPLAY       0xA6
#define OLED_INVERTDISPLAY       0xA7


#define OLED_WRITECOMMAND(command)      comm[0]=COMMAND; comm[1]=(command); oled_write(comm,2)

oled::oled(I2C_HandleTypeDef *hi2c, uint8_t address,TIM_HandleTypeDef *htim)
{
    this->hi2c=hi2c;
	this->address=address;
	this->htim=htim;
}
void oled::init(void)
{
	if (HAL_I2C_IsDeviceReady(this->hi2c, this->address, 1, 20000) != HAL_OK) {
	}
    
    OLED_WRITECOMMAND(0xAE); //display off
	OLED_WRITECOMMAND(0x20); //Set Memory Addressing Mode   
	OLED_WRITECOMMAND(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WRITECOMMAND(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	OLED_WRITECOMMAND(0xC8); //Set COM Output Scan Direction
	OLED_WRITECOMMAND(0x00); //---set low column address
	OLED_WRITECOMMAND(0x10); //---set high column address
	OLED_WRITECOMMAND(0x40); //--set start line address
	OLED_WRITECOMMAND(0x81); //--set contrast control register
	OLED_WRITECOMMAND(0xFF);
	OLED_WRITECOMMAND(0xA1); //--set segment re-map 0 to 127
	OLED_WRITECOMMAND(0xA6); //--set normal display
	OLED_WRITECOMMAND(0xA8); //--set multiplex ratio(1 to 64)
	OLED_WRITECOMMAND(0x3F); //
	OLED_WRITECOMMAND(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WRITECOMMAND(0xD3); //-set display offset
	OLED_WRITECOMMAND(0x00); //-not offset
	OLED_WRITECOMMAND(0xD5); //--set display clock divide ratio/oscillator frequency
	OLED_WRITECOMMAND(0xF0); //--set divide ratio
	OLED_WRITECOMMAND(0xD9); //--set pre-charge period
	OLED_WRITECOMMAND(0x22); //
	OLED_WRITECOMMAND(0xDA); //--set com pins hardware configuration
	OLED_WRITECOMMAND(0x12);
	OLED_WRITECOMMAND(0xDB); //--set vcomh
	OLED_WRITECOMMAND(0x20); //0x20,0.77xVcc
	OLED_WRITECOMMAND(0x8D); //--set DC-DC enable
	OLED_WRITECOMMAND(0x14); //
	OLED_WRITECOMMAND(0xAF); //--turn on OLED panel

    OLED_WRITECOMMAND(OLED_DEACTIVATE_SCROLL); //Deactivate scroll

    HAL_TIM_Base_Start_IT(htim);

}
void oled::oled_fill(OLED_COLOR_t color) {
	/* Set memory */
    for (uint8_t m = 0; m < 8; m++) {
	memset(&oled_buffer[W * m + 1], (uint8_t)color, W);
    }
}
uint8_t oled::oled_write(uint8_t* data, uint16_t count)
{
	HAL_I2C_Master_Transmit(hi2c, address, data, count, 10);
    return 1;
}
void oled::oled_refresh(void)
{
	uint8_t m;
	
	for (m = 0; m < 8; m++) {
		OLED_WRITECOMMAND(0xB0 + m);
		OLED_WRITECOMMAND(0x00);
		OLED_WRITECOMMAND(0x10);
        oled_buffer[W * m]=DATA;
		/* Write multi data */
		oled_write(&oled_buffer[W * m], W+1 );
	}
	Counter_increment();
}

void oled::oled_print(char* string, FontDef_t size, uint16_t x , uint16_t y)
{	
	uint8_t strItr=0;
	// //loop through the chars in the string
	while(strItr<=strlen(string)+3)
	{
		int i=0;
		int j=0;
		int b;
		for (i = 0; i < size.FontHeight; i++) {
			for (j = 0; j < size.FontWidth; j++) {

					oled_buffer[((y+i) / 8)*W+x+j]=0;

				
			}
		}
		for (i = 0; i < size.FontHeight; i++) {
			b = size.data[(*string - 32) * size.FontHeight + i];
			for (j = 0; j < size.FontWidth; j++) {
				if ((b << j) & 0x8000) {
					oled_buffer[((y+i) / 8)*W+x+j]|=1 << ((y+i) % 8);
				} else {
					oled_buffer[((y+i) / 8)*W+x+j]&= ~(1 << ((y+1) % 8));

				}
			}
		}
		x=x+i-size.FontWidth/2;


	
		string++;
		strItr++;
	}
}

void oled::switch_cursor()
{
	
}

void oled::oled_update_battery(float voltage)
{
	char volt [20];
	sprintf(volt,"%.1fv",voltage);
	//next lines are supposed to work as Battery GUI
	DrawRectangle(90,0,4.2,4.2);
	DrawFilledRectangle(90,0,4.2,(atoi(volt)/4.2)*100);
}

void oled::oled_off()
{
	OLED_WRITECOMMAND(0xAE); //display off
	oledOn=0;
}

void oled::oled_on()
{
	OLED_WRITECOMMAND(0xAF); //display on
	oledOn=1;
	oled_resetTimer();
}
uint8_t oled::oled_isOledOn()
{
	return oledOn;
}
void oled::oled_resetTimer()
{
	tmoCounter = 0;
}

void oled::oled_setTimer(uint32_t time)
{
	tmoValue = ((time*1000)/200);
	
}

void oled::Counter_increment(void)
{
	tmoCounter++;
	if(tmoValue == tmoCounter)
	{
		oled_off();
	}
}

void oled::DrawPixel(uint16_t x, uint16_t y) {
	if (
		x >= W||
		y >= H
	) {
		/* Error */
		return;
	}
		oled_buffer[x + (y / 8) * W] |= 1 << (y % 8);

}

void oled::DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= W) {
		x0 = W- 1;
	}
	if (x1 >= W) {
		x1 = W- 1;
	}
	if (y0 >= H) {
		y0 = H - 1;
	}
	if (y1 >= H) {
		y1 = H - 1;
	}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			DrawPixel(x0, i);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			DrawPixel(i, y0);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		DrawPixel(x0, y0);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void oled::DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {

	
	if ((x + w) == W) {
		w = W- x;
	}
	if ((y + h) == H) {
		h = H - y;
	}
	
	DrawLine(x, y, x + w, y);          //Top line 
	DrawLine(x, y + h, x + w, y + h );  //Bottom line 
	DrawLine(x, y, x, y + h);          //Left line 
	DrawLine(x + w, y, x + w, y + h);  //Right line 
}

void oled::DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	uint8_t i;
	
	// Check input parameters 
	if (
		x == W ||
		y == H
	);
	else {
		return;
	}
	
	if ((x + w) == W) {
		w = W- x;
	}
	if ((y + h) == H) {
		h = H - y;
	}
	
	for (i = 0; i = h; i++) {
		DrawLine(x, y + i, x + w, y + i);
	}
}