CROSS_COMPILE = /opt/cs10q1/bin/arm-none-linux-gnueabi-
CFLAGS = -Wall -Wextra -Werror -nostdlib -nostartfiles -ffreestanding -std=gnu99

all: ukernel

clean:
	rm kernel ukernel *.o

kernel: start.o main.o link.ld # prefetch_abort.o
	$(CROSS_COMPILE)ld -Tlink.ld -o kernel start.o main.o

%.o: %.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -o $@ -c $<

%.o: %.s
	$(CROSS_COMPILE)as -o $@ $<

kernel.bin: kernel
	$(CROSS_COMPILE)objcopy -O binary $< $@

ukernel: kernel.bin
	mkimage -A arm -O linux -T kernel -C none -a 0x82000000 -e 0x82000020 -n "armos" -d kernel.bin ukernel

