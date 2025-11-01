#include "utils.hpp"
#include "disk.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>

void DiskUtil::print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id)
{
    std::cout << std::endl;
    super_block sb = disk.getSuperBlock();

    unsigned int lastgroup{group_id * sb.group_block_count};
    if ((sb.block_count) / ((group_id + 1) * sb.group_block_count + 1) == 0)
        lastgroup += (sb.block_count) % (((group_id)*sb.group_block_count) + 1);
    else
        lastgroup += sb.group_block_count;

    std::ostringstream oss;
    oss << "Group " << group_id << ": (Blocks " << group_id * sb.group_block_count + 1 << "-" << lastgroup << ")\n";
    oss << ((group_id == 0) ? "Primary superblock" : "Backup superblock") << " at " << group_id * sb.group_block_count + 1 << " Group Descriptors at " << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "-" << ((group_id == 0) ? 2 : group_id * sb.group_block_count + 2) << "\n";
    oss << "Block bitmap at " << bgd.block_bitmap << std::endl;
    oss << "Inode bitmap at " << bgd.inode_bitmap << std::endl;
    oss << "Inode table at " << bgd.addr_inode_table << std::endl;
    oss << bgd.unall_blocks << " free Blocks, " << bgd.unall_inodes << " free inodes, " << bgd.dir_count << " directories" << std::endl;

    std::cout << oss.str();
}

void DiskUtil::printSuperBlock(std::ifstream &img)
{
    super_block sb = disk.getSuperBlock();
    unsigned int block_size = pow(2, sb.block_size + 10);
    unsigned int frag_size = pow(2, sb.frag_size + 10);
    std::cout << "inode_count: " << sb.inode_count << "\n";
    std::cout << "block_count: " << sb.block_count << "\n";
    std::cout << "reserve_count: " << sb.reserve_count << "\n";
    std::cout << "unallo_blocks: " << sb.unallo_blocks << "\n";
    std::cout << "unallo_inodes: " << sb.unallo_inodes << "\n";
    std::cout << "first_data_block: " << sb.first_data_block << "\n";
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
    // if (sb.major_ver >= 1)
    // {
    // ext_super_block ex_sb;
    // img.read(reinterpret_cast<char *>(&ex_sb), sizeof(ex_sb));
    std::cout << "first_inode: " << sb.ext_sb.first_inode << "\n";
    std::cout << "inode_size: " << sb.ext_sb.inode_size << "\n";
    std::cout << "sb_block: " << sb.ext_sb.sb_block << "\n";
    std::cout << "opt_feat: " << sb.ext_sb.opt_feat << "\n";
    std::cout << "req_feat: " << sb.ext_sb.req_feat << "\n";
    std::cout << "read_only_feat: " << sb.ext_sb.read_only_feat << "\n";
    std::cout << "file_sys_id: "; // TODO
    // printArr(ex_sb.file_sys_id, 16);
    for (int i = 0; i < 16; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(sb.ext_sb.file_sys_id[i]);
    }
    std::cout << std::dec << "\n";
    std::cout << "vol_name: "; // TODO
    printArr(sb.ext_sb.vol_name, 16);
    std::cout << "vol_path: ";
    printArr(sb.ext_sb.vol_path, 64);
    std::cout << "comp_algo: " << sb.ext_sb.comp_algo << "\n";
    std::cout << "pre_allo_files: " << sb.ext_sb.comp_algo << "\n";
    std::cout << "pre_allo_dir: " << sb.ext_sb.pre_allo_dir << "\n";
    // printArr(ex_sb.pre_allo_dir, );
    std::cout << "journal_id: ";
    for (int i = 0; i < 16; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(sb.ext_sb.journal_id[i]);
    }
    std::cout << std::dec << "\n";
    std::cout << "journal_inode: " << sb.ext_sb.journal_inode << "\n";
    std::cout << "jourrnal_device: " << sb.ext_sb.jourrnal_device << "\n";
    std::cout << "inode_list_head: " << sb.ext_sb.inode_list_head << "\n";
    // }
}

void DiskUtil::printArr(uint8_t arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

void DiskUtil::setDirFiles(std::ifstream &img)
{

    inode i = disk.getInode(curretnInode); 
    dirEntries.clear();
    if (((i.i_mode & 0xF000) == 0x4000) || (i.i_mode & 0xF000) == 0x8000)
    {
        unsigned int offset = 0;
        do
        {
            directory dirEntry;
            img.seekg((i.i_block[0] * disk.getMiscInfo().block_size) + offset);
            img.read(reinterpret_cast<char *>(&dirEntry), 8);
            img.read(reinterpret_cast<char *>(&dirEntry.name), dirEntry.name_len);
            offset += dirEntry.rec_len;
            if (dirEntry.inode == 0 )
                break;

            dirEntries.insert(dirEntries.end(), dirEntry);

        } while (offset < disk.getMiscInfo().block_size);
    }
    else
    {
        std::cout << "ERR: inode at top of stack is not a directory!" << std::endl;
    }
}

void DiskUtil::ls(std::ifstream &img)
{

    setDirFiles(img);
    for (directory i : dirEntries)
    {
        for (int k = 0; k < i.name_len; k++)
        {
            std::cout << i.name[k];
        }
        if (i.file_type == 2)
            std::cout << "/";
        if (i.name_len != 0)
            std::cout << std::endl;
    }
}

void DiskUtil::cd(std::ifstream &img, std::string dir)
{
    bool found = false;
    if (dir == "/")
    {
        found = true;
        curretnInode = 2;
    }
    setDirFiles(img);
    
    for (auto i : dirEntries)
    {
        if(i.file_type == 2 && i.name_len == dir.length()){
            bool isEqual = true;
            for (int j = 0; j < i.name_len; j++)
            {
                isEqual = (i.name[j] == dir[j]);
                if (!isEqual)
                    break;
                
            }
            if (isEqual)
            {
                curretnInode = i.inode;
                found = true;
                break;
            }
            
            
            
        }
    }
    if (!found)
    {
        std::cout<<"No such directory "<<dir<<std::endl;
    }
    
    // setDirFiles(img);

}