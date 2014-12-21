CFLAGS = -Wall -Wextra -pedantic -Werror -std=c99 -iquote lib/

all: irdecode daikindecode irtx

irtx: irtx.o lib/c_gpio.o
