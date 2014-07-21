******************
*** readme.txt ***
******************

works fine with "Open On-Chip Debugger 0.5.0-dev-00875-g6349a47 (2011-05-16-17:20)"

how to use:
===========

start openocd once as root and keep it running:
	openocd -f ocd050/board.cfg

flash and run your application:
	arm-elf-gdb -x ocd050/gdbinit main.elf
	(gdb) load
	(gdb) continue
OR
	arm-elf-gdb -x ocd050/gdbinit2 main.elf

flash and debug your application:
	arm-elf-gdb -x ocd050/gdbinit main.elf
	(gdb) load
	(gdb) break my_function
	(gdb) continue
	...
	(gdb) where
	(gdb) print my_variable
	(gdb) step
	(gdb) print my_variable
	...

useful stuff:
=============

set breakpoint:	(gdb) break [symbol]
singlestep: 	(gdb) step
stacktrace:		(gdb) where
dump variables:	(gdb) print [symbol]
interrupt program flow: CTRL+C
exit from gdb: CTRL+D

dirty reset:
	CTRL+C
	monitor reg pc 0x00100000
	continue

