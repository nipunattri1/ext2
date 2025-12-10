#include "struct.hpp"
#include "disk.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <bitset>

void Disk::setSuperBlock()
{
    img.seekg(1024);
    img.read(reinterpret_cast<char *>(&sb), sizeof(sb));
    miscInfo.block_size = 1024 << sb.block_size;
    miscInfo.frag_size = 1024 << sb.frag_size;

    miscInfo.group_count = sb.block_count / sb.group_block_count;
    if (sb.block_count % sb.group_block_count != 0)
        miscInfo.group_count++;
}

void Disk::setGDT()
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

void Disk::setinodeTable()
{

    inodeTableList.resize(sb.inode_count);
    int counter = 0;

    while (counter < sb.inode_count)
    {

        unsigned int group = (counter) / sb.group_inode_count;
        unsigned int seek = miscInfo.block_size * all_gdt[group].addr_inode_table + (counter % sb.group_inode_count) * sb.ext_sb.inode_size;
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



std::vector<uint8_t> Disk::getBitMap( int group_index)
{
    if (group_index < 0 || group_index >= all_gdt.size())
    {
        throw std::out_of_range("Invalid block group index");
    }

    std::vector<uint8_t> bitmap(miscInfo.block_size);

    uint64_t offset = (uint64_t)all_gdt[group_index].block_bitmap * miscInfo.block_size;

    img.seekg(offset);
    img.read(reinterpret_cast<char *>(bitmap.data()), miscInfo.block_size);

    return bitmap;
}

std::vector<uint32_t> Disk::getFreeBlocks( int n)
{
    std::vector<uint32_t> found_blocks;

    for (size_t group_idx = 0; group_idx < all_gdt.size(); ++group_idx)
    {
        if (found_blocks.size() == n)
            break;
        std::vector<uint8_t> bitmap = getBitMap( group_idx);

        for (size_t byte_idx = 0; byte_idx < bitmap.size(); ++byte_idx)
        {
            if (bitmap[byte_idx] == 0xFF)
                continue;
            for (int bit_idx = 0; bit_idx < 8; ++bit_idx)
            {
                if (!((bitmap[byte_idx] >> bit_idx) & 1))
                {
                    uint32_t block_in_group = (byte_idx * 8) + bit_idx;

                    if (block_in_group >= sb.group_block_count)
                    {
                        break;
                    }

                    uint32_t global_block = sb.first_data_block +
                                            (group_idx * sb.group_block_count) +
                                            block_in_group;

                    found_blocks.push_back(global_block);

                    if (found_blocks.size() == n)
                    {
                        return found_blocks;
                    }
                }
            }
        }
    }
    return found_blocks;
}
void Disk::setInode(unsigned int n, const inode &ino)
{
    // n is 1-based inode number, as in getInode
    unsigned int index = n - 1;

    if (index >= inodeTableList.size())
        return;

    inodeTableList[index] = ino;

    unsigned int group = index / sb.group_inode_count;
    unsigned int indexInGroup = index % sb.group_inode_count;

    uint64_t offset = static_cast<uint64_t>(miscInfo.block_size) *
                      all_gdt[group].addr_inode_table +
                      static_cast<uint64_t>(indexInGroup) * sb.ext_sb.inode_size;

    img.seekp(offset, std::ios::beg);
    img.write(reinterpret_cast<const char *>(&ino), sizeof(inode));
}

void Disk::writeBlock(uint32_t blockNo, const void *buf)
{
    uint64_t offset = static_cast<uint64_t>(blockNo) * miscInfo.block_size;
    img.seekp(offset, std::ios::beg);
    img.write(reinterpret_cast<const char *>(buf), miscInfo.block_size);
}
