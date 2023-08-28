#include "kernel.h"

void main()
{
	uart_init(); // Set up serial console

	// Run CLI
	while (1)
	{
		cli();
	}
}

void cli()
{
}