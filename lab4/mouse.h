#ifndef _MOUSE_H
#define _MOUSE_H

//S & U
int mouse_subscribe_int(void);
int mouse_unsubscribe_int();

//OTHERS
unsigned long mouse_reading();
int mouse_writting(unsigned long reg, unsigned long command);
int mouse_packet_handler(int * cntr);
void mouse_packet_printer();
int clean_out_buf();
int poll_register();

states gesture_handling(state_t currstate, sttmachines * machine);

//STATE MACHINE
//members
typedef enum {INITIAL, DRAWING, COMPLETION} state_t;
typedef enum {RBUP, RBDOWN} ev_type_t;
typedef enum {POSSLOPE, NEGSLOPE} slope_states;

//the object
typedef struct{
	short length;
	signed short x_move;
	signed short y_move; //because of the pos/neg slope
	ev_type_t rightbutton_type;
	slope_states slope;
}sttmachines;

#endif /* MOUSE_H */
