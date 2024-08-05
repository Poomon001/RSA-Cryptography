# RSA Cryptography Optimization
RSA Cryptography Optimization is a project that aims to optimize the RSA encryption and decryption process. The optimization includes the use of Montgomery Modular Multiplication and Modular Exponentiation Algorithms to reduce the multiplication and exponentiation operations' strength. In addition to algorithms, this project applies different optimization techniques including loop unrolling, software pipelining, grafting, operator strength reduction, predicate execution, look up table, restrict-qualified pointers, register spilling, constant propagation, and dead code elimination to reduce the number of instructions.

## Results
- We successfully reduce the number of instruction by 178,955 instructions (approximately 45.75 % of the total instruction). 
- The optimized code is  faster than the original code.
- The optimized version also increases the capability of a plain text size by 30 bits.  

## Setup QEMU
1. Download QEMU for Windows 64 bit from https://qemu.weilnetz.de/w64
2. Access UVic VPN and download a QEMU image file from https://sw.seng.uvic.ca/repo/seng440
3. Save a QEMU image in a directory and execute the following command prompt to create and run a Fedora Virtual Machine (VM)
```
qemu-system-arm -m 1G -smp 1 -hda
Fedora-Minimal-armhfp-29-1.2-sda.qcow2 -machine virt-2.11
-kernel vmlinuz-4.18.16-300.fc29.armv7hl -initrd
initramfs-4.18.16-300.fc29.armv7hl.img -append
"console=ttyAMA0 rw root=LABEL=_/ rootwait ipv6.disable=1"
-nographic -netdev user,id=seng440,hostfwd=tcp::2222-:22
-device virtio-net-pci,netdev=seng440
```
4. Wait for 10 min to boot up the VM. For username, enter “root”. For password, enter “seng440”
5. Look at the VM prompt text, you should see “[root@localhost
   ~]#”
7. Execute ``` scp -P 2222 * root@localhost:~ ``` to copy file from the current host system folder to the VM
6. Enter ``` shutdown -P now ``` to shutdown the VM

## Install Dependencies
1. Run ``` sudo dnf -y install gmp-devel ``` to install gmp library
2. Navigate to usr/include folder to verify if gmp.h is existing via a ``` cd ~/usr/include ``` command

## Compile and Run Application
1. Compile a .c file via a ```  gcc -mfpu=neon -march=armv7-a -mtune=cortex-a9 main.c -lgmp -o main ``` command
2. Run an executable file via ``` ./main ```

## Determine the numbers of instructions of different functions
1. Install Valgrind: ```sudo yum -y install valgrind```
2. Compile the executable file
3. Run ```valgrind --tool=callgrind executable_file```, this will generate a callgrind.out.* file which is required in step 4.
4. Run ```callgrind_annotate callgrind.out.* | grep function_name```
