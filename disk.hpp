#ifndef DISK_H
#define DISK_H
#include "struct.hpp"
#include <fstream>
#include <vector>
#include <iostream>
class Disk
{
private:
    super_block sb;
    misc_info miscInfo;
    std::vector<block_group_decriptor> all_gdt;
    std::vector<inode> inodeTableList;

public:
    void writeBlock(uint32_t blockNo, const void *buf);
    void setInode(unsigned int n, const inode &ino);
    std::fstream img;
    Disk(const char file[])
    {
        img.open(file, std::ios::in | std::ios::out | std::ios::binary);
        if (!img)
        {
            std::cerr << "ERR: Couldn't Open the file: " << file << std::endl;
            exit(1);
        }
        setSuperBlock();
        setGDT();
        setinodeTable();
    }
    ~Disk()
    {
        img.close();
    }
    std::vector<uint8_t> getBitMap(int group_index);
    std::vector<uint32_t> getFreeBlocks(int n);
    void setSuperBlock();
    void setGDT();
    void setinodeTable();
    super_block getSuperBlock() { return sb; }
    misc_info getMiscInfo() { return miscInfo; }
    block_group_decriptor getBGD(unsigned int index) { return all_gdt[index]; }
    inode getInode(unsigned int n) { return inodeTableList[n - 1]; }
};

#endif