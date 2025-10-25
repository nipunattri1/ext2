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
    std::streampos gdt_start = 1024*2; // GDT after superblock (always primary location first)
    img.seekg(gdt_start);
    std::vector<block_group_decriptor> all_gdt(group_count);
    img.read(reinterpret_cast<char *>(all_gdt.data()), sizeof(block_group_decriptor) * group_count);
    if (img.gcount() != sizeof(block_group_decriptor) * group_count)
    {
        std::cerr << "Warning: Incomplete GDT read (" << img.gcount() << "/" << (sizeof(block_group_decriptor) * group_count) << " bytes)\n";
        img.close();
        return 1;
    }
    for (int i = 0; i < group_count; i++)
    {
        print_gdt_entry(all_gdt[i], i, group_count);
    }
    img.close();

    return 0;
}

void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id, uint32_t num_groups)
{
    std::cout << std::endl;

    unsigned int lastgroup{group_id * sb.group_block_count};
    if ((sb.block_count) / ((group_id + 1) * sb.group_block_count + 1) == 0)
        lastgroup += (sb.block_count) % (((group_id ) * sb.group_block_count) + 1) ;
    else
        lastgroup += sb.group_block_count;

    std::ostringstream oss;
    oss << "Group " << group_id << ": (Blocks " << group_id * sb.group_block_count + 1 << "-" << lastgroup << ")\n";
    oss << ((group_id == 0) ? "Primary superblock" : "Backup superblock") << " at " << group_id * sb.group_block_count + 1 << " Group Descriptors at " << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "-" << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "\n";
    oss << "Block bitmap at " << bgd.block_bitmap  << std::endl;
    oss << "Inode bitmap at " << bgd.inode_bitmap << std::endl;
    oss << "Inode table at " <<  bgd.addr_inode_table <<std::endl;
    oss << bgd.unall_blocks << " free Blocks, " <<  bgd.unall_inodes << " free inodes, "<< bgd.dir_count<<" directories"<<std::endl;

    std::cout << oss.str();
   
}
