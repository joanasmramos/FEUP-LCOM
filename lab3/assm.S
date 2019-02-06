#include "i8042.h"
.global _kbd_assmhandler
.global _scancoding
.global _readingerror

.data
_scancoding: .byte 0
_readingerror: .byte 0

.text

_kbd_assmhandler:
xorl %eax, %eax
inb $STAT_REG, %al			# Reading status register
testb $PAR_ERR, %al			# Checking parity error
jnz parerror				# If there is an error, return error
testb $TO_ERR, %al			# Checking timeout error
jnz toerror					# Again, if there is an error, return the error
inb $OUT_BUF, %al			# If success, outbuff is read
movl %eax, _scancoding

parerror:
movb $-1, _readingerror
jmp end

toerror:
movb $-1, _readingerror
jmp end

end:
ret
