#if compile x86 demo on unbuntu 64.
CC:=gcc -m32

all:app1 app2
	@echo build complete


clean:
	-rm  cute7,myfifo

app1:main.c
	$(CC) main.c -o cute7 -O0 -g3 -I"../include" -L"../lib" -ltxdevicesdk -lpthread -ldl -lstdc++
app2:myfifo.c
	$(CC) myfifo.c -o myfifo -O0 -g3  -lpthread -ldl -lstdc++

