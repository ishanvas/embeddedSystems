#include <types.h>


#ifndef NEW_SWI_H
#define NEW_SWI_H

void New_S_Handler();


ssize_t read(int fd, void *buf, size_t count)  ;
ssize_t write(int fd, const void *buf, size_t count);
void exit(unsigned *argv);

unsigned long time ();
void sleep (unsigned sleep_time);

void setup_stack(int argc, char *argv[]);
void setup_irq_stack();

#endif /* NEW_SWI_H */
