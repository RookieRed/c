
proxyFTP: main.o
	gcc main.o -o proxyFTP

main.o: output.o proxy.o ../src/main.c
	gcc output.o proxy.o ../src/main.c -o main.o

output.o: ../src/output.c
	gcc ../src/output.c -o output.o

proxy.o: ../src/proxy.c
	gcc ../src/proxy.c -o proxy.o


clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello