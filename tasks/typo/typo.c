/*Program to accept user input on a prompt
 *Print the entered input
 *Print the time taken to type input
 *Loop back to start
*/

/*Submitted by: 
Ishan Vashishtha : ivashish
Leelakrishna Nukala : lnukala
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	char buffer[20]; //Input buffer
	int read_count;  //To store return value of read

	while(1) {
		printf("Please enter your prompt below (buffer limit 20)\n");
		unsigned long time_start = time(); //Start time for typing
		read_count = read (STDIN_FILENO, buffer, 20);
		unsigned long time_finish = time() - time_start; //Calculating time taken to type
		if(read_count == 20) {
			printf("\n"); //Printing the o/p in the next line when the buffer is full
		}
		write(STDOUT_FILENO, buffer, read_count); //Printing the o/p
		printf("\n%lu\n",time_finish); //Printing the typing time
	}
}
