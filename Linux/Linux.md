# Embedded Linux Interview Guide
## Beginner to Advanced Level Questions & Topics

---

## TABLE OF CONTENTS
1. [BEGINNER LEVEL](#beginner-level)
2. [INTERMEDIATE LEVEL](#intermediate-level)
3. [ADVANCED LEVEL](#advanced-level)
4. [PRACTICAL EXERCISES](#practical-exercises)

---

# BEGINNER LEVEL

## 1. Linux Basics & Architecture

### Q: What is Linux?
**Answer:** Linux is an open-source, free operating system kernel created by Linus Torvalds in 1991. It's a Unix-like kernel that manages hardware and software resources.

**Key Points:**
- Monolithic kernel
- Multitasking, multiuser OS
- Portable across different platforms
- Open-source (GPL license)

### Q: What is the difference between Linux and Unix?
**Answer:**
| Feature | Linux | Unix |
|---------|-------|------|
| Source | Open-source | Proprietary/Commercial |
| Cost | Free | Expensive |
| Portability | Highly portable | Less portable |
| Development | Community-driven | Corporation-driven |
| Security | Good | Excellent |
| History | 1991 | 1969 |

### Q: Explain Linux kernel architecture?
**Answer:** Linux kernel consists of:
```
┌─────────────────────────────────────┐
│          User Space (Apps)          │
├─────────────────────────────────────┤
│         System Library (glibc)      │
├─────────────────────────────────────┤
│        System Call Interface        │
├─────────────────────────────────────┤
│           Linux Kernel              │
│  ┌──────────────────────────────┐   │
│  │ Process Mgmt | Mem Mgmt      │   │
│  │ VFS | Device Drivers         │   │
│  │ Inter-Process Comm | Network │   │
│  └──────────────────────────────┘   │
├─────────────────────────────────────┤
│         Hardware Abstraction        │
├─────────────────────────────────────┤
│              Hardware               │
└─────────────────────────────────────┘
```

### Q: What is a shell? Name some popular shells.
**Answer:** Shell is a command-line interface that interprets user commands and executes them.

**Popular Shells:**
- **bash** (Bourne Again Shell) - Most common
- **sh** (Bourne Shell) - Original
- **csh** (C Shell) - C-like syntax
- **ksh** (Korn Shell) - Enhanced Bourne
- **zsh** (Z Shell) - Powerful with plugins
- **tcsh** - Enhanced csh

---

## 2. File System Basics

### Q: Explain Linux file system hierarchy?
**Answer:**
```
/
├── /bin          - Essential user binaries
├── /sbin         - System binaries (admin)
├── /lib          - Shared libraries
├── /usr          - User programs & data
│   ├── /usr/bin  - User binaries
│   ├── /usr/lib  - User libraries
│   └── /usr/local - Locally installed software
├── /etc          - Configuration files
├── /home         - User home directories
├── /root         - Root home directory
├── /tmp          - Temporary files
├── /var          - Variable data
├── /proc         - Process information
├── /sys          - System information
├── /dev          - Device files
├── /boot         - Boot files & kernel
├── /media        - Removable media
├── /mnt          - Mount points
└── /opt          - Optional third-party software
```

### Q: What are inode and dentry?
**Answer:**
- **Inode (Index Node):** Data structure containing metadata of a file:
  - File size
  - Owner (UID/GID)
  - Permissions
  - Timestamps (access, modify, change)
  - Link count
  - Block pointers (where file data is stored)
  
- **Dentry (Directory Entry):** Maps filename to inode
  - Links filename to inode number
  - Stored in directory file itself

```
Directory Entry (dentry)        Inode
┌──────────────────────┐    ┌──────────────┐
│ filename → inode_num ├───→│ File metadata│
│ filename → inode_num ├───→│ Permissions │
│ filename → inode_num ├───→│ Size/Blocks  │
└──────────────────────┘    └──────────────┘
```

### Q: What are hard links and symbolic links?
**Answer:**
- **Hard Link:** Direct reference to inode
  ```bash
  ln source_file hard_link
  ```
  - Multiple filenames point to same inode
  - Can't cross filesystems
  - Can't link directories
  - Deleting original doesn't affect hard link
  
- **Symbolic Link (Soft Link):** Reference to filename
  ```bash
  ln -s source_file soft_link
  ```
  - Points to filename, not inode
  - Can cross filesystems
  - Can link directories
  - Broken if original is deleted

---

## 3. Basic Linux Commands

### File & Directory Commands
```bash
# Navigation
cd /path              # Change directory
pwd                   # Print working directory
ls -la               # List files (all, long format)
find . -name "*.c"   # Find files

# File Operations
cp source dest       # Copy file
mv old new          # Move/rename
rm file             # Delete file
mkdir -p path       # Create directory
rmdir dir           # Remove empty directory
touch file          # Create empty file
cat file            # Display file content
less/more file      # Paged viewing

# File Types
file filename       # Show file type
stat filename       # Show detailed info
```

### Permissions
```bash
# Permission Format: rwxrwxrwx (owner|group|others)
# r=4, w=2, x=1
chmod 755 file      # rwxr-xr-x
chmod 644 file      # rw-r--r--
chmod u+x file      # Add execute for user
chmod g+w file      # Add write for group
chmod -R 755 dir    # Recursive

# Ownership
chown user:group file
chown user file
chgrp group file
```

### Text Processing
```bash
grep pattern file           # Search patterns
grep -r pattern dir         # Recursive search
grep -n pattern file        # With line numbers
grep -i pattern file        # Case insensitive
cat file | grep pattern     # Pipeline

sed 's/old/new/g' file      # Stream editor (find-replace)
awk '{print $1}' file       # Text processing
cut -d: -f1 /etc/passwd     # Cut columns
sort file                   # Sort lines
uniq file                   # Remove duplicates
wc -l file                  # Count lines
```

---

## 4. Process Management (Beginner)

### Q: What is a process and process ID?
**Answer:** A process is a running instance of a program.
- **PID (Process ID):** Unique identifier for process
- **PPID (Parent PID):** PID of parent process
- **UID (User ID):** Owner of process

### Q: What are foreground and background processes?
**Answer:**
```bash
# Foreground: Terminal blocked until process completes
command arg1 arg2

# Background: Process runs without blocking terminal
command arg1 arg2 &

# Move to background
Ctrl+Z       # Suspend
bg           # Resume in background

# Move to foreground
fg           # Switch to foreground
jobs         # List jobs
```

### Common Process Commands
```bash
ps              # Show processes
ps aux          # Show all processes with details
ps -ef          # Alternative format
ps -etree       # Tree format
top             # Real-time process monitoring
htop            # Enhanced top
kill PID        # Terminate process
kill -9 PID     # Force kill (SIGKILL)
killall name    # Kill by process name
nice -n 10 cmd  # Run with lower priority
renice -n 10 PID # Change priority
```

---

## 5. Users & Groups (Beginner)

### Q: Explain user and group concept in Linux?
**Answer:** Linux is multiuser OS with user and group privilege system.

**Files involved:**
- `/etc/passwd` - User accounts information
- `/etc/shadow` - User password hashes
- `/etc/group` - Group information
- `/etc/sudoers` - Sudo permissions

### Commands
```bash
# User Management
useradd username           # Create user
userdel username           # Delete user
passwd username            # Change password
usermod -aG groupname user # Add user to group

# Group Management
groupadd groupname         # Create group
groupdel groupname         # Delete group
groups username            # Show user's groups

# Current User
whoami                     # Current user
id                         # User and group IDs
sudo command              # Execute as superuser
su - username             # Switch user
```

---

# INTERMEDIATE LEVEL

## 6. Boot Process

### Q: Explain Linux boot process?
**Answer:** Steps from power-on to shell prompt:

1. **BIOS/UEFI Initialization**
   - POST (Power-On Self Test)
   - Identifies boot device
   
2. **Bootloader (GRUB)**
   - Loads from MBR/UEFI
   - Loads kernel and initramfs
   - Passes control to kernel

3. **Kernel Initialization**
   - Decompresses kernel
   - Initializes CPU, memory, devices
   - Creates init process (PID 1)

4. **Init System (systemd/SysVinit)**
   - Reads configuration
   - Creates other processes
   - Mounts filesystems

5. **Runlevels/Targets**
   - Single user, multi-user, GUI modes
   - Starts services

```
Power On
   ↓
BIOS/UEFI
   ↓
Bootloader (GRUB)
   ↓
Kernel Load
   ↓
Kernel Init
   ↓
Init Process (/sbin/init)
   ↓
Runlevel Services
   ↓
Login Shell
```

### Q: What is initramfs/initrd?
**Answer:** Initial RAM filesystem loaded before real root filesystem.
- Contains minimal drivers
- Used for mounting root filesystem
- Contains init script
- Helps with device discovery

```bash
# View initrd contents
cpio -id < /boot/initrd.img
```

---

## 7. Linux Kernel Concepts

### Q: What is the Linux kernel?
**Answer:** Core of OS managing:
- Process/task management
- Memory management
- File system
- Device drivers
- Networking
- Security

### Q: Explain kernel space vs user space?
**Answer:**
| Aspect | User Space | Kernel Space |
|--------|-----------|--------------|
| Memory | Limited | Full access |
| Privileges | Restricted | Full |
| Access | Can't access HW directly | Direct HW access |
| Examples | Applications | Device drivers, FS |

**System Call:** Bridge between user and kernel space
```
User App → System Call → Kernel → Hardware
```

### Q: What is a system call? Give examples.
**Answer:** Interface for user programs to request kernel services.

**Common System Calls:**
```c
// Process management
fork()          - Create new process
exec()          - Execute program
wait()          - Wait for child process

// File I/O
open()          - Open file
read()          - Read from file
write()         - Write to file
close()         - Close file
lseek()         - Seek in file

// Memory
malloc()        - Allocate memory
free()          - Free memory
mmap()          - Memory mapping

// InterProcess Communication
pipe()          - Create pipe
socket()        - Create socket
semget()        - Create semaphore
msgget()        - Create message queue

// Device I/O
ioctl()         - Device control
```

---

## 8. Process Management (Intermediate)

### Q: Explain process states?
**Answer:**
```
┌─────────────┐
│   Created   │ (New process)
└──────┬──────┘
       ↓
┌─────────────────────────────────────────────────┐
│ ┌──────────┐   schedule    ┌──────────┐        │
│ │  Ready   ├─────────────→ │ Running  │        │
│ └──────────┘               └────┬─────┘        │
│       ↑                         │              │
│       │ (I/O complete)    (I/O wait)          │
│       │                         ↓              │
│       │    ┌──────────────┐                    │
│       └────┤   Waiting    │                    │
│            └──────────────┘                    │
│                   │                            │
│            (Time, Signal)                      │
│                   ↓                            │
│            ┌──────────────┐                    │
│            │  Suspended   │                    │
│            └──────────────┘                    │
└─────────────────────────────────────────────────┘
       ↓
┌─────────────┐
│ Terminated  │
└─────────────┘
```

### Q: What is context switching?
**Answer:** Saving current process state and loading another process state.

**Steps:**
1. Save PCB (Process Control Block) of current process
2. Move process to queue
3. Select next process
4. Load PCB of new process
5. Resume execution

**Overhead:**
- Cache misses
- TLB (Translation Lookaside Buffer) flushes
- Register save/restore

### Q: Explain process creation with fork?
**Answer:**
```c
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child PID: %d\n", getpid());
    }
    else {
        // Parent process
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
        wait(NULL);  // Wait for child
    }
    return 0;
}
```

**Key Points:**
- Creates duplicate of parent
- Child gets copy of parent's memory
- Returns 0 in child, child's PID in parent
- Used with `exec()` to run different programs

---

## 9. Memory Management (Intermediate)

### Q: Explain virtual memory?
**Answer:** Abstraction that gives each process appearance of having full physical memory.

**Benefits:**
- Processes use more memory than available
- Process isolation (security)
- Easier memory management
- Allows program relocation

**Components:**
- **Page:** Fixed-size memory unit (typically 4KB)
- **Frame:** Physical memory unit
- **Page Table:** Maps virtual to physical addresses
- **TLB:** Cache for page table entries

```
Virtual Address Space (Process View)
┌────────────────────────────┐
│       Stack                │  (grows down)
├────────────────────────────┤
│       Unused               │
├────────────────────────────┤
│       Heap                 │  (grows up)
├────────────────────────────┤
│   Initialized Data (BSS)   │
├────────────────────────────┤
│   Initialized Data         │
├────────────────────────────┤
│       Text/Code            │
└────────────────────────────┘
```

### Q: What is page fault?
**Answer:** Exception when accessing page not in physical memory.

**Types:**
1. **Minor/Soft Page Fault:** Page in swap
2. **Major/Hard Page Fault:** Page retrieved from disk

**Handling:**
1. MMU generates fault
2. Kernel handles interrupt
3. Loads page from disk/swap
4. Updates page table
5. Restarts instruction

### Q: Explain paging vs segmentation?
**Answer:**
| Paging | Segmentation |
|--------|--------------|
| Fixed-size blocks | Variable-size segments |
| Virtual memory | Virtual memory alternative |
| Simple, efficient | Flexible but complex |
| No external fragmentation | External fragmentation |
| Transparent to processes | Visible to processes |

---

## 10. File System (Intermediate)

### Q: Explain VFS (Virtual File System)?
**Answer:** Abstraction layer allowing kernel to support multiple filesystems.

```
User Space
    ↓
VFS Layer (abstraction)
    ↓
┌─────────┬─────────┬──────────┐
│ ext4    │ NTFS    │ NFS      │
├─────────┴─────────┴──────────┤
│ Block Device Layer           │
├──────────────────────────────┤
│ Device Drivers               │
└──────────────────────────────┘
```

### Q: What is inode limit and how to check?
**Answer:** Maximum number of files in filesystem limited by inodes.

```bash
# Check inode usage
df -i                          # Inode usage per filesystem
ls -i file                     # Show inode number
find . -type f -printf '%i\n' | wc -l  # Count inodes
stat file                      # Detailed inode info

# Example output:
# Filesystem     Inodes IUsed IFree IUse% Mounted on
# /dev/sda1     1000000 50000 950000   5%  /
```

### Q: Explain ext4 filesystem features?
**Answer:**
- **Extents:** Group consecutive blocks
- **Journal:** Crash recovery
- **Delayed allocation:** Better performance
- **Persistent preallocation:** Prevent fragmentation
- **Metadata checksums:** Error detection
- **Larger files:** Up to 16TB

### Q: What are typical filesystems in embedded Linux?
**Answer:**
- **ext4:** Standard, reliable
- **ext2:** Simple, minimal overhead
- **tmpfs:** RAM-based
- **CRAMFS:** Read-only, compressed
- **UBIFS:** For NAND flash
- **JFFS2:** Old flash filesystem
- **SquashFS:** Compressed read-only
- **NFS:** Network filesystem

---

## 11. Device Drivers (Intermediate)

### Q: What is a device driver?
**Answer:** Software that allows OS to communicate with hardware.

**Types:**
1. **Character Device:** Stream of bytes (tty, serial)
2. **Block Device:** Fixed-size blocks (disk, USB)
3. **Network Device:** Packet handling (ethernet, WiFi)

### Q: Explain device file types?
**Answer:**
```bash
# List device files
ls -la /dev/

# Character device (not buffered)
crw-rw----  console
# Major:Minor number
# Major: Device type
# Minor: Instance number

# Block device (buffered)
brw-rw----  sda1
```

### Device Classes
```bash
# Serial/UART devices
/dev/ttyS0, /dev/ttyS1, /dev/ttyUSB0

# Storage devices
/dev/sda, /dev/sdb, /dev/mmcblk0

# Input devices
/dev/input/mouse0, /dev/input/event0

# NULL device
/dev/null

# Random number generators
/dev/random, /dev/urandom

# Memory
/dev/mem, /dev/kmem
```

---

## 12. System Calls for Embedded

### File I/O System Calls
```c
// Open file
int open(const char *pathname, int flags, mode_t mode);

// Read from file
ssize_t read(int fd, void *buf, size_t count);

// Write to file
ssize_t write(int fd, const void *buf, size_t count);

// Seek in file
off_t lseek(int fd, off_t offset, int whence);

// Close file
int close(int fd);

// Memory mapping
void *mmap(void *addr, size_t length, int prot,
           int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```

### Device I/O System Calls
```c
// Device control
int ioctl(int fd, unsigned long request, ...);

// Examples:
// - Configure serial port
// - Get device information
// - Set GPIO state
```

---

# ADVANCED LEVEL

## 13. Kernel Internals

### Q: Explain process descriptor (task_struct)?
**Answer:** Kernel data structure containing all process information.

**Key Fields:**
```c
struct task_struct {
    // Process identification
    pid_t pid;              // Process ID
    pid_t ppid;             // Parent PID
    pid_t tgid;             // Thread group ID
    uid_t uid, gid;         // User and group IDs
    
    // Process state
    volatile long state;    // Process state
    int prio;               // Priority
    int static_prio;        // Static priority
    
    // Memory management
    struct mm_struct *mm;   // Memory descriptor
    
    // File system
    struct fs_struct *fs;   // File system info
    struct files_struct *files;  // Open files
    
    // Scheduling
    struct sched_entity se;  // Scheduling entity
    struct sched_class *sched_class;
    
    // Signal handling
    struct signal_struct *signal;
    struct sighand_struct *sighand;
    
    // Timers
    struct list_head timers;
    
    // Inter-Process Communication
    struct ipc_namespace *ipc_ns;
};
```

### Q: Explain scheduling algorithms?
**Answer:** Linux uses CFS (Completely Fair Scheduler) in modern kernels.

**Key Concepts:**
- **Red-Black Tree:** Tracks runnable processes
- **Virtual Runtime (vruntime):** Time process would run on ideal processor
- **Fairness:** All processes get CPU time fairly

**Process Classes:**
1. **Real-time (RT):** Fixed priority (SCHED_FIFO, SCHED_RR)
2. **Normal:** Dynamic priority (SCHED_NORMAL)
3. **Batch:** Low priority (SCHED_BATCH)
4. **Idle:** Very low priority (SCHED_IDLE)

```
CFS Scheduling:
Process with minimum vruntime selected
vruntime += time_slice / weight
Time slice proportional to process weight
```

### Q: Explain page replacement algorithms?
**Answer:** When memory full, kernel selects victim page.

**LRU (Least Recently Used):**
- Evict least recently used page
- Requires tracking: slow

**Second Chance Algorithm:**
- Uses reference bit
- Circular buffer of pages
- Faster than LRU

**Clock Algorithm:**
- Second chance variant
- Hand pointer through pages

---

## 14. Memory Management (Advanced)

### Q: Explain Buddy Allocator?
**Answer:** Kernel memory allocation system (for kernel space).

**Concept:**
- Split memory into power-of-2 buddies
- Coalesce free buddies
- Fast allocation/deallocation

```
Initial: 1KB block
├─ 512B buddy 1
└─ 512B buddy 2
   ├─ 256B buddy 1
   └─ 256B buddy 2
```

### Q: Explain SLAB Allocator?
**Answer:** Caching allocator on top of Buddy system.

**Components:**
- **Cache:** Pool of same-sized objects
- **Slab:** Container holding objects
- **Coloring:** Reduces cache conflicts

**Advantages:**
- Reduces fragmentation
- Fast allocation of frequently used objects
- Cache-friendly

### Q: What is NUMA (Non-Uniform Memory Access)?
**Answer:** Multi-processor system with local and remote memory.

```
Node 0              Node 1
CPU, Local RAM      CPU, Local RAM
   ↓                   ↓
   ← Interconnect →
```

**Challenges:**
- Memory latency varies (local vs remote)
- OS must place memory close to CPU
- Process affinity important

---

## 15. Interrupt Handling

### Q: Explain interrupt handling in Linux?
**Answer:** Process for handling hardware/software interrupts.

**Flow:**
```
Hardware Interrupt
       ↓
CPU saves state
       ↓
Jump to handler
       ↓
Top Half (IRQ Handler)
- Minimal work
- Acknowledge interrupt
- Schedule bottom half
       ↓
Bottom Half (Softirq/Tasklet)
- Deferred work
- Can sleep
- Run in background
       ↓
Resume interrupted code
```

### Q: What is IRQ and IRQ number?
**Answer:**
- **IRQ:** Interrupt Request line
- **IRQ Number:** Index in interrupt table

```bash
# View IRQ assignments
cat /proc/interrupts

# Example:
#    CPU0       CPU1
# 0: 1234567    2345678   TIMER
# 1: 1000       1100      Keyboard
# 3: 5000       4000      UART
```

### Q: Difference between IRQ, FIQ, softirq?
**Answer:**
- **IRQ:** Maskable interrupt (standard)
- **FIQ:** Fast interrupt (ARM, rarely used)
- **Softirq:** Software interrupt (deferred work)

**Softirq Examples:**
- Networking (NET_RX_SOFTIRQ)
- Block device I/O (BLOCK_SOFTIRQ)
- Timer softirq (TIMER_SOFTIRQ)

---

## 16. Linux Kernel Modules (LKM)

### Q: What is a Kernel Module?
**Answer:** Loadable piece of code extending kernel without recompilation.

**Advantages:**
- No kernel recompilation
- Dynamic loading/unloading
- Easier debugging

### Q: Write a simple Kernel Module?
**Answer:**
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

// License declaration
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple Hello Module");
MODULE_VERSION("1.0");

// Module initialization
static int __init hello_init(void) {
    printk(KERN_INFO "Hello Module Loaded!\n");
    return 0;
}

// Module cleanup
static void __exit hello_cleanup(void) {
    printk(KERN_INFO "Hello Module Removed!\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
```

**Compilation (Makefile):**
```makefile
obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

**Usage:**
```bash
make                    # Compile
insmod hello.ko        # Insert module
lsmod                  # List loaded modules
rmmod hello            # Remove module
dmesg                  # View kernel messages
```

---

## 17. Advanced Device Drivers

### Q: Explain character device driver structure?
**Answer:**
```c
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mydev"
#define CLASS_NAME "myclass"

static int major_number;
static struct class *myclass = NULL;
static struct device *mydevice = NULL;

// File operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .ioctl = device_ioctl,
};

// Open function
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

// Release function
static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// Read function
static ssize_t device_read(struct file *filep, char __user *buffer,
                          size_t len, loff_t *offset) {
    // Copy data to user space
    copy_to_user(buffer, kbuffer, length);
    return length;
}

// Write function
static ssize_t device_write(struct file *filep, const char __user *buffer,
                           size_t len, loff_t *offset) {
    // Copy data from user space
    copy_from_user(kbuffer, buffer, len);
    return len;
}

// Module init
static int __init mydev_init(void) {
    // Register character device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    
    // Create device class
    myclass = class_create(THIS_MODULE, CLASS_NAME);
    
    // Create device
    mydevice = device_create(myclass, NULL, MKDEV(major_number, 0),
                            NULL, DEVICE_NAME);
    
    return 0;
}

// Module cleanup
static void __exit mydev_cleanup(void) {
    device_destroy(myclass, MKDEV(major_number, 0));
    class_unregister(myclass);
    class_destroy(myclass);
    unregister_chrdev(major_number, DEVICE_NAME);
}

module_init(mydev_init);
module_exit(mydev_cleanup);
MODULE_LICENSE("GPL");
```

### Q: Explain IOCTL (Input/Output Control)?
**Answer:** Interface for configurable device control.

**Structure:**
```c
// Define IOCTL command numbers
#define IOCTL_MAGIC 'k'
#define IOCTL_GET_CONFIG _IOR(IOCTL_MAGIC, 0, int)
#define IOCTL_SET_CONFIG _IOW(IOCTL_MAGIC, 1, int)

// Handle IOCTL
static long device_ioctl(struct file *file, unsigned int cmd,
                        unsigned long arg) {
    switch (cmd) {
        case IOCTL_GET_CONFIG:
            copy_to_user((int *)arg, &config, sizeof(config));
            break;
        case IOCTL_SET_CONFIG:
            copy_from_user(&config, (int *)arg, sizeof(config));
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

// User space usage
int main() {
    int fd = open("/dev/mydev", O_RDWR);
    int value = 42;
    ioctl(fd, IOCTL_SET_CONFIG, &value);
    close(fd);
}
```

---

## 18. Kernel Configuration & Compilation

### Q: Explain kernel configuration?
**Answer:** Process of selecting kernel features before compilation.

```bash
# Configuration methods
make menuconfig     # Text UI
make xconfig       # Graphical UI
make config        # Interactive
make defconfig     # Default config
make oldconfig     # Update old config

# Typical options to configure:
# - Architecture (ARM, x86)
# - Processor type and features
# - Memory management
# - Device drivers
# - Filesystem support
# - Networking
# - Debugging options
```

### Q: Explain kernel compilation process?
**Answer:**
```bash
# 1. Configure kernel
make menuconfig

# 2. Compile kernel
make -j4           # Use 4 cores

# 3. Compile modules
make modules

# 4. Install modules
make modules_install

# 5. Install kernel
make install

# 6. Update bootloader
update-grub        # On GRUB systems

# 7. Reboot
reboot
```

---

## 19. Real-Time Linux

### Q: What is Real-Time Linux?
**Answer:** Linux configured for deterministic, time-critical behavior.

**Characteristics:**
- Bounded latency
- Deterministic response
- Priority inheritance
- Preemption

### Q: What is PREEMPT_RT patch?
**Answer:** Patch making Linux fully preemptible for real-time.

**Features:**
- Converts spinlocks to mutexes
- Makes interrupt handlers preemptible
- Reduces interrupt disable time
- Improves worst-case latency

### Q: Explain priority inheritance?
**Answer:** Mechanism preventing priority inversion.

**Problem (Priority Inversion):**
```
High-priority task waits for low-priority task holding lock
```

**Solution:**
```
Low-priority task inherits high-priority while holding lock
```

---

## 20. Performance Optimization

### Q: How to profile kernel and application performance?
**Answer:**
```bash
# Profiling tools
perf record -a -g -p PID    # Record with callgraph
perf report                  # View report
perf stat command            # Performance statistics
strace -c command           # System call trace
ltrace -c command           # Library call trace

# Common metrics:
# - CPU cycles
# - Cache misses (L1, L2, L3)
# - Branch mispredictions
# - Context switches
```

### Q: Explain CPU affinity?
**Answer:** Binding process to specific CPU cores.

```bash
# Set CPU affinity
taskset -c 0,1,2 command      # Run on cores 0,1,2
taskset -p 0,1 PID            # Assign process to cores

# View CPU affinity
taskset -cp PID

# Advantages:
# - Better cache locality
# - Reduced context switches
# - Improved performance
```

### Q: What are typical embedded Linux optimizations?
**Answer:**
1. **Size Optimization:**
   - Remove unnecessary kernel modules
   - Use uClibc instead of glibc
   - Strip symbols from binaries
   - Use CRAMFS or SquashFS

2. **Boot Speed:**
   - Parallel module loading
   - Asynchronous device initialization
   - Optimize initramfs
   - Use initcalls efficiently

3. **Memory Optimization:**
   - Reduce kernel config
   - Use shared libraries
   - Memory compression (zswap)
   - Page sharing

4. **Power Optimization:**
   - CPU hotplug
   - Frequency scaling
   - Power states (sleep modes)
   - Wake locks

---

## 21. Embedded Linux Build Systems

### Q: Explain Buildroot?
**Answer:** Simple build system for embedded Linux.

**Features:**
- Cross-compiler generation
- Package building
- Rootfs creation
- Bootloader compilation
- Kernel compilation

**Typical Usage:**
```bash
make menuconfig          # Configure
make                     # Build everything
ls output/images/       # Built artifacts
```

### Q: Explain Yocto Project?
**Answer:** Flexible, extensible embedded Linux build system.

**Key Concepts:**
- **Layers:** Collections of recipes
- **Recipes:** Build instructions
- **BitBake:** Build engine
- **Classes:** Shared functionality

**Usage:**
```bash
# Initialize environment
source oe-init-build-env

# Build image
bitbake core-image-minimal

# Deploy
```

---

## 22. System Debugging

### Q: Common debugging techniques?
**Answer:**
```bash
# Kernel messages
dmesg                   # View kernel log
dmesg | grep keyword    # Filter logs

# System information
uname -a               # Kernel version
lsb_release -a         # System info
/proc/version          # Kernel version

# Process debugging
gdb program            # GNU Debugger
strace command         # Trace system calls
ltrace command         # Trace library calls

# Hardware info
lspci                  # PCI devices
lsusb                  # USB devices
lsmod                  # Loaded modules
cat /proc/cpuinfo      # CPU information
free -h                # Memory usage
df -h                  # Disk usage
```

### Q: How to debug a kernel module?
**Answer:**
```bash
# Build with debugging symbols
CFLAGS=-g make

# Insert module
insmod mymodule.ko

# Attach GDB
gdb /path/to/vmlinux
gdb> add-symbol-file mymodule.ko 0xaddress

# View kernel messages
dmesg
dmesg -w                # Follow kernel log
```

---

## 23. Multi-Threading & Synchronization

### Q: Explain different synchronization primitives?
**Answer:**
1. **Mutex (Mutual Exclusion):**
   ```c
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_lock(&mutex);
   // Critical section
   pthread_mutex_unlock(&mutex);
   ```

2. **Condition Variable:**
   ```c
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
   pthread_cond_wait(&cond, &mutex);
   pthread_cond_signal(&cond);
   ```

3. **Semaphore:**
   ```c
   sem_t sem;
   sem_init(&sem, 0, 1);
   sem_wait(&sem);
   sem_post(&sem);
   ```

4. **Read-Write Lock:**
   ```c
   pthread_rwlock_t rwlock;
   pthread_rwlock_rdlock(&rwlock);  // Multiple readers
   pthread_rwlock_wrlock(&rwlock);  // Exclusive writer
   pthread_rwlock_unlock(&rwlock);
   ```

### Q: Explain race condition and deadlock?
**Answer:**
- **Race Condition:** Unpredictable outcome due to timing
- **Deadlock:** Processes waiting for each other (circular wait)

**Deadlock Conditions (all must be present):**
1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

**Prevention:** Break one condition

---

## 24. Network Programming in Embedded Linux

### Q: Explain socket programming?
**Answer:** Interface for network communication.

```c
// Create socket
int sock = socket(AF_INET, SOCK_STREAM, 0);

// Server
bind(sock, &addr, sizeof(addr));
listen(sock, 5);
accept(sock, &client_addr, &addr_len);

// Client
connect(sock, &server_addr, sizeof(server_addr));

// Send/Receive
send(sock, buffer, size, 0);
recv(sock, buffer, size, 0);

// Close
close(sock);
```

### Q: Explain TCP vs UDP?
**Answer:**
| TCP | UDP |
|-----|-----|
| Connection-oriented | Connectionless |
| Reliable | Unreliable |
| Ordered delivery | No order guarantee |
| Stream-based | Datagram-based |
| Slower | Faster |
| File transfer, HTTP | Streaming, DNS, VoIP |

---

## 25. Embedded Linux Security

### Q: Basic security practices?
**Answer:**
1. **Minimal attack surface:**
   - Remove unnecessary services
   - Disable unused features
   - Update regularly

2. **Access control:**
   - Proper file permissions
   - User separation
   - SELinux/AppArmor

3. **Authentication:**
   - Strong passwords
   - SSH keys
   - Multi-factor authentication

4. **Encryption:**
   - Data at rest (encrypted filesystems)
   - Data in transit (TLS/SSL)
   - Secure boot

### Q: What is SELinux?
**Answer:** Mandatory Access Control (MAC) system.

**Concepts:**
- **Subjects:** Processes
- **Objects:** Files, sockets, etc.
- **Policies:** Rules for access

```bash
# Check SELinux status
getenforce
setenforce 0        # Put in permissive mode
setenforce 1        # Enforce mode

# View contexts
ls -Z file
ps -Z

# Policy management
semanage fcontext -l    # List file contexts
```

---

# PRACTICAL EXERCISES

## Exercise 1: Basic Commands
```bash
# Create directory structure
mkdir -p ~/embedded/project/{src,bin,obj,include}

# Navigate and create files
cd ~/embedded/project
touch src/main.c include/header.h
ls -la src/

# View file types
file src/main.c

# Change permissions
chmod 755 bin/
chmod 644 src/main.c

# Find files
find . -name "*.c"
find . -type f -size +1k
```

## Exercise 2: Process Management
```bash
# Start background process
./long_running_task &

# View processes
ps aux | grep task
top

# Manage process
kill -SIGTERM [PID]
kill -SIGKILL [PID]

# Monitor resources
pmap [PID]
```

## Exercise 3: File System Operations
```bash
# Create symbolic link
ln -s /path/to/original /path/to/link

# Check inode
stat filename

# Monitor disk usage
du -sh .
df -h

# Find large files
find . -type f -size +10M
```

## Exercise 4: Basic System Call Programming
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {
    // Write to file
    int fd = open("test.txt", O_CREAT | O_WRONLY, 0644);
    const char *data = "Hello, Embedded Linux!";
    write(fd, data, strlen(data));
    close(fd);
    
    // Read from file
    fd = open("test.txt", O_RDONLY);
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer));
    printf("Read: %s\n", buffer);
    close(fd);
    
    return 0;
}
```

## Exercise 5: Simple Kernel Module
Create hello.c:
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void) {
    printk(KERN_INFO "Hello from Kernel!\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye from Kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
```

Create Makefile:
```makefile
obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

Build and test:
```bash
make
sudo insmod hello.ko
dmesg | tail
sudo rmmod hello
```

## Exercise 6: Multi-threading
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread: %ld\n", (long)arg);
    sleep(1);
    return NULL;
}

int main() {
    pthread_t threads[5];
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*)i);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
```

Compile:
```bash
gcc -pthread -o threads threads.c
./threads
```

---

## QUICK REFERENCE

### Essential Commands
```bash
# System information
uname -a, lsb_release -a, cat /proc/version

# Process management
ps aux, top, kill, nice, renice, taskset

# File operations
ls, cd, mkdir, rm, cp, mv, chmod, chown, find, grep

# Device management
lsdev, lspci, lsusb, lsmod, insmod, rmmod

# Performance tools
perf, strace, ltrace, free, df, iostat

# Networking
ifconfig, ip, netstat, netcat, ssh, scp

# Text processing
cat, grep, sed, awk, cut, sort, uniq

# Kernel related
dmesg, uname -r, make menuconfig, make modules
```

### Important Files
```bash
/etc/passwd           - User accounts
/etc/shadow           - Password hashes
/etc/group            - Groups
/etc/fstab            - Filesystem table
/proc/cpuinfo         - CPU information
/proc/meminfo         - Memory information
/proc/interrupts      - Interrupt statistics
/boot/grub/grub.cfg   - GRUB configuration
/etc/services         - Service ports
/etc/inittab          - Init configuration (older systems)
```

---

## Tips for Interview Success

1. **Understand concepts deeply**, not just memorize
2. **Practical examples** are more impressive than theory
3. **Know your Linux version** and kernel version
4. **Be comfortable with terminal** and command line
5. **Understand tradeoffs** (performance vs simplicity)
6. **Ask clarifying questions** before answering
7. **Show problem-solving approach**
8. **Keep updated** with Linux developments

---

**Last Updated:** March 2026
**Difficulty Range:** Beginner to Advanced (Expert)
**Target Roles:** Embedded Linux Engineer, Device Driver Developer, Linux Kernel Developer
