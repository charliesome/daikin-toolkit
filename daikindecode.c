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
get_mode(command_t* cmd)
{
    return (cmd->second_part[5] >> 4) & 7;
}

int
get_power_state(command_t* cmd)
{
    return cmd->second_part[5] & 1;
}

int
get_fan_strength(command_t* cmd)
{
    return cmd->second_part[8] >> 4;
}

int
get_swing_mode(command_t* cmd)
{
    return (cmd->second_part[8] >> 3) & 1;
}

int
get_powerful_mode(command_t* cmd)
{
    return cmd->second_part[13] & 1;
}

int
verify_checksums(command_t* cmd)
{
    uint8_t sum = 0;

    for(int i = 0; i < 7; i++) {
        sum += cmd->first_part[i];
    }

    if(sum != cmd->first_part[7]) {
        return 0;
    }

    sum = 0;

    for(int i = 0; i < 18; i++) {
        sum += cmd->second_part[i];
    }

    if(sum != cmd->second_part[18]) {
        return 0;
    }

    return 1;
}

int
main()
{
    while(1) {
        command_t cmd = read_command();

        print_command(&cmd);
        printf("Checksum:      %s\n", verify_checksums(&cmd) ? "valid" : "invalid");
        printf("Temperature:   %d C\n", get_temperature(&cmd));
        printf("Power:         %s\n", get_power_state(&cmd) ? "on" : "off");

        switch(get_mode(&cmd)) {
        case 0:
            printf("Mode:          auto\n");
            break;
        case 2:
            printf("Mode:          dry\n");
            break;
        case 3:
            printf("Mode:          cool\n");
            break;
        case 4:
            printf("Mode:          heat\n");
            break;
        case 6:
            printf("Mode:          fan\n");
            break;
        default:
            printf("Mode:          unknown?\n");
            break;
        }

        if(get_fan_strength(&cmd) & 0x8) {
            printf("Fan strength:  auto\n");
        } else {
            printf("Fan strength:  %d/5\n", get_fan_strength(&cmd) - 2);
        }

        printf("Swing mode:    %s\n", get_swing_mode(&cmd) ? "on" : "off");
        printf("Powerful mode: %s\n", get_powerful_mode(&cmd) ? "on" : "off");

        printf("\n");
    }
}
