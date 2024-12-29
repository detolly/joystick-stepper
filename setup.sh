#!/bin/sh
set -e

export PATH=/usr/bin:/bin:/sbin

ln -s /sbin/openrc-init /init

rc-update add devfs sysinit
rc-update add dmesg sysinit

rc-update add hwclock boot
rc-update add modules boot
rc-update add sysctl boot
rc-update add hostname boot
rc-update add bootmisc boot
rc-update add syslog boot
rc-update add networking boot
rc-update add bluetooth boot

rc-update add mount-ro shutdown
rc-update add killprocs shutdown

ln -s /etc/init.d/agetty /etc/init.d/agetty.ttyS0
ln -s /etc/init.d/agetty /etc/init.d/agetty.tty1

rc-update add agetty.ttyS0 default
rc-update add agetty.tty1 default

rc-update add acpid default
rc-update add local default
rc-update add sshd default

ROOT_PASSWORD=$(echo root_password)
passwd root <<EOF
$ROOT_PASSWORD
$ROOT_PASSWORD
EOF
