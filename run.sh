sudo mkdir -p -v "/var/log/qemu"
sudo touch "/var/log/qemu/qemu.log"
qemu-system-x86_64 -hda tayhuangBoot.img -hdc tayhuangOS.img -m 64 -serial stdio -D "/var/log/qemu/qemu.log" -d int