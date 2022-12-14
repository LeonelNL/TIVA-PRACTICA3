#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uartconfig.h"
#include "configsensortemp.h"


unsigned char dat[6];
uint32_t ui32ADC0Value[4];
uint32_t ui32ADC0Value1[4];
volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ConfigUART();
    ConfigSensorTemperatura();

    while(1)
    {
        ADCIntClear(ADC0_BASE, 1);                  // Limpiar interrupcion sensor de temperatura
        ADCIntClear(ADC0_BASE, 2);                  // Limpiar interrupcion potenciometro
        ADCProcessorTrigger(ADC0_BASE, 1);          // Inicializar trigger secuencia 1 (Sensor de temperatura)
        ADCProcessorTrigger(ADC0_BASE, 2);          // Inicializar trigger secuencia 2 (Potenciometro)

        while(!ADCIntStatus(ADC0_BASE, 1, false))
        {
        }

        ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);    // Lectura Sensor de Temperatura
        ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value1);   // Lectura potenciometro 1

        ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
        ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
        ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

        dat[0] = '#';
        dat[1]= (unsigned char)  ((ui32TempValueC/1000));
        dat[2]= (unsigned char)   ((ui32TempValueC/100)-(dat[1]*10));
        dat[3]= (unsigned char)   ((ui32TempValueC/10)-(dat[1]*100)-(dat[2]*10));
        dat[4]= (unsigned char)   ((ui32TempValueC/1)-(dat[1]*1000)-(dat[2]*100)-(dat[3]*10));
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

        SysCtlDelay(1333333); //10 ms

    }
}

