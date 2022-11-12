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

void ConfigSensorTemperatura()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);                   // Se configuran 2 entradas analogicas a usar (el numero 2)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH3);                        // Muestra 1 que  va a tomar en pasos de la secuencia 2 (Potenciometro)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH3);                        // Muestra 2 que va a tomar en pasos de la secuencia 2 (Potenciometro)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH3);                        // Muestra 3 que va a tomar en pasos de la secuencia 2 (Potenciometro)
    ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH3|ADC_CTL_IE|ADC_CTL_END); // Ultima muestra y finalizamos la interrupcion (Potenciometro)

    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);                         // Muestras que va a tomar en pasos de la secuencia 1 (sensor)
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);                         // Muestras que va a tomar en pasos de la secuencia 1 (sensor)
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);                          // Muestras que va a tomar en pasos de la secuencia 1 (sensor)
    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);      // Ultima muestra y finalizamos la interrupcion (sensor)

    ADCSequenceEnable(ADC0_BASE, 1);                                                        // Habilitar secuencia 1 (Sensor de temperatura)
    ADCSequenceEnable(ADC0_BASE, 2);                                                        // Habilitar secuencia 2 (Potenciometro)
}
