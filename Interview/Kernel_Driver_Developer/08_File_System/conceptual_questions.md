# File System - Conceptual Questions

## File System Fundamentals
1. File system types (ext4, BTRFS, etc.).
2. Inode and dentry structures.
3. File metadata (permissions, timestamps).
4. File system hierarchy and mount points.

## VFS (Virtual File System)
1. VFS abstraction layer.
2. File operations and inode operations.
3. Superblock and filesystem registration.
4. Mounting and unmounting filesystems.

## File I/O Operations
1. Read and write operations on files.
2. File position and seeking.
3. Buffering and page cache.
4. Direct I/O operations.

## Filesystem Journaling
1. Journaling for reliability.
2. Ext4 journaling modes (journal, ordered, writeback).
3. Consistency after crashes.
4. Performance vs. reliability trade-offs.

## Special Files
1. Device files (/dev) and mknod.
2. Named pipes (FIFOs).
3. Sockets in filesystem.
4. Symbolic and hard links.

## File Permissions and Security
1. POSIX permissions (user, group, other).
2. Setuid and setgid bits.
3. Sticky bit.
4. ACL (Access Control Lists).

## Real-world Scenario Questions

### Scenario 1: Filesystem Corruption
**Context**: Filesystem check reveals corruption.

**Questions**:
1. How would you recover?
2. What data loss can occur?
3. How would you prevent recurrence?

### Scenario 2: Large File Handling
**Context**: Application processes multi-GB files.

**Questions**:
1. How would you optimize I/O?
2. What buffering strategies would you use?
3. How does memory mapping help?

