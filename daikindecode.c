#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char first_part[8];
    char second_part[19];
}
command_t;

command_t
read_command()
{
    command_t command;

    if(!fread(&command, sizeof(command), 1, stdin)) {
        exit(EXIT_FAILURE);
    }

    return command;
}

int
main()
{
    while(1) {
        command_t cmd = read_command();

        printf("first part:  ");

        for(int i = 0; i < 8; i++) {
            printf("%02x ", cmd.first_part[i]);
        }

        printf("\n");

        printf("second part: ");

        for(int i = 0; i < 19; i++) {
            printf("%02x ", cmd.second_part[i]);
        }

        printf("\n\n");
    }
}
