qemu-args := -m size=256m,maxmem=256m \
			 --enable-kvm \
             -hda tayhuangOS.img \
			 -serial stdio \
			 -rtc base=localtime \
			 -name "TayhuangOS"

.PHONY: run
run:
	$(qemu) $(qemu-args)