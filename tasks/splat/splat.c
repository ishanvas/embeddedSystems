/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

/*Submitted by : 
Ishan Vashishtha : ivashish
Leelakrishna Nukala : lnukala
*/

#include <unistd.h>
#include <stdio.h>

static unsigned state = 0;


void changeState()
{
        /*Defining the states of the buffer*/
	const char cursor_display_0[] = "\b|";  //State 1
	const char cursor_display_1[] = "\b/";  //State 2
	const char cursor_display_2[] = "\b-";  //State 3
	const char cursor_display_3[] = "\b\\"; //State 4 

        /*Writing the state to stdout and changing the state to the next state*/
	if(state == 0)
	{
		write(STDOUT_FILENO, cursor_display_0, sizeof(cursor_display_0));
		state++;
	}
	else if (state ==1)
	{
		write(STDOUT_FILENO, cursor_display_1, sizeof(cursor_display_1));
		state++;
	}
	else if (state == 2)
	{
		write(STDOUT_FILENO, cursor_display_2, sizeof(cursor_display_2));
		state++;
	}
	else 
	{
		write(STDOUT_FILENO, cursor_display_3, sizeof(cursor_display_3));
		state =0;
	}



}



int main(int argc, char** argv)
{
	/*Non terminating loop*/
	while(1)
	{
		sleep(160); //Sleep for 160 ms
		changeState();
	}


	return 0;
}
