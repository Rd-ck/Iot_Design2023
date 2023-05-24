#ifndef __PRINTF_H__
#define __PRINTF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "usart.h"

/* printf */
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */


#ifdef __cplusplus
}
#endif

#endif
