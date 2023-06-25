obj-m := helloworld.o \
		helloworld_test_list.o \
		helloworld_test_timer.o \
		helloworld_test_hrtimer.o \
		helloworld_test_workqueue.o \
		helloworld_test_tasklet.o \
		helloworld_test_dedicated_workqueue.o \
		char_device_driver_basic.o \
		misc_device_driver.o \
		i2c_wii_nunchuk.o

KERNEL_DIR ?= /workspace/learn-embedded/linux

all:
	$(MAKE) -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) modules

dtb:
	pushd $(KERNEL_DIR); \
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs; \
	popd

clean:
	$(MAKE) -C $(KERNEL_DIR) \
		ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
		M=$(PWD) clean

deploy:
	scp *.ko dandan@raspberrypi.local:/home/dandan/code/driver_from_host 
	scp $(KERNEL_DIR)/arch/arm/boot/dts/bcm2708-rpi-zero-w.dtb \
		dandan@raspberrypi.local:/home/dandan/code/driver_from_host 