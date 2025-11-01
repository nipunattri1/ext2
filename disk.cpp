#include "struct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <bitset>

void Disk::setSuperBlock(std::ifstream &img)
{
    img.seekg(1024);
    img.read(reinterpret_cast<char *>(&sb), sizeof(sb));
    miscInfo.block_size = 1024 << sb.block_size;
    miscInfo.frag_size = 1024 << sb.frag_size;

    miscInfo.group_count = sb.block_count / sb.group_block_count;
    if (sb.block_count % sb.group_block_count != 0)
        miscInfo.group_count++;
}

void Disk::setGDT(std::ifstream &img)
{
    img.seekg(2048);
    all_gdt.resize(miscInfo.group_count);
    img.read(reinterpret_cast<char *>(all_gdt.data()), sizeof(block_group_decriptor) * miscInfo.group_count);
    if (img.gcount() != sizeof(block_group_decriptor) * miscInfo.group_count)
    {
        std::cerr << "Warning: Incomplete GDT read (" << img.gcount() << "/" << (sizeof(block_group_decriptor) * miscInfo.group_count) << " bytes)\n";
        img.close();
        exit(1);
    }
}

void Disk::setinodeTable(std::ifstream &img)
{

    inodeTableList.resize(sb.inode_count);
    int counter = 0;

    while (counter < sb.inode_count)
    {

        unsigned int group = (counter) / sb.group_inode_count;
        unsigned int seek = miscInfo.block_size * all_gdt[group].addr_inode_table + counter * sb.ext_sb.inode_size;
        img.seekg(seek, std::ios::beg);
        inode inodeVal;
        img.read(reinterpret_cast<char *>(&inodeVal), sizeof(inode));
        inodeTableList[counter] = inodeVal;
        if (img.gcount() != sizeof(inode))
        {
            std::cerr << "Warning: Incomplete GDT read (" << img.gcount() << "/" << (sizeof(inode)) << " bytes)\n";
            img.close();
            exit(1);
        }
        counter++;
    }
}

