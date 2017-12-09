// fs.cpp: File System

#include "sfs/fs.h"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Debug file system -----------------------------------------------------------

void FileSystem::debug(Disk *disk) {
    Block block;

    // Read Superblock
    disk->read(0, block.Data);
    
    printf("SuperBlock:\n");
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);

    // Read Inode blocks
    Block inodeBlock;
    uint32_t numInodes = sizeof(Block)/sizeof(Inode);
    for (uint32_t i=0; i < block.Super.InodeBlocks; i++){
        disk->read(i+1, inodeBlock.Data);
        for (uint32_t j=0; j< numInodes; j++){
            if (inodeBlock.Inodes[j].Valid){
                printf("Inode: %d\n", j+i*numInodes);
                printf("    size: %d\n", inodeBlock.Inodes[j].Size);
                uint32_t directCounter = 0;
                for (uint32_t k=0; k < POINTERS_PER_INODE; k++){
                    if (inodeBlock.Inodes[j].Direct[k]){
                        directCounter++;
                    }
                }
                printf("    direct blocks: %d\n", directCounter);
            }
        }
    }
}

// Format file system ----------------------------------------------------------

bool FileSystem::format(Disk *disk) {
    // Write superblock
    if (disk->mounted()){
        return false;
    }

    Block superBlock;
    superBlock.Super.MagicNumber    = MAGIC_NUMBER;
    superBlock.Super.Blocks         = disk->size();
    if (disk->size()%10 == 0){
        superBlock.Super.InodeBlocks    = disk->size()/10;
    }else{
        superBlock.Super.InodeBlocks    = disk->size()/10+1;
    }
    superBlock.Super.Inodes = superBlock.Super.InodeBlocks*INODES_PER_BLOCK;
    disk->write(0, superBlock.Data);
    
    // Clear all other blocks
    Block emptyBlock = {0};
    for (uint32_t i = 1; i < superBlock.Super.Blocks; i++){
        disk->write(i, emptyBlock.Data);
    }

    return true;
}

// Mount file system -----------------------------------------------------------


bool FileSystem::mount(Disk *disk) {
    // Read superblock
    Block superBlock;
    disk->read(0, superBlock.Data);
    
    // Set device and mount
    this->disk = disk;
    disk->mount();

    // Copy metadata
    this->numBlocks     = superBlock.Super.Blocks;
    this->inodeBlocks   = superBlock.Super.InodeBlocks;
    this->inodes        = superBlock.Super.Inodes;
 
    this->inodeTable = new Block[this->inodeBlocks];
    for (uint32_t i = 0; i < this->inodeBlocks; i++){
        disk->read(i+1, this->inodeTable[i].Data);
    }

    // Allocate free block bitmap
    this->freeBlocks = new bool[this->numBlocks];
    for (uint32_t i = 0; i < this->numBlocks; i++){
        this->freeBlocks[i] = true;
    }
    this->freeBlocks[0] = false;
    for (uint32_t i = 0; i < this->inodeBlocks; i++){
        this->freeBlocks[i+1] = false;
    }

    Block inodeBlock;
    for (uint32_t i = 0; i < this->inodeBlocks; i++){
        disk->read(i+1, inodeBlock.Data);
        for (uint32_t j = 0; j < INODES_PER_BLOCK; j++){
            if (inodeBlock.Inodes[j].Valid){
                for (uint32_t k = 0; k < POINTERS_PER_INODE; k++){
                    if (inodeBlock.Inodes[j].Direct[k]){
                        this->freeBlocks[inodeBlock.Inodes[j].Direct[k]] = false;
                    }
                }
                if (inodeBlock.Inodes[j].Indirect){
                    this->freeBlocks[inodeBlock.Inodes[j].Indirect] = false;
                    Block indirectBlock;
                    disk->read(inodeBlock.Inodes[j].Indirect, indirectBlock.Data);
                    for (uint32_t k = 0; k < POINTERS_PER_BLOCK; k++){
                        if (indirectBlock.Pointers[k]){
                            this->freeBlocks[indirectBlock.Pointers[k]] = false;
                        }
                    } 
                }
            }
        }
    }
    
    return true;
}

// Create inode ----------------------------------------------------------------

void FileSystem::initialize_inode(Inode *node) {
    for (uint32_t i = 0; i < POINTERS_PER_INODE; i++) {
        node->Direct[i] = 0;
    }
    node->Indirect = 0;
}

ssize_t FileSystem::create() {
    // Locate free inode in inode table
    ssize_t inodeNumber = -1;
    for (uint32_t i = 0; i < this->inodeBlocks; i++) {
        for (uint32_t j = 0; j < INODES_PER_BLOCK; j++){
            if (!this->inodeTable[i].Inodes[j].Valid){
                this->inodeTable[i].Inodes[j].Valid = 1;
                initialize_inode(&inodeTable[i].Inodes[j]);
                inodeNumber = j+INODES_PER_BLOCK*i;
                break;
            }
        }
        if (inodeNumber != -1) {
            break;
        }
    }
    
    // Record inode if found   

    return inodeNumber;
}

// Remove inode ----------------------------------------------------------------

bool FileSystem::remove(size_t inumber) {
    // Load inode information
    Inode node_to_remove;
    load_inode(inumber, node_to_remove);
   
    // Free direct blocks
    for (uint32_t i = 0; i < POINTERS_PER_INODE; i++){
        this->freeBlocks[node_to_remove.Direct[i]] = true;
        node_to_remove.Direct[i] = 0;
    }   
 
    // Free indirect blocks
    if (node_to_remove.Indirect){
        
    }

    // Clear inode in inode table
    return true;
}

// Inode stat ------------------------------------------------------------------

ssize_t FileSystem::stat(size_t inumber) {
    // Load inode information
    return 0;
}

// Read from inode -------------------------------------------------------------

ssize_t FileSystem::read(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode information

    // Adjust length

    // Read block and copy to data
    return 0;
}

// Write to inode --------------------------------------------------------------

ssize_t FileSystem::write(size_t inumber, char *data, size_t length, size_t offset) {
    // Load inode
    
    // Write block and copy to data
    return 0;
}

bool FileSystem::load_inode(size_t inumber, Inode *node) {
    node = inodeTable[inumber/INODES_PER_BLOCK].Inodes[inumber%INODES_PER_BLOCK];
    if (node->Valid) {
        return true;
    }
    return false;
}   

bool FileSystem::save_inode(size_t inumber, Inode *node){
    inodeTable[inumber/INODES_PER_BLOCK].Inodes[inumber%INODES_PER_BLOCK] = node;
    if (inodeTable[inumber/INODES_PER_BLOCK].Inodes[inumber%INODES_PER_BLOCK].Valid){
        return true;
    }   
    return false;
} 
