#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32f4xx_usart.h"

int fgetc(FILE *fp);
int fputc(int c, FILE *f);
void uprintf(USART_TypeDef* USARTx, char *fmt, ...);
void usart1_config(uint32_t baud);
void usart2_config(uint32_t baud);
