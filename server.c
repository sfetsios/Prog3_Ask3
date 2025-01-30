
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "function.h"

#define PORT 8080

// Theloume kati san katalogo , me 20 proionta
Product catalog[20]; 

int main() {
    // Arxikopoiisi tou server file descriptor kai tou new socket
    int server_fd, new_socket;
    struct sockaddr_in address;
    // To opt einai gia to setsockopt
    int opt = 1;
    // Megethos tou address
    int addrlen = sizeof(address);
    // Buffer gia ta mhnumata
    char buffer[1024] = {0};
    // Mhnymata gia epityxia kai apotyxia
    char *success_response = "Order processed successfully!";
    char *failure_response = "Order failed. Product unavailable.";
    // Metritis gia tis paraggelies
    int totalOrdersProcessed = 0;
    // Oles oi paraggelies pou theloume na exoume
    const int totalOrdersExpected = 50; 

    // Arxikopoiisi tou katalogou
    for (int i = 0; i < 20; i++) {
        sprintf(catalog[i].description, " Antikeimeno %d", i + 1);
        catalog[i].price = 10.0 * (i + 1);
        catalog[i].item_count = 2; 
        catalog[i].purchase_requests = 0;
        catalog[i].units_sold = 0;
        catalog[i].unserved_count = 0;
        memset(catalog[i].unserved_customers, 0, sizeof(catalog[i].unserved_customers));
    }

    //Epeita dimiourgoume to socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Apotuxia sth dimiourgia tou socket");
        exit(EXIT_FAILURE);
    }

    // Setsockopt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Apotuxia sth setsockopt");
        exit(EXIT_FAILURE);
    }

    // Arxikopoiisi tou address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Diadikasia bind tou socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Kati pige strava sto bind");
        exit(EXIT_FAILURE);
    }

    // Listen gia na perimenoume sundeseis apo clients
    if (listen(server_fd, 3) < 0) {
        perror("Apotuxia sth listen");
        exit(EXIT_FAILURE);
    }

    // Oso den exoume exipiretisei oles tis paraggelies
    while (totalOrdersProcessed < totalOrdersExpected) {
        printf("Anamenw Paraggelies\\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Kati phge lathos kata thn apodoxh");
            continue;
        }

        printf("Perimenw parragelies %d/%d...\n", totalOrdersProcessed + 1, totalOrdersExpected);

        // Diavasma tou product_id apo ton client.c
        read(new_socket, buffer, 1024);
        int product_id = atoi(buffer);

        // Elegxos an to product_id einai entos oriwn
        if (product_id >= 0 && product_id < 20) {
            catalog[product_id].purchase_requests++;
            if (catalog[product_id].item_count > 0) {
                // An to proion einai diathesimo
                catalog[product_id].item_count--;
                catalog[product_id].units_sold++;
                send(new_socket, success_response, strlen(success_response), 0);
            } else {
                // An to proion den einai diathesimo
                if (catalog[product_id].unserved_count < 100) {
                catalog[product_id].unserved_customers[catalog[product_id].unserved_count++] = getpid();
                  }else{
                    printf("Den mporei na prostethei o pelatis %d sth lista me tous mh diathesimous tha exoume overflow\n", getpid());
                    }

                send(new_socket, failure_response, strlen(failure_response), 0);
            }
            totalOrdersProcessed++;

        }
        // Kleisimo tou socket otan teleiwsei h paraggelia
        close(new_socket);   

        if (totalOrdersProcessed >= totalOrdersExpected) {
            printf(" Telos parragelies noumero:%d \n", totalOrdersProcessed);
            break;  // kleinoume to server
}
    }
    
    // Ektypwsi tou telikou report
    TelikhAnafora(catalog, 20);

    // Kleisimo tou server_fd
    close(server_fd);
    // Exodos
    printf("Telos programmatos server.c! Ola pigan kala!");
    return 0;
}