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
#include "driverlib/debug.h"
#include "uartconfig.h"

unsigned char dat[6];
char dato[13];
volatile uint32_t ui32TempValueC;
uint32_t ui32Status;
uint32_t CentenasNumero1, CentenasNumero2;
uint32_t DecenasNumero1, DecenasNumero2;
uint32_t UnidadesNumero1, UnidadesNumero2;
uint32_t DecimasNumero1, DecimasNumero2;
uint32_t CentecimasNumero1, CentecimasNumero2;
uint32_t i = 0;
uint32_t Numero1, Numero2;
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
        CentenasNumero1 = (dato[1] - '0') * 10000;
        DecenasNumero1 = (dato[2] - '0') * 1000;
        UnidadesNumero1 = (dato[3] - '0') * 100;
        DecimasNumero1 = (dato[5] - '0') * 10;
        CentecimasNumero1 = (dato[6] - '0');
        Numero1 = CentenasNumero1 + DecenasNumero1 + UnidadesNumero1 + DecimasNumero1 + CentecimasNumero1;
        CentenasNumero2 = (dato[7] - '0') * 10000;
        DecenasNumero2 = (dato[8] - '0') * 1000;
        UnidadesNumero2 = (dato[9] - '0') * 100;
        DecimasNumero2 = (dato[11] - '0') * 10;
        CentecimasNumero2 = (dato[12] - '0');
        Numero2 = CentenasNumero2 + DecenasNumero2 + UnidadesNumero2 + DecimasNumero2 + CentecimasNumero2;

        if(dato[0] == 'x')
        {
            Resultado = Numero1 * Numero2;
        }
        else
        {
            Resultado = Numero1 / Numero2;
        }
    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ConfigUART();

    while(1)
    {

       ui32TempValueC = (1475 - ((2475 * 10)) / 4096)/10;
       dat[0] = '#';
       dat[1]= (unsigned char)  ((Numero1/1000));
       dat[2]= (unsigned char)   ((Numero1/100)-(dat[1]*10));
       dat[3]= (unsigned char)   ((Numero1/10)-(dat[1]*100)-(dat[2]*10));
       dat[4]= (unsigned char)   ((Numero1/1)-(dat[1]*1000)-(dat[2]*100)-(dat[3]*10));
       dat[5] = '&';

       dat[1]= dat[1]+48;
       dat[2]= dat[2]+48;
       dat[3]= dat[3]+48;
       dat[4]= dat[4]+48;

       UARTCharPut(UART0_BASE, dat[0]);
       UARTCharPut(UART0_BASE, dat[1]);
       UARTCharPut(UART0_BASE, dat[2]);
       UARTCharPut(UART0_BASE, dat[3]);
       UARTCharPut(UART0_BASE, dat[4]);
       UARTCharPut(UART0_BASE, dat[5]);
       SysCtlDelay(133333); //10 ms

    }
}

