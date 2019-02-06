//TOADDMACROS

#define ESC 	0x81
#define KBD_IRQ 	0x01
#define BIT(n) (0x01<<(n))
#define OUT_BUF 	0x60
#define STAT_REG 	0x64
#define WAIT_KBC 	20000
#define OBF 	BIT(0)
#define IBF 	BIT(1)
#define PAR_ERR 	0x80
#define TO_ERR 		0x40
#define TWO_B 		0xe0
#define RDCOMMAND	0x20
#define WRTCOMMAND	0x60
#define KBC_CMD_REG 0x64
