
#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

// H sunartisis gia na kanoume paraggelies
void placeOrder(int sock) {
    for (int i = 0; i < 10; i++) {
        int product_id = rand() % 20; // Tuxaio product ID
        char buffer[1024] = {0};
        sprintf(buffer, "%d", product_id);
        send(sock, buffer, strlen(buffer), 0);
        printf("Pelatis %d zhthse to : %d\n", getpid(), product_id);

        // perimenoume gia apantisi apo to client
        int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Null terminate for safety
                printf("Apanthsh tou server: %s\n", buffer);
            } else {
                perror(" Kati xalase sthn diavasma apo ton server");
}
        sleep(1); // mikrh kathusterisi stis paraggelies
    }
}

// H struct gia ta proionta
typedef struct {
    char description[50];
    float price;
    int item_count;
    int purchase_requests;
    int units_sold;
    int unserved_customers[100];
    int unserved_count;
} Product;

// H sunartisi gia na dimiourgisoume to report
void TelikhAnafora(Product catalog[], int catalogSize) {
    int totalSuccessful = 0;
    int totalOrders = 0;
    int totalFailed = 0;
    float totalRevenue = 0;
    printf("Telikh anafora:\n");
    for (int i = 0; i < catalogSize; i++) {
        totalOrders += catalog[i].purchase_requests;
        totalSuccessful += catalog[i].units_sold;
        totalFailed += catalog[i].unserved_count;
        totalRevenue += catalog[i].units_sold * catalog[i].price;

        printf("Proion: %s\n", catalog[i].description);
        printf("Aitimata: %d\n", catalog[i].purchase_requests);
        printf("Poliseis: %d\n", catalog[i].units_sold);
        printf("Mh diathesima: ");
        for (int j = 0; j < catalog[i].unserved_count; j++) {
            printf("%d ", catalog[i].unserved_customers[j]);
        }
        printf("\n\n");
    }
    printf("Oles oi paraggelies: %d\n", totalOrders);
    printf("Sum epituxwn: %d\n", totalSuccessful);
    printf("Sum apotuximenwn: %d\n", totalFailed);
    printf(" Kerdi : %.2f\n", totalRevenue);
}


#endif 
