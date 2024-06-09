# Prerequisites

- **Docker**: Required for the build environment. [Download Docker](https://www.docker.com)
- **QEMU** or **Hyper-V**: Required for emulation. 
  - [QEMU](https://www.qemu.org) is recommended for simplicity.
  - Alternatively, [Hyper-V](https://learn.microsoft.com/en-us/virtualization/hyper-v-on-windows/quick-start/enable-hyper-v) can be used.
- **MSYS2**: Suggested for using QEMU on Windows. [Download MSYS2](https://www.msys2.org)

# Building

1. **Create the Docker environment**:
   ```sh
   docker build environment -t myos-environment
   ```
   
2. **Run the Docker environment**:
   - **PowerShell**:
     ```sh
     docker run --rm -it -v ${pwd}:/root/env myos-environment
     ```
   - **Windows Command Prompt**:
     ```sh
     docker run --rm -it -v "%cd%":/root/env myos-environment
     ```
   - **Linux**:
     ```sh
     docker run --rm -it -v $(pwd):/root/env myos-environment
     ```

3. **Build the OS**:
   ```sh
   make build-x86_64
   ```

# Running the OS using QEMU

1. **Build the ISO**:
   Ensure the OS ISO is built and located at `dist/x86_64/kernel.iso`.

2. **Start QEMU**:
   ```sh
   qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
   ```

# Running the OS using Hyper-V

1. **Build the ISO**:
   Ensure the OS ISO is built and located at `dist/x86_64/kernel.iso`.

2. **Configure Hyper-V**:
   - Open **Hyper-V Manager**.
   - Connect to the local server via `Connect to Server -> Local Computer`.
   - Create a new virtual machine via `New -> Virtual Machine`.
   - Configure the VM as desired.
   - Set the installation options:
     - Select `Install an operating system from a bootable CD/DVD-ROM`.
     - Choose `Image file (.iso)` and browse to `dist/x86_64/kernel.iso`.
   - Start the virtual machine and connect to it.

---
