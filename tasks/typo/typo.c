/*Program to accept user input on a prompt
 *Print the entered input
 *Print the time taken to type input
 *Loop back to start
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char buffer[20];
	
	while(1) {
		printf("Please enter your prompt below (buffer limit 20)\n");
		unsigned long time_start = time();
		int read_count = read (STDIN_FILENO, buffer, 20);
		unsigned long time_finish = time() - time_start;
		if(read_count == 20) {
			printf("\n");
		}
		write(STDOUT_FILENO, buffer, read_count);
		printf("\n%lu\n",time_finish);
	}
}
