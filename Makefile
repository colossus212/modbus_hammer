all: modbus

modbus: modbus.c
	gcc -g -o modbus modbus.c -lhammer `pkg-config --libs --cflags glib-2.0 libhammer`

clean:
	rm modbus 
