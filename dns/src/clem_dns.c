#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr, htons
#include "dns_packet_parsing.c"
#include "dns_server_rr_files.c"

#define BUF_SIZE 500


void ip_string_to_array(char * ip_str, unsigned char * ip_tab){
    int a,b,c,d;
    sscanf(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
    ip_tab[0] = (unsigned char) a;
    ip_tab[1] = (unsigned char) b;
    ip_tab[2] = (unsigned char) c;
    ip_tab[3] = (unsigned char) d;
}


int main(int argc, char const *argv[])
{
    /**Définition des variables **/
    int num_port, size;
    const char * fichier_conf;

    char * root_server = "127.0.0.1";
    int root_port = 8888;

    char qname[255];
    char ip_address[32];
    unsigned char ip_address_array[4];

    int sock_id;
    struct sockaddr_in server_adr;
    struct sockaddr_in client_adr;
    socklen_t client_adr_len = sizeof(client_adr);

    int sock_id2;
    struct sockaddr_in server_adr2;
    struct sockaddr_in client_adr2;
    socklen_t client_adr_len2 = sizeof(client_adr2);

    char buffer[BUF_SIZE];
    char original_message[BUF_SIZE];

    if(argc != 2){
        printf("Usage : ./local_dns_server_v1 [port UDP d’écoute]\n");
        exit(99);
    }

    num_port = atoi(argv[1]);

    /* Creation d'une socket en mode datagramme  */
    /*-------------------------------------------*/
    sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sock_id < 0 ) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    /* Association adresse et port à la socket (bind)  */
    /*-------------------------------------------------*/

    /* Initialisation adresse locale du serveur  */
    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_port = htons(num_port/*SERVER_PORT*/); 
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY); // wildcard address

    if (bind(sock_id, (struct sockaddr *) &server_adr, sizeof(server_adr)) < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    /* Réception des messages (attente active)  */
    /*------------------------------------------*/
    while (1) {

        printf("\nServeur en attente...\n");
        client_adr_len = sizeof(client_adr);
        if ((size = recvfrom(sock_id, original_message, BUF_SIZE, 0, (struct sockaddr *)&client_adr, &client_adr_len)) < 0) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

        //print_dns_packet(buffer, size);

        qname_from_question(original_message, size, qname);

        char * host = strtok(qname, ".");
        char * domain = strtok(NULL, ".");
        char * root = strtok(NULL, ".");
        char file_response[255];

        char full_domain[255];
        strcpy(full_domain, domain);
        strcat(full_domain, ".");
        strcat(full_domain, root);

        char full_full_domain[255];
        strcpy(full_full_domain, host);
        strcat(full_full_domain, ".");
        strcat(full_full_domain, full_domain);


        char host2[255];
        strcpy(host2, host);

        int size2;

        /***************/
        /**  ETAPE 1  **/
        /***************/

        /* Creation d'une socket en mode datagramme  */
        /*-------------------------------------------*/
        sock_id2 = socket(AF_INET, SOCK_DGRAM, 0);
        if ( sock_id2 < 0 ) {
            perror("socket error");
            exit(EXIT_FAILURE);
        }

        /* Initiatlisation de l'adresse du serveur */
        /*-----------------------------------------*/
        memset(&server_adr2, 0, sizeof(server_adr2));
        server_adr2.sin_family = AF_INET;
        server_adr2.sin_port = htons(root_port); 
        server_adr2.sin_addr.s_addr = inet_addr(root_server);

        /* Envoi du message au serveur */
        /*-----------------------------*/
        
        int size = build_dns_request(buffer, full_full_domain);


        if ( (sendto(sock_id, buffer, size, 0, (struct sockaddr*) &server_adr2, sizeof(server_adr2))) < 0) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }

        client_adr_len2 = sizeof(client_adr2);
        if ((size = recvfrom(sock_id, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_adr2, &client_adr_len2)) < 0) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

        ns_ip_from_add_section(buffer, ip_address_array);


        int next_port = ip_address_array[0] * ip_address_array[3];
        close(sock_id2);




        /***************/
        /**  ETAPE 2  **/
        /***************/

        sprintf(ip_address, "%d.%d.%d.%d", ip_address_array[0], ip_address_array[1], ip_address_array[2], ip_address_array[3]);


        /* Creation d'une socket en mode datagramme  */
        /*-------------------------------------------*/
        sock_id2 = socket(AF_INET, SOCK_DGRAM, 0);
        if ( sock_id2 < 0 ) {
            perror("socket error");
            exit(EXIT_FAILURE);
        }

        /* Initiatlisation de l'adresse du serveur */
        /*-----------------------------------------*/
        memset(&server_adr2, 0, sizeof(server_adr2));
        server_adr2.sin_family = AF_INET;
        server_adr2.sin_port = htons(next_port); 
        server_adr2.sin_addr.s_addr = inet_addr(ip_address);

        /* Envoi du message au serveur */
        /*-----------------------------*/
        
        size = build_dns_request(buffer, full_full_domain);


        if ( (sendto(sock_id, buffer, size, 0, (struct sockaddr*) &server_adr2, sizeof(server_adr2))) < 0) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }

        client_adr_len2 = sizeof(client_adr2);
        if ((size = recvfrom(sock_id, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_adr2, &client_adr_len2)) < 0) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

        ns_ip_from_add_section(buffer, ip_address_array);


        next_port = ip_address_array[0] * ip_address_array[3];

        close(sock_id2);




        /***************/
        /**  ETAPE 3  **/
        /***************/

        sprintf(ip_address, "%d.%d.%d.%d", ip_address_array[0], ip_address_array[1], ip_address_array[2], ip_address_array[3]);

        /* Creation d'une socket en mode datagramme  */
        /*-------------------------------------------*/
        sock_id2 = socket(AF_INET, SOCK_DGRAM, 0);
        if ( sock_id2 < 0 ) {
            perror("socket error");
            exit(EXIT_FAILURE);
        }

        /* Initiatlisation de l'adresse du serveur */
        /*-----------------------------------------*/
        memset(&server_adr2, 0, sizeof(server_adr2));
        server_adr2.sin_family = AF_INET;
        server_adr2.sin_port = htons(next_port); 
        server_adr2.sin_addr.s_addr = inet_addr(ip_address);

        /* Envoi du message au serveur */
        /*-----------------------------*/
        
        size = build_dns_request(buffer, full_full_domain);


        if ( (sendto(sock_id, buffer, size, 0, (struct sockaddr*) &server_adr2, sizeof(server_adr2))) < 0) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }

        client_adr_len2 = sizeof(client_adr2);
        if ((size = recvfrom(sock_id, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_adr2, &client_adr_len2)) < 0) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }

        ip_from_answer(buffer, ip_address_array);


        close(sock_id2);


        size2 = build_dns_answer(original_message, ip_address_array);

        sendto(sock_id, original_message, size2, 0, &client_adr, client_adr_len);

    }

    close(sock_id);

	return 0;
}