#ifndef KBD_H
#define KBD_H

//S & U
int kbd_subscribe_int(void);
int kbd_unsubscribe_int();

//OTHERS
void int_handler();
unsigned long reading();
unsigned long kbd_tosend(unsigned long reg, unsigned long command);
unsigned int getCnt();


#endif /* KBD_H */
