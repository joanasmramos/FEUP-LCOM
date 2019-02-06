//TOADDMACROS

#define ESC 	0x81
#define KBD_IRQ 	0x01
#define BIT(n) (0x01<<(n))
#define OUT_BUF 	0x60
#define STAT_REG 	0x64
#define WAIT_KBC 	20000
#define OBF 	BIT(0)
#define IBF 	BIT(1)
#define IBF BIT(1)
#define PAR_ERR 	0x80
#define TO_ERR 		0x40
#define TWO_B 		0xE0
#define COMMAND		0x20
#define KBC_CMD_REG 0x64


/*MOUSE*/
#define MOUSE_IRQ			12

#define ACK					0xFA		//OK
#define NACK				0xFE		//1 byte error
#define ERROR				0xFC		//2nd consecutive byte error

#define STAT_AUX 			BIT(5)

#define MOUSE_WRITECMD		0x60		//cmd byte argument
#define MOUSE_READCMD		0x20		//cmd byte return
#define MOUSE_DISABLE		0xA7	/*These are for KBC and must be written to port 0x64*/
#define MOUSE_ENABLE		0xA8
#define MOUSE_CHECK_ITF		0xA9		//returns 0 if OK
#define MOUSE_WRITE_BTM		0xD4		//byte(argument)
#define MOUSE_READ_DATA		0xEB

#define MOUSE_BTT_L			BIT(0)
#define MOUSE_BTT_R			BIT(1)
#define MOUSE_BTT_M			BIT(2)
#define MOUSE_X_SIGN		BIT(4)
#define MOUSE_Y_SIGN		BIT(5)
#define MOUSE_X_OVFL		BIT(6)
#define MOUSE_Y_OVFL		BIT(7)

#define SET_REMOTE_MODE		0xF0
#define SET_STREAM_MODE		0xEA
#define MOUSE_ENABLE_DATA	0xF4
#define MOUSE_DISABLE_DATA	0xF5 		//FOR STREAM MODE
#define MOUSE_REQUEST_STAT	0xE9


#define NUMBER_OF_TRIES		6
