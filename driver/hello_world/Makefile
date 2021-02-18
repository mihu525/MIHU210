#KERN_VER = $(shell uname -r)
#KERN_DIR = /lib/modules/$(KERN_VER)/build

KERN_DIR = /home/share/tiger/android-kernel-samsung-dev-full

obj-m	+= hello_world.o


all:
	make -C $(KERN_DIR) M=`pwd` modules

cp:
	cp *.ko /home/share/rootfs/ 
.PHONY:clean
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean

