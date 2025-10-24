#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
void superBlock(std::ifstream &img);

struct super_block
{
    uint32_t inode_count;
    uint32_t block_count;
    uint32_t reserve_count;
    uint32_t unallo_blocks;
    uint32_t unallo_inodes;
    uint32_t super_block_no;
    uint32_t block_size;
    uint32_t frag_size;
    uint32_t group_block_count;
    uint32_t groyp_frag_count;
    uint32_t group_inode_count;
    uint32_t mount_time;
    uint32_t written_time;
    uint16_t mount_count;
    uint16_t max_mnt;
    uint16_t ext2_sig;
    uint16_t fs_state;
    uint16_t error_act;
    uint16_t minor_ver;
    uint32_t const_check;
    uint32_t force_check;
    uint32_t os_id;
    uint32_t major_ver;
    uint16_t user_id;
    uint16_t group_res;

} sb;
struct ext_super_block
{
    uint32_t first_inode;
    uint16_t inode_size;
    uint16_t sb_block;
    uint32_t opt_feat;
    uint32_t req_feat;
    uint32_t read_only_feat;
    uint8_t file_sys_id[16];
    uint8_t vol_name[16];
    uint8_t vol_path[64];
    uint32_t comp_algo;
    uint8_t pre_allo_files;
    uint8_t pre_allo_dir;
    uint16_t _;
    uint8_t journal_id[16];
    uint32_t journal_inode;
    uint32_t jourrnal_device;
    uint32_t inode_list_head;
};

struct block_group_decriptor
{
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t addr_inode_table;
    uint16_t unall_blocks;
    uint16_t unall_inodes;
    uint16_t dir_count;
};

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

void print_gdt_entry(const block_group_decriptor& bgd, uint32_t group_id, uint32_t actual_block_size) {
    std::ostringstream oss;
    uint64_t group_start_block = static_cast<uint64_t>(group_id) * bgd.block_bitmap; 
    uint64_t group_start = group_id * (actual_block_size / sizeof(block_group_decriptor));  


    oss << "(Group " << group_id << ": Blocks " << (group_id * 10000 + 1) << " to " << ((group_id + 1) * 10000) << " approx, based on group_block_count)\n";  // Placeholder; use real blocks_per_group from sb
    oss << "\tSuperblock type: primary if group 0, else secondary/backup\n";
    oss << "\tGroup descriptors at offset " << (1024 + actual_block_size + group_id * 32) << " (32 bytes/entry)\n";
    oss << "\tReserved GDT blocks: at end of group descriptors (if sparse)\n";
    oss << "\tBlock bitmap at block " << bgd.block_bitmap << " (offset: " << (bgd.block_bitmap * actual_block_size) << ")\n";
    oss << "\tInode bitmap at block " << bgd.inode_bitmap << " (offset: " << (bgd.inode_bitmap * actual_block_size) << ")\n";
    oss << "\tInode table at blocks " << bgd.addr_inode_table << " to " << (bgd.addr_inode_table + (sb.group_inode_count / 8 / actual_block_size)) << " (approx, offset start: " << (bgd.addr_inode_table * actual_block_size) << ")\n";  // Rough calc; inodes per block = block_size / inode_size
    oss << "\t" << bgd.unall_blocks << " free blocks, " << bgd.unall_inodes << " free inodes, " << bgd.dir_count << " directories\n";
    oss << "\tFree blocks: " << bgd.unall_blocks << " (total in group)\n";
    oss << "\tFree inodes: " << bgd.unall_inodes << " (total in group)\n";
    std::cout << oss.str();
}

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
    }
    // read inputed file (img)
    // std::ofstream log(std::format("{}.log", argv[1]));
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
    unsigned int blocks = sb.block_count / sb.group_block_count + (sb.block_count % sb.group_block_count) ? 1 : 0;
    for (int i = 1; i <= blocks; i++)
    {
        block_group_decriptor bgd;
        img.seekg(1024 + (sb.block_size * i));
        img.read(reinterpret_cast<char *>(&bgd), sizeof(block_group_decriptor));
        print_gdt_entry(bgd, i-1, block_size);
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