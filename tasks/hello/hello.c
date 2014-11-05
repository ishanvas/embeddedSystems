/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	char buf[100];

	//const char hello[] = "Hello World\r\n";

	unsigned long start_time = time();
	printf("%lu\n", start_time);
	

	//sleep(1565);
	read(STDIN_FILENO, buf, 100);
	//write(STDOUT_FILENO, hello, sizeof(hello)-1);


	unsigned long end_time = time();
	printf("%lu\n", end_time);
	return 0;
}
