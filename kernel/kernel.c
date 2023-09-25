#include "../uart/uart0.h"
#include "../uart/uart1.h"
#include "mbox.h"
#include "framebf.h"
#include "../command/comm.h"
#define MAX_CMD_SIZE 100
#define MAX_NUM_CMD 10

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];	// declare buffer string
	static int time = 1, posi = 0, history_index = -1; // declare time of "tab", position of the cursor from intial name of the OS, history address 
	static char cmd_history [MAX_NUM_CMD] [MAX_CMD_SIZE]; // declare command history
	static int index = 0, num_cmd = 0; // declare index of buffer and number of command
	int cmd = 0; // command holder (0 for not command and 1 for command)
	
	// read character from console
	char c = uart_getc();
	
	
	//put into a buffer until got new line character
	if (c != '\n'){
		
		// read for backspace 
		if (c == '\b')
		{
			if (posi > 0) 				// condition for backspace not delete "BareOS>"
			{
			uart_sendc('\b'); 			// cursor move back 1 char
			uart_sendc(' ');			// delete the char	
			uart_sendc('\b');	
			cli_buffer[index-1] = ' ';	// delete that char in the buffer
			index--;					// reduce index of buffer
			posi--;						// reduce cursor position
			}
			time = 1;					// reset time of tab
		}
		
		// read for tab
		else if (c == '\t')
		{
			
			cli_buffer[index] = '\0';		// end buffer string
			del_line();						// delete current display
			uart_puts("Team Dragon> ");
			autocomp(cli_buffer,time);		// call auto-complete function
			uart_puts(cli_buffer);			// display auto-complete command
			index = strlen (cli_buffer);	// update new index of buffer
			posi = index;					// update position of cursor
			time ++;						// increase time of tab pressed
		}

		// browse command history
		
		// read for underline
		else if (c == '_')
		{
			history_index ++;								// increase history address
			if (history_index > (num_cmd - 1))				// condition to return address to the first command when reach the last
			{
				history_index = 0;
			}
			del_line ();									
			strcpy(cli_buffer,"");							// clear buffer
			strcpy(cli_buffer,cmd_history[history_index]);	// copy the current command to buffer
			index = strlen(cli_buffer);
			uart_puts("Team Dragon> ");			
			uart_puts(cli_buffer);							// print out command
			posi = strlen(cli_buffer);						// update cursor address
		}

		// read for plus
		else if (c == '+')
		{
			history_index --;								//	decrease history address
			if (history_index < 0 )							//  condition to return address to the last command when reach the first
			{
				history_index = num_cmd - 1;
			}
			del_line ();									// similar behavior to the above
			strcpy(cli_buffer,"");
			strcpy(cli_buffer,cmd_history[history_index]);
			index = strlen(cli_buffer);
			uart_puts("Team Dragon> ");
			uart_puts(cli_buffer);
			posi = strlen(cli_buffer);
		}
		
		// read for space at the start of the input
		else if (c == ' ' && index == 0)
		{
			uart_sendc(c);					// display space but not update buffer
			posi ++;
		}
		
		// display and write to the buffer
		else 
		{
			uart_sendc(c);			// display input from user
			cli_buffer[index] = c; 	//Store into the buffer
			index ++;
			posi++;
		}
	} 

	// read for new line (enter)
	else if (c == '\n')
	{
		cli_buffer[index] = '\0'; // end buffer string
		uart_puts("\n");


		// Compare with supported commands and execute 

		//clear command
		if (strcmp(cli_buffer,"clear"))	// check for the buffer match the command
		{
			clear();
			cmd = 1;
		}

		if (strcmp(cli_buffer,"display image slideshow"))	// check for the buffer match the command
		{
			display_slideshow();
			cmd = 1;
		}

        if (strcmp(cli_buffer,"display large image"))	// check for the buffer match the command
		{
			display_largeimg();
			cmd = 1;
		}
		
		if (strcmp(cli_buffer,"display video"))	// check for the buffer match the command
		{
			display_video();
			cmd = 1;
		}
		// non-legit command prompt will be display with warning
		else if (cli_buffer[0] != '\0' && cmd == 0) // condition to manage empty buffer
		{
			uart_puts(cli_buffer);
			uart_puts(" is not in the command list !!!\n");				
			uart_sendc('\n');
		}

		// condition for a command is read
		if (cmd)
		{
			strcpy(cmd_history[num_cmd],cli_buffer); // write to command history
			num_cmd++; 									
		}
		
		
		//Return to command line
		strcpy(cli_buffer,"");	//clear the buffer
		posi = index = 0;		// reset all
		time = 1;
		uart_puts("\n");
		uart_puts("Team Dragon> ");
	}
}

char *welcome_message =
    "8888888888 8888888888 8888888888 88888888888     .d8888b.      d8888   .d8888b.   .d8888b.                                 \n"
    "888        888        888            888        d88P  Y88b    d8P888  d88P  Y88b d88P  Y88b                                \n"
    "888        888        888            888               888   d8P 888  888    888 888    888                                \n"
    "8888888    8888888    8888888        888             .d88P  d8P  888  Y88b. d888 888    888                                \n"
    "888        888        888            888         .od888P   d88   888    Y888P888 888    888                                \n"
    "888        888        888            888        d88P       8888888888        888 888    888                                \n"
    "888        888        888            888        888              888  Y88b  d88P Y88b  d88P                                \n"
    "8888888888 8888888888 8888888888     888        888888888        888    Y8888P     Y8888P                                 \n"
    "                                                                                                                          \n"
    "                                                                                                                          \n"
    "                                                                                                                          \n"
    "88888888888 8888888888        d8888 888b     d888    8888888b.  8888888b.         d8888  .d8888b.   .d88888b.  888b    888 \n"
    "    888     888              d88888 8888b   d8888    888   Y88b 888   Y88b       d88888 d88P  Y88b d88P   Y88b 8888b   888 \n"
    "    888     888             d88P888 88888b.d88888    888    888 888    888      d88P888 888    888 888     888 88888b  888 \n"
    "    888     8888888        d88P 888 888Y88888P888    888    888 888   d88P     d88P 888 888        888     888 888Y88b 888 \n"
    "    888     888           d88P  888 888 Y888P 888    888    888 8888888P      d88P  888 888  88888 888     888 888 Y88b888 \n"
    "    888     888          d88P   888 888  Y8P  888    888    888 888 T88b     d88P   888 888    888 888     888 888  Y88888 \n"
    "    888     888         d8888888888 888       888    888  .d88P 888  T88b   d8888888888 Y88b  d88P Y88b. .d88P 888   Y8888 \n"
    "    888     8888888888 d88P     888 888       888    8888888P   888   T88b d88P     888  Y8888P88    Y88888P   888    Y888 \n"
    "                                                                                                                          \n"
    "                      Developed by Vo Duy Cuong (S3941544)                                                                \n" 
    "                                   Nguyen Tuan Dung (S3927511)                                                            \n"
    "                                   Tran Minh Anh (S3931980)                                                               \n"
    "                                   Dam Ta Minh Nhat (S3926946)                                                            \n"
    "                                   Le Tran Trong Phuoc (S3927230)                                                     \n\n\n";

void main(){
    // set up serial console
	uart_init();
	uart_puts(welcome_message);
	uart_puts("Team Dragon> ");
    // run CLI
    while(1) 
	{
    	cli();
    }
}







