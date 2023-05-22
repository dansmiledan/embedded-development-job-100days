CC = arm-linux-gnueabihf-gcc

app: ioctl_test.c
	$(CC) -o $@ $^
clean:
	rm -f app

deploy: app
	scp $^ dandan@raspberrypi.local:/home/dandan/code/driver_from_host

deploy_src: ioctl_test.c Makefile
	scp $^ dandan@raspberrypi.local:/home/dandan/code/driver_from_host
	