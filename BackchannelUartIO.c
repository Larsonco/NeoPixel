/*
 * BackchannelUartIO.c
 *
 * provides simple character I/O through MSP432 Launchpad backchannel UART
 * RWB 3/17/2017
 *
 */

#include "driverlib.h"
/* Standard Includes */
#include <stdio.h>
#include <string.h>

void initBackchannelUART(void) {

    /* UART Configuration Parameter. These are the configuration parameters to
     * make the eUSCI A UART module to operate with a 115200 baud rate. These
     * values were calculated based on the instructions in the MSP432P4xx Family
     * Technical Reference Manual, section 22.3.10 p.721
    */
    const eUSCI_UART_Config uartConfig =
    {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,   // SMCLK Clock Source at 12000000 (divided down from MCLK)
            6,                                // BRDIV (clock prescaler) = INT((12000000/115200=104.167)/16)
            8,                                // UCxBRF INT([(104.167/16)–INT(104.167/16)]×16))=(6.51-6)x16=8.16)
            0x11,                             // UCxBRS = 0x11 (fractional part of N from table on p.721 of tech ref)
            EUSCI_A_UART_NO_PARITY,           // No Parity
            EUSCI_A_UART_LSB_FIRST,           // LSB First
            EUSCI_A_UART_ONE_STOP_BIT,        // One stop bit
            EUSCI_A_UART_MODE,                // UART mode
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
    };
    // set up EUSCI0 in UART mode
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
             GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
}

char my_getc(void)  // wait for character to appear in receive buffer
{
  while(!(UCA0IFG&UCRXIFG));
  return(UCA0RXBUF);
}

void my_gets(char *inString, int max) // collect characters and return string
{
    int i,c;
    char *p;

    /* get max bytes or up to a newline */
    p=inString; // set pointer to first byte address of inString
    for (i=0; i<max; i++) {
        c=my_getc();
        my_putc(c);
        if (c == 0x0D)
            break;
        *p++ = c;
    }
    *p = 0; // terminate string
}

void my_putc(char _c)
{
  while((UCA0IFG & UCTXIFG) != UCTXIFG);  // wait until flag is set to indicate a new byte can be sent
  UCA0TXBUF = _c;
}

void my_puts(char *outString)
{
  unsigned int i, len;

  len = strlen(outString);

  for(i=0 ; i<len ; i++)
  {
      while((UCA0IFG & UCTXIFG) != UCTXIFG);  // wait until flag is set to indicate a new byte can be sent
      UCA0TXBUF = (uint8_t) outString[i];;  // load register with byte to send
  }
}
