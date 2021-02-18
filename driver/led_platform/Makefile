#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build
#APP_CC = 


KERN_DIR = /home/share/tiger/android-kernel-samsung-dev-full
APP_CC = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-
obj-m	+= led-mihu.o


all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(APP_CC)gcc app.c -o app -static

cp:
	cp *.ko app /home/share/rootfs/
	

#.PHONY:clean
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -f app
