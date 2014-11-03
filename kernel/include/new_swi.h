
#ifndef NEW_SWI_H
#define NEW_SWI_H

void New_S_Handler();
void New_IRQ_Handler();
void setup_stack(int argc, char *argv[]);
void setup_irq_stack();

#endif /* NEW_SWI_H */
