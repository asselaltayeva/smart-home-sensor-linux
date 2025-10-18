#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device.h"
#include "print.h"

int main(int argc, char *argv[]) {
    // Task 1: Convert text to binary
    if (argc == 4 && strcmp(argv[1], "init") == 0) {
        FILE *fin = fopen(argv[2], "r");
        FILE *fout = fopen(argv[3], "wb");
        if (!fin || !fout) {
            perror("File open error");
            return 2;
        }
        Sensor sensors[20];
        for (int i = 0; i < 20; ++i)
            fscanf(fin, "%d%15s%15s", &sensors[i].id, sensors[i].name, sensors[i].status);
        fwrite(sensors, sizeof(Sensor), 20, fout);
        fclose(fin);
        fclose(fout);
        printf("Task 1 complete: data converted to binary.\n");
        return 0;
    }

    // Task 2: Print out the sensor data to binary
    if (argc == 3 && strcmp(argv[1], "print") == 0) {
        printall(argv[2]);
        return 0;
    }

    // Task 3: Update sensor info by ID
    if (argc == 6 && strcmp(argv[1], "update") == 0) {
        const char* binfile = argv[2];
        int update_id = atoi(argv[3]);
        char* new_name = argv[4];
        char* new_status = argv[5];

        printf("Sensor data before update:\n");
        printall(binfile);

        FILE *fp = fopen(binfile, "r+b");
        if (!fp) {
            perror("File open error");
            return 2;
        }
        Sensor sensor;
        int found = 0;
        for (int i = 0; i < 20; ++i) {
            fseek(fp, i * sizeof(Sensor), SEEK_SET);
            fread(&sensor, sizeof(Sensor), 1, fp);
            if (sensor.id == update_id) {
                if (strcmp(new_name, "-") != 0) {
                    strncpy(sensor.name, new_name, 15);
                    sensor.name[15] = '\0';
                }
                if (strcmp(new_status, "-") != 0) {
                    strncpy(sensor.status, new_status, 15);
                    sensor.status[15] = '\0';
                }
                fseek(fp, i * sizeof(Sensor), SEEK_SET);
                fwrite(&sensor, sizeof(Sensor), 1, fp);
                found = 1;
                break;
            }
        }
        fclose(fp);
        if (!found)
            printf("Sensor with ID %d not found.\n", update_id);

        printf("\nSensor data after update:\n");
        printall(binfile);
        return 0;
    }

    // Deactivate everything at once
    if (argc == 3 && strcmp(argv[1], "deactivateall") == 0) {
        FILE *fp = fopen(argv[2], "r+b");
        if (!fp) {
            perror("File open error");
            return 2;
        }
        Sensor sensor;
        for (int i = 0; i < 20; ++i) {
            fseek(fp, i * sizeof(Sensor), SEEK_SET);
            fread(&sensor, sizeof(Sensor), 1, fp);
            strncpy(sensor.status, "deactivate", 15);
            sensor.status[15]='\0';
            fseek(fp, i * sizeof(Sensor), SEEK_SET);
            fwrite(&sensor, sizeof(Sensor), 1, fp);
        }
        fclose(fp);
        printf("All sensors are now deactivated:\n");
        printall(argv[2]);
        return 0;
    }

    // Command format:
    printf("Usage:\n");
    printf("  %s init data.txt databinary.dat\n", argv[0]);
    printf("  %s print databinary.dat\n", argv[0]);
    printf("  %s update databinary.dat <ID> <name|-> <status|->\n", argv[0]);
    printf("  %s deactivateall databinary.dat\n", argv[0]);
    return 1;
}
