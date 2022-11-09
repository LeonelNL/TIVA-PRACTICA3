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

unsigned char dat[6];

uint32_t pui32ADC0Value[1];

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); //enable pin for LED PF2

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 |ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    while (1) //let interrupt handler do the UART echo function
    {
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }

        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
        dat[0] = '#';
        dat[1]= (unsigned char)  ((pui32ADC0Value[0]/1000));
        dat[2]= (unsigned char)   ((pui32ADC0Value[0]/100)-(dat[1]*10));
        dat[3]= (unsigned char)   ((pui32ADC0Value[0]/10)-(dat[1]*100)-(dat[2]*10));
        dat[4]= (unsigned char)   ((pui32ADC0Value[0]/1)-(dat[1]*1000)-(dat[2]*100)-(dat[3]*10));
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
