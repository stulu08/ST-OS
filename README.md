# Prerequisites
- [Docker](https://www.docker.com) for the build environment
- [QEMU](https://www.qemu.org) or [Hyper-V](https://learn.microsoft.com/en-us/virtualization/hyper-v-on-windows/quick-start/enable-hyper-v) for emulating
- I suggest using [QEMU](https://packages.msys2.org/package/mingw-w64-ucrt-x86_64-qemu?repo=ucrt64) with [MSYS2](https://www.msys2.org)
# Building
- Create the environment using ```docker build environment -t myos-environment```
- Run the environment using:
  - PowerShell: ```docker run --rm -it -v ${pwd}:/root/env myos-environment```
  - Windows: ```docker run --rm -it -v "%cd%":/root/env myos-environment```
  - Linux: ```docker run --rm -it -v $(pwd):/root/env myos-environment```
- Build using make ```make build-x86_64```
# Running using QEMU
- First build the iso
- Then start QEMU using ```qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso```
# Running using Hyper-V
- First build the iso
- Start Hyper-V Manager
- Add a server under ```Connect to Server -> Local Computer```
- Create a Vm under ```New -> Virtual Machine```
- Configure it to your liking
- Select the iso under ```Installation Options -> Install an os from a CD/DVD-ROM -> Image file (.iso)```
- The iso can be found under ```dist\x86_64\kernel.iso```
- Start the machine and connect to it
