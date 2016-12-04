#Dossiers
O=./o
SRC=./src

#Commandes
MAIN=gcc -o
PRECMP=gcc -c -o
FLAG= -std=c99

all: dns_client dns_server_v0 dns_server_v1

dns_server_v1: src/dns_server_v1.c dns_lib_perso.o dns_packet_parsing.o dns_server_rr_files.o
	$(MAIN) $@ $(SRC)/dns_server_v1.c $(O)/dns_lib_perso.o $(O)/dns_packet_parsing.o $(O)/dns_server_rr_files.o

dns_client: src/dns_client.c dns_lib_perso.o dns_packet_parsing.o dns_server_rr_files.o
	$(MAIN) $@ $(SRC)/dns_client.c $(O)/dns_lib_perso.o $(O)/dns_packet_parsing.o $(O)/dns_server_rr_files.o

dns_server_v0: src/dns_server_v0.c dns_lib_perso.o dns_packet_parsing.o dns_server_rr_files.o
	$(MAIN) $@ $(SRC)/dns_server_v0.c $(O)/dns_lib_perso.o $(O)/dns_packet_parsing.o $(O)/dns_server_rr_files.o

dns_lib_perso.o: src/dns_lib_perso.c
	$(PRECMP) $(O)/dns_lib_perso.o $(SRC)/dns_lib_perso.c $(FLAG)

dns_packet_parsing.o: src/dns_packet_parsing.c
	$(PRECMP) $(O)/dns_packet_parsing.o $(SRC)/dns_packet_parsing.c $(FLAG)

dns_server_rr_files.o: src/dns_server_rr_files.c
	$(PRECMP) $(O)/dns_server_rr_files.o $(SRC)/dns_server_rr_files.c $(FLAG)

clean:
	rm -rf *.o
