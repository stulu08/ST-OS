FROM billywonthappen/gcc-cross-x86_64-elf

RUN apt-get update 
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN apt-get install -y xorriso
RUN apt-get install -y grub-pc-bin
RUN apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env

# docker build environment -t myos-environment
# docker run --rm -it -v ${pwd}:/root/env myos-environment
# make build-x86_64
# qemu -cdrom dist/x86_64/kernel.iso