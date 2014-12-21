#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    uint8_t first_part[8];
    uint8_t second_part[19];
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

void
print_command(command_t* command)
{
    for(int i = 0; i < 8; i++) {
        printf("%02x ", command->first_part[i]);
    }

    for(int i = 0; i < 19; i++) {
        printf("%02x ", command->second_part[i]);
    }
    printf("\n");
}

void
print_bin_byte(uint8_t byte)
{
    for(int i = 0; i < 8; i++) {
        if(i == 4) {
            putc(' ', stdout);
        }

        putc((byte & 0x80) ? '1' : '0', stdout);
        byte <<= 1;
    }

    putc('\n', stdout);
}

int
get_temperature(command_t* cmd)
{
    return (cmd->second_part[6] >> 1) & 0x1f;
}

int
get_power_state(command_t* cmd)
{
    return cmd->second_part[5] & 1;
}

int
get_powerful_mode(command_t* cmd)
{
    return cmd->second_part[18] & 1;
}

int
main()
{
    while(1) {
        command_t cmd = read_command();

        print_command(&cmd);
        printf("Temperature:   %d C\n", get_temperature(&cmd));
        printf("Power:         %s\n", get_power_state(&cmd) ? "on" : "off");
        printf("Powerful mode: %s\n", get_powerful_mode(&cmd) ? "on" : "off");

        printf("\n");
    }
}
