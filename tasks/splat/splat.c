/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <stdio.h>

static unsigned state = 0;


void changeState()
{
	const char cursor_display_0[] = "\b|";
	const char cursor_display_1[] = "\b/";
	const char cursor_display_2[] = "\b-";
	const char cursor_display_3[] = "\b\\";


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
	/* Add code here -- put your names at the top. */
	while(1)
	{
		sleep(160);
		changeState();
	}


	return 0;
}
