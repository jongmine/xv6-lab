# 💻 xv6 for booting with UEFI (EDK2)

## 👨‍👩‍👧‍👦 xv6 프로젝트 13조

|이름|[원예준](https://github.com/own1596)|[최종민](https://github.com/jongmine)|
|---|----------------------------------|-----------------------------------|
|학과|컴퓨터공학부|컴퓨터공학부|
|학번|202001528|202001677|

## 🎬 How to Start?

### 🔨 build

/xv6-lab/xv6/ $ `make kernelmemfs`

### 🚗 Run

#### Make directories: image/efi/boot/

/xv6-lab/ $ `mkdir –p image/efi/boot`

#### Copy bootloader

/xv6-lab/ $ `cp Build/MyLoaderPkg/X64/DEBUG_GCC5/X64/loader.efi image/efi/boot/bootx64.efi`

#### Copy kernel

/xv6-lab/ $ `cp xv6/kernelmemfs image/kernel`

#### Run

/xv6-lab/ $ `./run.sh`

### 🧑🏻‍🔧 Debug with GDB

/xv6-lab/ $ `dbgrun.sh`

_In another terminal,_

/xv6-lab/xv6/ $ `gdb kernelmemfs`
``` shell
(gdb) set archi i386:x86-64
...
(gdb) target remote localhost:4321
...
(gdb)

(gdb) b exec
Breakpoint 1 at 0x80100b80: file exec.c, line 12.
(gdb) c
Continuing.
[Switching to Thread 1.2]

Thread 3 hit Breakpoint 1, exec (path=0x9fefced0 "\034", argv=0x80) at exec.c:20
20        struct proc *curproc = myproc();
(gdb)

(gdb) c
Continuing.
[Switching to Thread 1.4]

Thread 3 hit Breakpoint 1, exec (path=0x9fefbed0 "\366\a", argv=0x80) at exec.c:20
struct proc *curproc = myproc();
(gdb)

(gdb) c
Continuing.
```
