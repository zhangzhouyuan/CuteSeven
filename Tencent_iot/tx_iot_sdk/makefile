#if compile x86 demo on unbuntu 64.
CC:=gcc -m32

all:app1 
	@echo build complete
clean:
	-rm  cute7

app1:tx_iot.c myfifo.c
	$(CC) tx_iot.c  myfifo.c -o cute7 -O0 -g3 -I. -I"/Robot/Tencent_iot_SDK/include" -L"/Robot/Tencent_iot_SDK/lib" -ltxdevicesdk -lpthread -ldl -lstdc++

