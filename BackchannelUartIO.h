/*
 * BackchannelUartIO.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Atalville
 */

#ifndef BACKCHANNELUARTIO_H_
#define BACKCHANNELUARTIO_H_

void initBackchannelUART(void);
char my_getc(void);
void my_gets(char *inString, int max);
void my_putc(char _c);
void my_puts(char *outString);



#endif /* BACKCHANNELUARTIO_H_ */
