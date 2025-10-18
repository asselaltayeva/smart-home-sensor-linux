#include "print.h"
#include <stdio.h>
#include <stdlib.h>

void printall(const char* filename) {
    FILE *fin = fopen(filename, "rb");
    if (!fin) { perror("File open error"); return; }
    Sensor sensors[20];
    size_t n = fread(sensors, sizeof(Sensor), 20, fin);
    fclose(fin);
    if (n != 20) { printf("Error: Expected 20, got %zu\n", n); return; }
    printf("ID\tName\t\tStatus\n");
    printf("--------------------------------\n");
    for (int i = 0; i < 20; ++i)
        printf("%d\t%-10s\t%-8s\n", sensors[i].id, sensors[i].name, sensors[i].status);
}
