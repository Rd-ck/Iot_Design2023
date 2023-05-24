#include "printf.h"

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

GETCHAR_PROTOTYPE
{
  int ch;

  HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
