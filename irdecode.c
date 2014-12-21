#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

FILE* lirc_dev;

void
irread(int* space_len, int* pulse_len)
{
    uint32_t packets[2];

    if(!fread(&packets, sizeof(packets), 1, lirc_dev)) {
        perror("fread");
        exit(EXIT_FAILURE);
    }

    if((packets[0] >> 24) != 0) {
        fprintf(stderr, "expected space\n");
        exit(EXIT_FAILURE);
    }

    if((packets[1] >> 24) != 1) {
        fprintf(stderr, "expected pulse\n");
        exit(EXIT_FAILURE);
    }

    *space_len = packets[0] & 0xffffff;
    *pulse_len = packets[1] & 0xffffff;
}

int
main(int argc, const char** argv)
{
    if(argc < 2) {
        fprintf(stderr, "usage: irdecode <lirc dev>\n");
        return 1;
    }

    lirc_dev = fopen(argv[1], "r");

    if(!lirc_dev) {
        perror("fopen");
        return 1;
    }

    int bits_written = 0;

    while(1) {
        int space_len, pulse_len;
        irread(&space_len, &pulse_len);

        if(space_len > 1600) {
            // new frame
            printf("\n");
        } else if(space_len > 800) {
            // 0 bit
            printf("0");
            bits_written++;
        } else {
            // 1 bit
            printf("1");
            bits_written++;
        }

        if(bits_written == 8) {
            bits_written = 0;
            printf(" ");
        }

        fflush(stdout);
    }
}
