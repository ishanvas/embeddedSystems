/* @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Leelakrishna Nukala
           Ishan Vahishtha
 * @date   2008-10-29
 */
#include <unistd.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	write(STDOUT_FILENO, hello, sizeof(hello)-1);
	return 0;
}
