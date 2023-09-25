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
			uart_puts("BareOS> ");
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
			uart_puts("BareOS> ");			
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
			uart_puts("BareOS> ");
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
		else if (cli_buffer[0] != '\0') // condition to manage empty buffer
		{
			uart_puts(cli_buffer);
			uart_puts(" is not in the command list !!!\n");				
			uart_puts("Please type 'help' for more information.\n");
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
		uart_puts("BareOS> ");
	}
}

void main(){
    // set up serial console
	uart_init();
	uart_puts(" _______  _______  _______  _______  _______  ___ ___  _______  _______\n");
    uart_puts("|   _   ||   _   ||   _   ||       ||       ||   Y   ||   _   ||   _   |\n");
    uart_puts("|.  |___||.  |___||.  |___||.|   | ||___|   ||   |   ||   |   ||.  |   |\n");
    uart_puts("|.  __)_ |.  __)_ |.  __)_ `-|.  |-' /  ___/ |____   | \\___   ||.  |   |\n");
    uart_puts("|:  |   ||:  |   ||:  |   |  |:  |  |:  |  \\     |:  ||:  |   ||:  |   |\n");
    uart_puts("|::.. . ||::.. . ||::.. . |  |::.|  |::.. . |    |::.||::.. . ||::.. . |\n");
    uart_puts("`-------'`-------'`-------'  `---'  `-------'    `---'`-------'`-------'\n");
    uart_puts("\n");
    uart_puts(" _______   _______  _______  _______    _______  _______ \n");
    uart_puts("|   _   \\ |   _   ||   _   \\|   _   |  |   _   ||   _   |\n");
    uart_puts("|.  |   / |.  |   ||.  |   /|.  |___|  |.  |   ||   |___|\n");
    uart_puts("|.  _   \\ |.  _   ||.  _   ||.  __)_   |.  |   ||____   |\n");
    uart_puts("|:  |    \\|:  |   ||:  |   ||:  |   |  |:  |   ||:  |   |\n");
    uart_puts("|::.. .  /|::.|:. ||::.|:. ||::.. . |  |::.. . ||::.. . |\n");
    uart_puts("`-------' `--- ---'`--- ---'`-------'  `-------'`-------'\n");
    uart_puts("\n");

	uart_puts("DEVELOPED BY DAM TA MINH NHAT - S3926946\n");
	uart_puts("\n");
	uart_puts("BareOS> ");
    // run CLI
    while(1) 
	{
    	cli();
    }
}







