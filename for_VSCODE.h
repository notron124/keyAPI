#ifndef _FOR_VSCODE_H
#define _FOR_VSCODE_H

#include <stdint.h>

#define     __IO    volatile             /*!< defines 'read / write' permissions              */

typedef struct
{
   __IO uint32_t CRL;
   __IO uint32_t CRH;
   __IO uint32_t IDR;
   __IO uint32_t ODR;
   __IO uint32_t BSRR;
   __IO uint32_t BRR;
   __IO uint32_t LCKR;
} GPIO_TypeDef;

#endif /* _FOR_VSCODE_H */