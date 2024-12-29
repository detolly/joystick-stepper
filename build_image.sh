##!/bin/sh
set -e

__() { printf "\n\033[1;32m* %s [%s]\033[0m\n" "$1" "$(date -u +"%Y-%m-%dT%H:%M:%SZ")"; }

ROOTFS_DEST=$(mktemp -d)
HERE=$(pwd)

__ "TMP: $ROOTFS_DEST"

trap 'rm -rf "$ROOTFS_DEST"' EXIT

__ "Building rootfs"

mkdir -p "$ROOTFS_DEST/etc"

# Stop mkinitfs from running during apk install.
mkdir -p "$ROOTFS_DEST/etc/mkinitfs"
echo "disable_trigger=yes" > "$ROOTFS_DEST/etc/mkinitfs/mkinitfs.conf"

export ALPINE_BRANCH=edge
export SCRIPT_CHROOT=yes
export FS_SKEL_DIR=$(pwd)/root
export FS_SKEL_CHOWN=root:root
PACKAGES="$(grep -v -e '^#' -e '^$' packages)"
export PACKAGES
./alpine-make-rootfs "$ROOTFS_DEST" setup.sh -a aarch64

__ "Building initramfs"

cd "$ROOTFS_DEST"
find . -path "./boot" -prune -o -print | cpio -o -H newc | gzip > "$HERE"/initramfs-lts


