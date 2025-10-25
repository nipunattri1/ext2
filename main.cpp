#include "struct.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

super_block sb;

void superBlock(std::ifstream &img);

void printArr(uint8_t arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        // if (arr[i] == '\n')
        //     break;
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id, uint32_t actual_block_size)
{
    std::cout << std::endl;

    unsigned int lastgroup{group_id * sb.group_block_count};
    if ((sb.block_count) / ((group_id + 1) * sb.group_block_count + 1) == 0)
        lastgroup += (sb.block_count) % (((group_id + 1) * sb.group_block_count) + 1) - 1;
    else
        lastgroup += sb.group_block_count;

    std::ostringstream oss;
    oss << "Group " << group_id << ": (Blocks " << group_id * sb.group_block_count + 1 << "-" << lastgroup << ")\n";
    oss << ((group_id == 0) ? "Primary superblock" : "Backup superblock") << " at " << group_id * sb.group_block_count + 1 << " Group Descriptors at " << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 1) << "-" << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 1) << "\n";
    oss << "Block bitmap at " << bgd.block_bitmap << std::endl;
    oss << "Inode bitmap at " << bgd.inode_bitmap << std::endl;
    oss << "Inode table at " <<  bgd.addr_inode_table <<std::endl;
    oss << bgd.unall_blocks << " free Blocks, " <<  bgd.unall_inodes << " free inodes, "<< bgd.dir_count<<" directories"<<std::endl;

    std::cout << oss.str();
}

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
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
    superBlock(img);

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

void superBlock(std::ifstream &img)
{
    unsigned int block_size = pow(2, sb.block_size + 10);
    unsigned int frag_size = pow(2, sb.frag_size + 10);
    std::cout << "inode_count: " << sb.inode_count << "\n";
    std::cout << "block_count: " << sb.block_count << "\n";
    std::cout << "reserve_count: " << sb.reserve_count << "\n";
    std::cout << "unallo_blocks: " << sb.unallo_blocks << "\n";
    std::cout << "unallo_inodes: " << sb.unallo_inodes << "\n";
    std::cout << "super_block_no: " << sb.super_block_no << "\n";
    std::cout << "block_size: " << block_size << "\n";
    std::cout << "frag_size: " << frag_size << "\n";
    std::cout << "group_block_count: " << sb.group_block_count << "\n";
    std::cout << "groyp_frag_count: " << sb.groyp_frag_count << "\n";
    std::cout << "group_inode_count: " << sb.group_inode_count << "\n";

    time_t mnt_time = static_cast<time_t>(sb.mount_time);
    std::cout << "mount_time: " << std::asctime(std::localtime(&mnt_time));

    time_t wr_time = static_cast<time_t>(sb.written_time);
    std::cout << "written_time: " << std::asctime(std::localtime(&wr_time));

    std::cout << "mount_count: " << sb.mount_count << "\n";
    std::cout << "max_mnt: " << static_cast<int>(sb.max_mnt) << "\n";
    std::cout << "ext2_sig: 0x" << std::hex << sb.ext2_sig << std::dec << "\n";
    std::cout << "fs_state: " << sb.fs_state << "\n";
    std::cout << "error_act: " << sb.error_act << "\n";
    std::cout << "minor_ver: " << sb.minor_ver << "\n";
    time_t const_check = static_cast<time_t>(sb.const_check);
    std::cout << "const_check: " << std::asctime(std::localtime(&const_check));
    std::cout << "force_check: " << sb.force_check << "\n";
    std::cout << "os_id: " << sb.os_id << "\n";
    std::cout << "major_ver: " << sb.major_ver << "\n";
    std::cout << "user_id: " << sb.user_id << "\n";
    std::cout << "group_res: " << sb.group_res << "\n";
    if (sb.major_ver >= 1)
    {
        ext_super_block ex_sb;
        img.read(reinterpret_cast<char *>(&ex_sb), sizeof(ex_sb));
        std::cout << "first_inode: " << ex_sb.first_inode << "\n";
        std::cout << "inode_size: " << ex_sb.inode_size << "\n";
        std::cout << "sb_block: " << ex_sb.sb_block << "\n";
        std::cout << "opt_feat: " << ex_sb.opt_feat << "\n";
        std::cout << "req_feat: " << ex_sb.req_feat << "\n";
        std::cout << "read_only_feat: " << ex_sb.read_only_feat << "\n";
        std::cout << "file_sys_id: "; // TODO
        // printArr(ex_sb.file_sys_id, 16);
        for (int i = 0; i < 16; ++i)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ex_sb.file_sys_id[i]);
        }
        std::cout << std::dec << "\n";
        std::cout << "vol_name: "; // TODO
        printArr(ex_sb.vol_name, 16);
        std::cout << "vol_path: ";
        printArr(ex_sb.vol_path, 64);
        std::cout << "comp_algo: " << ex_sb.comp_algo << "\n";
        std::cout << "pre_allo_files: " << ex_sb.comp_algo << "\n";
        std::cout << "pre_allo_dir: " << ex_sb.pre_allo_dir << "\n";
        // printArr(ex_sb.pre_allo_dir, );
        std::cout << "journal_id: ";
        for (int i = 0; i < 16; ++i)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ex_sb.journal_id[i]);
        }
        std::cout << std::dec << "\n";
        std::cout << "journal_inode: " << ex_sb.journal_inode << "\n";
        std::cout << "jourrnal_device: " << ex_sb.jourrnal_device << "\n";
        std::cout << "inode_list_head: " << ex_sb.inode_list_head << "\n";
    }
}