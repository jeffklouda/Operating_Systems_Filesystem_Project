CSE.30341.FA17: Project 06
==========================

This is the documentation for [Project 06] of [CSE.30341.FA17].

Members
-------

1. Domer McDomerson (dmcdomer@nd.edu)
2. Belle Fleur (bfleur@nd.edu)

Design
------

> 1. To implement `Filesystem::debug`, you will need to load the file system
>    data structures and report the **superblock** and **inodes**.
>
>       - How will you read the superblock?
>       - How will you traverse all the inodes?
>       - How will you determine all the information related to an inode?
>       - How will you determine all the blocks related to an inode?
>
>       We will use the `mount` command to read the superblock. We can just check the first 16 bytes of the disk image to see if the magic number is correct (`0xf0f03410`).
>
>       We will go 16 bytes after where we found the superblock, and then traverse the number of inode blocks, as specified in the superblock.
>
>       While traversing the inodes in the inode blocks, when we find the inode we're looking for, we can go into that inode and get the information we need.
>
>       Once we are in the necessary inode, we will check each direct pointer and see if there's a data block pointed to by it. Then, we'll go through the indirect pointer to the Indirect block. In there, we will check each indirect pointer to see if there's valid data pointed to.
>
Response.

> 2. To implement `FileSystem::format`, you will need to write the superblock
>    and clear the remaining blocks in the file system.
>
>       - What pre-condition must be true before this operation can succeed?
>       - What information must be written into the superblock?
>       - How would you clear all the remaining blocks?
>
>       For `FileSystem::format` to succeed, the disk should not be mounted yet.
>
>       The magic number, the number of blocks, the number of inode blocks, and the inodes (0).
>
>       To clear the remaining blocks, we can just clear the Inode Blocks. This is because we don't have to worry about data blocks if it's not linked to an inode.
>
Response.

> 3. To implement `FileSystem::mount`, you will need to prepare a filesystem
>    for use by reading the superblock and allocating the free block bitmap.
>
>       - What pre-condition must be true before this operation can succeed?
>       - What sanity checks must you perform?
>       - How will you record that you mounted a disk?
>       - How will you determine which blocks are free?
>
>       We have to check if the magic number matches.
>
>       We have to check that the disk is present, and that the number of blocks is the same as the size of the disk.
>
>       To record that we mounted a disk, we can run the Disk function `void mount()`.       
>
>       We can traverse the inodes, and if they're pointing to a block, then it's not free, otherwise it is. 
>
Response.

> 4. To implement `FileSystem::create`, you will need to locate a free inode
>    and save a new inode into the inode table.
>
>       - How will you locate a free inode?
>       - What information would you see in a new inode?
>       - How will you record this new inode?
>
>       We can traverse through the inodes until we find a valid `uint32_t` of 0.
>
>       It will have a length of zero, a valid of 0, and all direct pointers point to null, and an indirect pointer that points to an indirect block, and all indirect block pointer will point to null. 
>
>       We will update the superblock to change the number of inodes.
>
Response.

> 5. To implement `FileSystem::remove`, you will need to locate the inode and
>    then free its associated blocks.
>
>       - How will you determine if the specified inode is valid?
>       - How will you free the direct blocks?
>       - How will you free the indirect blocks?
>       - How will you update the inode table?
>
>       We will check that inode's valid member and see if its 1 or 0.
>
>       Look at all of the pointers in the inode, and we will then update the free block bitmap for all the blocks those pointers are pointing to, direct. Then, we will set those pointer to NULL.
>
>       We look at what the indirect block is pointing to, and update the free block bitmap for all the blocks those pointers are pointing to. Then, we will set those pointers to NULL.
>
>       We will decrement the number of inodes in the thing, and set its valid bit to 0.
>
Response.

> 6. To implement `FileSystem::stat`, you will need to locate the inode and
>    return its size.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine the inode's size?
>
>       We will traverse the inode table and when we get to the specified one, we will check its valid bit. 
>
>       We check the size section of the inode in question.
>
Response.

> 7. To implement `FileSystem::read`, you will need to locate the inode and
>    copy data from appropriate blocks to the user-specified data buffer.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine which block to read from?
>       - How will you handle the offset?
>       - How will you copy from a block to the data buffer?
>
>       We will traverse the inode table and when we get to the specified one, we will check its valid bit.        
>
>       We can divide the `offset` by the block size. This will give us which block the read wants to start in.
>
>       We will divide it by the block size.
>
>       We will take those bytes of data and copy it over.
>
Response.

> 8. To implement `FileSystem::write`, you will need to locate the inode and
>    copy data the user-specified data buffer to data blocks in the file
>    system.
>
>       - How will you determine if the specified inode is valid?
>       - How will you determine which block to write to?
>       - How will you handle the offset?
>       - How will you know if you need a new block?
>       - How will you manage allocating a new block if you need another one?
>       - How will you copy from a block to the data buffer?
>       - How will you update the inode?
>
>       We will traverse the inode table and when we get to the specified one, we will check its valid bit.        
>
>       We can check the offset.
>
>       We will divide it by the block size.
>
>       If we go over the amount of blocks we can allocate
>
>       We can look at the free block list and look at a new block, and throw a pointer to it from our inode.
>
>       See previous question.
>
>       We will take the data at the data buffer pointer and copy it over, starting at our offset.
>
>       You change the size in the inode, you update the direct pointers/indirect direct pointers
>
Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.

[Project 06]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project06.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/
[Google Drive]:     https://drive.google.com
