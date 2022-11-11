#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uartconfig.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"

double ResultadoOperacion;
char dato[13];
char num1[6], num2[6];
unsigned char dat[10];
float valor1, valor2;
uint32_t i = 0;
uint32_t ui32Status;
uint32_t Resultado;


void UARTIntHandler(void)
{

    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
        for(i = 0; i < 13; i++)
        {
            dato[i] = UARTCharGetNonBlocking(UART0_BASE);
            SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        }
        num1[0] = dato[1];
        num1[1] = dato[2];
        num1[2] = dato[3];
        num1[3] = dato[5];
        num1[4] = dato[6];
        valor1 = atoi(num1);
        num2[0] = dato[7];
        num2[1] = dato[8];
        num2[2] = dato[9];
        num2[3] = dato[11];
        num2[4] = dato[12];
        valor2 = atoi(num2);

        if(dato[0] == 'x')
        {
            ResultadoOperacion = valor1 * valor2;
        }
        else
        {
            ResultadoOperacion = (valor1/valor2) * 10000;

        }
    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    ConfigUART();

    while(1)
    {

       dat[0] = '#';
       dat[1]= (unsigned char)  ((ResultadoOperacion/100000000));
       dat[2]= (unsigned char)   ((ResultadoOperacion/10000000)-(dat[1]*10));
       dat[3]= (unsigned char)   ((ResultadoOperacion/1000000)-(dat[1]*100)-(dat[2]*10));
       dat[4]= (unsigned char)   ((ResultadoOperacion/100000)-(dat[1]*1000)-(dat[2]*100)-(dat[3]*10));
       dat[5]= (unsigned char)   ((ResultadoOperacion/10000)-(dat[1]*10000)-(dat[2]*1000)-(dat[3]*100)-(dat[4]*10));
       dat[6]= '.';
       dat[7]= (unsigned char)   ((ResultadoOperacion/1000)-(dat[1]*100000)-(dat[2]*10000)-(dat[3]*1000)-(dat[4]*100)-(dat[5]*10));
       dat[8]= (unsigned char)   ((ResultadoOperacion/100)-(dat[1]*1000000)-(dat[2]*100000)-(dat[3]*10000)-(dat[4]*1000)-(dat[5]*100)-(dat[7]*10));
       dat[9] = '&';

       dat[1]= dat[1]+48;
       dat[2]= dat[2]+48;
       dat[3]= dat[3]+48;
       dat[4]= dat[4]+48;
       dat[5]= dat[5]+48;
       dat[7]= dat[7]+48;
       dat[8]= dat[8]+48;

       UARTCharPut(UART0_BASE, dat[0]);
       UARTCharPut(UART0_BASE, dat[1]);
       UARTCharPut(UART0_BASE, dat[2]);
       UARTCharPut(UART0_BASE, dat[3]);
       UARTCharPut(UART0_BASE, dat[4]);
       UARTCharPut(UART0_BASE, dat[5]);
       UARTCharPut(UART0_BASE, dat[6]);
       UARTCharPut(UART0_BASE, dat[7]);
       UARTCharPut(UART0_BASE, dat[8]);
       UARTCharPut(UART0_BASE, dat[9]);

       SysCtlDelay(1333333); //10 ms

    }
}

