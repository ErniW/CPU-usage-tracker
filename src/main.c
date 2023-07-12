#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){

    #ifdef DEBUG
        printf("Hello from debug!\n");
    #else
        printf("Hello from build!\n");
    #endif

    FILE* data = fopen("/proc/stat", "r");
    if(data == NULL){
        perror("Error opening file");
        fclose(data);
        return 1;
    }

    char line[256];
    while(fgets(line, sizeof(line), data) != NULL && strncmp(line, "cpu", 3) == 0){
        printf("%s", line);
    }

    fclose(data);

    return 0;
}
