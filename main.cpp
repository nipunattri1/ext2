#include "superblock.hpp"
#include "struct.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

super_block sb;

void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id, uint32_t actual_block_size);



int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
        return 1;
    }
    // read inputed file (img)
    std::ifstream img(argv[1], std::ios::binary);
    if (!img)
    {
        std::cerr << "ERR: Couldn't Open the file" << std::endl;
        return 1;
    }

    img.seekg(1024);
    img.read(reinterpret_cast<char *>(&sb), sizeof(sb));
    superBlock(img, sb);

    unsigned int block_size = 1024 << sb.block_size;
    unsigned int frag_size = 1024 << sb.frag_size;

    unsigned int group_count = sb.block_count / sb.group_block_count;
    if (sb.block_count % sb.group_block_count != 0)
        group_count++;

    for (int i = 0; i < group_count; i++)
    {
        block_group_decriptor bgd;
        img.seekg(2048 + (sb.block_size * i));
        img.read(reinterpret_cast<char *>(&bgd), sizeof(block_group_decriptor));
        print_gdt_entry(bgd, i, block_size);
    }
    img.close();

    return 0;
}


void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id, uint32_t actual_block_size)
{
    std::cout << std::endl;

    unsigned int lastgroup{group_id * sb.group_block_count};
    if ((sb.block_count) / ((group_id + 1) * sb.group_block_count + 1) == 0)
        lastgroup += (sb.block_count) % (((group_id ) * sb.group_block_count) + 1) - 1;
    else
        lastgroup += sb.group_block_count;

    std::ostringstream oss;
    oss << "Group " << group_id << ": (Blocks " << group_id * sb.group_block_count + 1 << "-" << lastgroup << ")\n";
    oss << ((group_id == 0) ? "Primary superblock" : "Backup superblock") << " at " << group_id * sb.group_block_count + 1 << " Group Descriptors at " << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "-" << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "\n";
    oss << "Block bitmap at " << bgd.block_bitmap + group_id*sb.group_block_count << std::endl;
    oss << "Inode bitmap at " << bgd.inode_bitmap + group_id*sb.group_block_count<< std::endl;
    oss << "Inode table at " <<  bgd.addr_inode_table + group_id*sb.group_block_count<<std::endl;
    oss << bgd.unall_blocks << " free Blocks, " <<  bgd.unall_inodes << " free inodes, "<< bgd.dir_count<<" directories"<<std::endl;

    std::cout << oss.str();
}
