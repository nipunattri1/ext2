#ifndef DISK_H
#define DISK_H
#include "struct.hpp"
#include <fstream>
#include <vector>
#include <iostream>
class Disk
{
    super_block sb;
    misc_info miscInfo;
    unsigned int filled_inode_count;
    std::vector<block_group_decriptor> all_gdt;
    std::vector<inode> inodeTableList;
    struct IMode
    {
        enum
        {
            EXT2_S_IFSOCK = 0xC000,
            EXT2_S_IFLNK = 0xA000,
            EXT2_S_IFRE = 0x8000,
            EXT2_S_IFBLK = 0x6000,
            EXT2_S_IFDIR = 0x4000,
            EXT2_S_IFCHR = 0x2000,
            EXT2_S_IFIFO = 0x1000
        } file;
        enum
        {
            EXT2_S_ISUID = 0x0800,
            EXT2_S_ISGID = 0x0400,
            EXT2_S_ISVTX = 0x0200
        } process;
        enum
        {
            EXT2_S_IRUSR = 0x0100,
            EXT2_S_IWUSR = 0x0080,
            EXT2_S_IXUSR = 0x0040,
            EXT2_S_IRGRP = 0x0020,
            EXT2_S_IWGRP = 0x0010,
            EXT2_S_IXGRP = 0x0008,
            EXT2_S_IROTH = 0x0004,
            EXT2_S_IWOTH = 0x0002,
            EXT2_S_IXOTH = 0x0001
        } rights;
    };

public:
    super_block getSuperBlock()
    {
        return sb;
    }
    misc_info getMiscInfo()
    {
        return miscInfo;
    }
    block_group_decriptor getBGD(unsigned int index)
    {
        return all_gdt[index];
    }
    inode getInode(unsigned int n)
    {
        return inodeTableList[n-1];
    }
    void setSuperBlock(std::ifstream &img);
    void setGDT(std::ifstream &img);
    void setinodeTable(std::ifstream &img);
};

#endif