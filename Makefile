
obj-m :=RFID-RC522.o
RFID-RC522-objs:=rc522.o rc522_api.o
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	# make ARCH=arm CROSS_COMPILE=/bin/arm-linux-gnueabihf- -C /home/kiencate/rpi-kernel/linux M=$(PWD) modules

clean:
	# $(MAKE) ARCH=arm CROSS_COMPILE=/bin/arm-linux-gnueabihf- -C /home/kiencate/rpi-kernel/linux M=$(PWD) clean
	$(MAKE) -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
