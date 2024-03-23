/*
 * WS2812_SPI.c
 *
 *  Created on: Mar 20, 2024
 *      Author: Launnish
 */

#include "main.h"
#include "WS2812_SPI.h"

#define  NUM_LED 10
uint8_t LED_Data[NUM_LED][4];

extern SPI_HandleTypeDef hspi3;

#define USE_BRIGHTNESS 1
extern int brightness;

void setLED (int led, int RED, int GREEN, int BLUE)
{
	LED_Data[led][0] = led;
	LED_Data[led][1] = GREEN;
	LED_Data[led][2] = RED;
	LED_Data[led][3] = BLUE;
}


// SPI
void ws2812_spi (int GREEN, int RED, int BLUE)
{

	uint32_t color = GREEN<<16 | RED<<8 | BLUE;
	uint8_t sendData[24];
	int indx = 0;

	for (int i=23; i>=0; i--)
	{
		if (((color>>i)&0x01) == 1) sendData[indx++] = 0b110;  // store 1
		else sendData[indx++] = 0b100;  // store 0
	}

	HAL_SPI_Transmit(&hspi3, sendData, 24, 1000);
}

void WS2812_Send (void)
{
	for (int i=0; i<NUM_LED; i++)
	{
		ws2812_spi(LED_Data[i][1],LED_Data[i][2],LED_Data[i][3]);
	}
	HAL_Delay (1);
}



// Rainbow [Mode :1]
uint16_t effStep = 0;

uint8_t rainbow() {

  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<10;j++) {
    ind = (int16_t)effStep + j * 2;
    switch((int)((ind % 20) / 6.666666666666667)) {

      case 0: factor1 = 1.0 - ((float)(ind % 20 - 0 * 6.666666666666667) / 6.666666666666667);
              factor2 = (float)((int)(ind - 0) % 20) / 6.666666666666667;
              setLED(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              WS2812_Send();

              break;

      case 1: factor1 = 1.0 - ((float)(ind % 20 - 1 * 6.666666666666667) / 6.666666666666667);
              factor2 = (float)((int)(ind - 6.666666666666667) % 20) / 6.666666666666667;
              setLED(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              WS2812_Send();

              break;

      case 2: factor1 = 1.0 - ((float)(ind % 20 - 2 * 6.666666666666667) / 6.666666666666667);
              factor2 = (float)((int)(ind - 13.333333333333334) % 20) / 6.666666666666667;
              setLED(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              WS2812_Send();

              break;

    }
  }
  if(effStep >= 20) {effStep = 0; return 0x03; }
  else effStep++;
  return 0x01;
}


// Fade [Mode:2]

unsigned long effStart;

uint8_t fade() {


	  for (int i=0; i<10; i++)
	  {
		 setLED(i, 255, 255, 0);
	  }

	  WS2812_Send();
	  HAL_Delay(600);

	  for (int i=0; i<10; i++)
	  {
		 setLED(i, 0, 255, 255);
	  }

	  WS2812_Send();
	  HAL_Delay(600);

	  for (int i=0; i<10; i++)
	  {
		 setLED(i, 255, 0, 255);
	  }

	  WS2812_Send();
	  HAL_Delay(600);

	  return 0x01;
}


// Spectrum [Mode:3]

uint8_t spectrum() {

	  for (int i=0; i<4; i++)
	  {
		 setLED(i,0 , 255, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  for (int i=4; i>0; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  HAL_Delay(20);

	  for (int i=0; i<10; i++)
	  {
		 setLED(i,0 , 0, 255);
		 HAL_Delay(15);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  for (int i=10; i>0; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(15);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  HAL_Delay(20);

	  for (int i=0; i<7; i++)
	  {
		 setLED(i,255 , 0, 0);
		 HAL_Delay(25);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  for (int i=7; i>0; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(25);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	  HAL_Delay(20);
	  return 0x01;
}
// Rain [Mode:4]
uint8_t rain() {

	  for (int i=10; i>7; i--)
	  {
		 setLED(i, 255, 0, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }
	  HAL_Delay(100);
	  for (int i=10; i>7; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }


	  for (int i=7; i>4; i--)
	  {
		 setLED(i, 0, 255, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }
	  HAL_Delay(100);
	  for (int i=7; i>4; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }


	  for (int i=4; i>0; i--)
	  {
		 setLED(i, 0, 0,255);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }
	  HAL_Delay(100);
	  for (int i=4; i>0; i--)
	  {
		 setLED(i, 0, 0, 0);
		 HAL_Delay(35);
		 #undef NUM_LED
		 #define NUM_LED i
		 WS2812_Send();
	  }

	return 0x01;
}

uint8_t off()
{
	for (int i=0; i<10; i++)
	{
		 setLED(i, 0, 0, 0);
		 WS2812_Send();
	}
	return 0x01;
}

