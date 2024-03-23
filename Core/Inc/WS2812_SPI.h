/*
 * WS2812_SPI.h
 *
 *  Created on: Mar 20, 2024
 *      Author: Launnish
 */

#ifndef SRC_WS2812_SPI_H_
#define SRC_WS2812_SPI_H_

void setLED (int led, int RED, int GREEN, int BLUE);
void WS2812_Send (void);
void ws2812_spi (int GREEN, int RED, int BLUE);

uint8_t rainbow(void);
uint8_t fade(void);
uint8_t spectrum(void);
uint8_t rain(void);
uint8_t off(void);


#endif /* SRC_WS2812_SPI_H_ */
