/*
This file is an auto generated file.
Do not modify its contents manually!
*/
#include <string.h>
#include <stddef.h>
#include "uart.h"
void terminal_help(char* in)
{
    if (NULL != strstr(in, "exit"))
    {
        uart_write_string("\tCloses down the terminal.\n\r\t\n\r");
    }
    else if (NULL != strstr(in, "system reset\r\n"))
    {
        uart_write_string("\tForces a software reset.\n\r\t\n\r");
    }
    else if (NULL != strstr(in, "spi3 init"))
    {
        uart_write_string("\tRuns the spi3 initialization code.\n\r\t\n\r");
    }
    else if (NULL != strstr(in, "spi3 send dword"))
    {
        uart_write_string("\tSends a 32 bit value over the spi3 interface.\n\r\tParameters: <dword to send (in hex)>\n\r\t\n\r");
    }
    else if (NULL != strstr(in, "get spi3 status"))
    {
        uart_write_string("\tDisplays the registers values of the spi3 module.\n\r\t\n\r");
    }
    else if (NULL != strstr(in, "get spi4 status"))
    {
        uart_write_string("\tDisplays the registers values of the spi4 module.\n\r\t\n\r");
    }
    else
    {
        uart_write_string("\tType \"help <command>\" for more info\n\r");
        uart_write_string("\tAvailible commands:\n\r");
        uart_write_string("\t------------------------------------\n\r");
        uart_write_string("\texit\n\r\tget spi3 status\n\r\tget spi4 status\n\r\tspi3 init\n\r\tspi3 send dword\n\r\tsystem reset\r\n\n\r\t");
        uart_write_string("\n\r");
    }
}
