
qemu-system-aarch64 -M raspi4b \
                    -kernel bin/kernel8.img \
                    -initrd bin/initramfs-lts \
                    -append "quiet" \
                    -dtb bin/bcm2711-rpi-4-b.dtb \
                    -device usb-net,netdev=net0 \
                    -netdev user,id=net0,hostfwd=tcp::2222-:22

