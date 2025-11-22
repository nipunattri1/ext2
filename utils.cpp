#include "utils.hpp"
#include "disk.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>
#include <bitset>

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

void DiskUtil::printSuperBlock(std::fstream &img)
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
    std::cout << "file_sys_id: ";
    // printArr(ex_sb.file_sys_id, 16);
    for (int i = 0; i < 16; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(sb.ext_sb.file_sys_id[i]);
    }
    std::cout << std::dec << "\n";
    std::cout << "vol_name: ";
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

void DiskUtil::setDirFiles(std::fstream &img)
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
            if (dirEntry.inode == 0)
                break;

            dirEntries.insert(dirEntries.end(), dirEntry);

        } while (offset < disk.getMiscInfo().block_size);
    }
    else
    {
        std::cout << "ERR: inode at top of stack is not a directory!" << std::endl;
    }
}

void DiskUtil::ls(std::fstream &img)
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

void DiskUtil::cd(std::fstream &img, std::string dir)
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
        if (i.file_type == 2 && i.name_len == dir.length())
        {
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
        std::cout << "No such directory " << dir << std::endl;
    }
}

void DiskUtil::cat(std::fstream &img, std::string file)
{
    setDirFiles(img);
    bool found = false;

    for (auto dirEntry : dirEntries)
    {
        // Check name length first
        if (dirEntry.name_len == file.length())
        {
            bool isEqual = true; // if this the same dir entry
            for (int j = 0; j < dirEntry.name_len; j++)
            {
                isEqual = (dirEntry.name[j] == file[j]);
                if (!isEqual)
                    break;
            }

            if (isEqual)
            {
                found = true;
                if (dirEntry.file_type == 1) // Check if Regular File
                {
                    inode fileInode = disk.getInode(dirEntry.inode);
                    uint32_t blockSize = disk.getMiscInfo().block_size;

                    std::vector<char> out(fileInode.i_size);
                    uint32_t bytesRead = 0;
                    uint32_t ptrsPerBlock = blockSize / sizeof(uint32_t);

                    // Helper Lambda to read a single data block into the buffer
                    auto readDataBlock = [&](uint32_t blockNum)
                    {
                        if (blockNum == 0)
                            return;
                        if (bytesRead >= fileInode.i_size)
                            return; // Done

                        uint32_t toRead = std::min((uint32_t)(fileInode.i_size - bytesRead), blockSize);

                        img.seekg((uint64_t)blockNum * blockSize);
                        img.read(&out[bytesRead], toRead);

                        bytesRead += toRead;
                    };

                    // Helper Lambda to load a block of pointers
                    auto loadPtrBlock = [&](uint32_t blockNum) -> std::vector<uint32_t>
                    {
                        std::vector<uint32_t> ptrs(ptrsPerBlock);
                        if (blockNum == 0)
                            return ptrs; // Return empty/zeroed if hole

                        img.seekg((uint64_t)blockNum * blockSize);
                        img.read(reinterpret_cast<char *>(ptrs.data()), blockSize);
                        return ptrs;
                    };

                    //  Direct Blocks (0-11)
                    for (int i = 0; i < 12; i++)
                    {
                        readDataBlock(fileInode.i_block[i]);
                    }

                    // Singly Indirect Block (12)
                    if (bytesRead < fileInode.i_size && fileInode.i_block[12] != 0)
                    {
                        std::vector<uint32_t> singlePtrs = loadPtrBlock(fileInode.i_block[12]);
                        for (uint32_t ptr : singlePtrs)
                        {
                            readDataBlock(ptr);
                        }
                    }

                    // Doubly Indirect Block (13)
                    if (bytesRead < fileInode.i_size && fileInode.i_block[13] != 0)
                    {
                        std::vector<uint32_t> doublePtrs = loadPtrBlock(fileInode.i_block[13]);

                        for (uint32_t singleBlockPtr : doublePtrs)
                        {
                            if (bytesRead >= fileInode.i_size)
                                break;
                            if (singleBlockPtr == 0)
                                continue;

                            std::vector<uint32_t> singlePtrs = loadPtrBlock(singleBlockPtr);
                            for (uint32_t ptr : singlePtrs)
                            {
                                readDataBlock(ptr);
                            }
                        }

                        // -Triply Indirect Block (14)
                        if (bytesRead < fileInode.i_size && fileInode.i_block[14] != 0)
                        {
                            std::vector<uint32_t> triplePtrs = loadPtrBlock(fileInode.i_block[14]);

                            for (uint32_t doubleBlockPtr : triplePtrs)
                            {
                                if (bytesRead >= fileInode.i_size)
                                    break;
                                if (doubleBlockPtr == 0)
                                    continue;

                                std::vector<uint32_t> doublePtrs = loadPtrBlock(doubleBlockPtr);
                                for (uint32_t singleBlockPtr : doublePtrs)
                                {
                                    if (bytesRead >= fileInode.i_size)
                                        break;
                                    if (singleBlockPtr == 0)
                                        continue;

                                    std::vector<uint32_t> singlePtrs = loadPtrBlock(singleBlockPtr);
                                    for (uint32_t ptr : singlePtrs)
                                    {
                                        readDataBlock(ptr);
                                    }
                                }
                            }
                        }
                    }
                    // Output content
                    for (char c : out)
                    {
                        std::cout << c;
                    }
                    std::cout << std::endl;
                    break; // Found and processed, exit loop
                }
                else
                {
                    std::cout << "ERR: " << file << " is not a Regular File" << std::endl;
                }
            }
        }
    }
    if (!found)
    {
        std::cout << "No such file " << file << std::endl;
    }
}

void DiskUtil::setBitMap(uint32_t block_id, bool val, std::fstream &img)
{
    /*
    Adjusted ID=block_id−FirstDataBlock
    Group Index=Adjusted ID/BlocksPerGroup
    Local Index=Adjusted ID%BlocksPerGroup
    */
    uint32_t adjusted_id = block_id - disk.getSuperBlock().first_data_block;
    uint32_t group_index = adjusted_id / disk.getSuperBlock().group_block_count;
    uint32_t local_index = adjusted_id % disk.getSuperBlock().group_block_count;

    uint32_t bit_loc = disk.getBGD(group_index).block_bitmap + local_index;
    std::bitset<8> prev;
    img.seekp(bit_loc);
    img.seekg(bit_loc);
    img.read(reinterpret_cast<char *>(&prev), 1);

    prev |= 01111111;
    img.write(reinterpret_cast<char *>(&prev), 1);
}

void DiskUtil::write(std::fstream &img, std::string content, std::string file)
{
    if (content[0] != '"' && content[content.length() - 1] != '"' && content.length() >= 2)
    {
        std::cout << "Err: text should be of format \"<text_here>\"" << std::endl;
    }
    else
    {
        setDirFiles(img);
        bool found = false, err = false;
        for (auto dirEntry : dirEntries)
        {
            if (dirEntry.name_len == file.length() && !err)
            {
                for (int i = 0; i < dirEntry.name_len; i++)
                {
                    if (dirEntry.name[i] != file[i])
                        break;

                    found = true;
                    if (dirEntry.file_type != 1)
                    {
                        err = true;
                        std::cout << "ERR: given input " << file << " is not a Regular File" << std::endl;
                    }
                    else
                    {
                        // TODO update inode
                        // TODO update file itself
                        //  dirEntry.inode
                        inode fileInode = disk.getInode(dirEntry.inode);
                        uint32_t prevSize = fileInode.i_size;
                        fileInode.i_size = content.length() - 2; // content of type "<text>" with " included
                        // TODO free the blocks first
                        std::vector<uint32_t> allocBlocks = getAllAllocatedBlocks(img, fileInode);
                        for (auto block : allocBlocks)
                            setBitMap(block, false, img);

                        // TODO update bitmap
                        // calculate the number of  blocks requried to allocate the storage
                        uint16_t blockSize = disk.getMiscInfo().block_size;
                        uint32_t dataBlocks = (content.length() + blockSize - 3) / blockSize;

                        uint32_t totalBlocks = dataBlocks;

                        // Constants for ext2
                        const uint32_t PTRS_PER_BLOCK = blockSize / 4; // usually 1024/4 = 256
                        const uint32_t DIRECT_COUNT = 12;

                        // Capacity thresholds (in number of data blocks)
                        const uint32_t SINGLE_CAPACITY = PTRS_PER_BLOCK;
                        const uint32_t DOUBLE_CAPACITY = PTRS_PER_BLOCK * PTRS_PER_BLOCK;
                        const uint32_t TRIPLE_CAPACITY = PTRS_PER_BLOCK * PTRS_PER_BLOCK * PTRS_PER_BLOCK;

                        if (dataBlocks > DIRECT_COUNT)
                        {
                            uint32_t remaining = dataBlocks - DIRECT_COUNT;
                            totalBlocks += 1;

                            if (remaining > SINGLE_CAPACITY)
                            {
                                remaining -= SINGLE_CAPACITY;

                                totalBlocks += 1;
                                uint32_t doubleIndirectPtrsNeeded = (remaining + PTRS_PER_BLOCK - 1) / PTRS_PER_BLOCK;

                                if (doubleIndirectPtrsNeeded > PTRS_PER_BLOCK)
                                {
                                    doubleIndirectPtrsNeeded = PTRS_PER_BLOCK;
                                }

                                totalBlocks += doubleIndirectPtrsNeeded;

                                if (remaining > DOUBLE_CAPACITY)
                                {
                                    remaining -= DOUBLE_CAPACITY;
                                    totalBlocks += 1;

                                    uint32_t triple_L2_Needed = (remaining + DOUBLE_CAPACITY - 1) / DOUBLE_CAPACITY;
                                    totalBlocks += triple_L2_Needed;

                                    uint32_t triple_L1_Needed = (remaining + PTRS_PER_BLOCK - 1) / PTRS_PER_BLOCK;
                                    totalBlocks += triple_L1_Needed;
                                }
                            }
                        }

                        auto freebits = disk.getFreeBlocks(img, totalBlocks);
                    }
                }
            }
        }
    }
}
std::vector<uint32_t> DiskUtil::getAllAllocatedBlocks(std::fstream &img, const inode &fileInode)
{
    std::vector<uint32_t> allocated_blocks;
    uint32_t blockSize = disk.getMiscInfo().block_size;
    uint32_t ptrsPerBlock = blockSize / 4; // 4 bytes per pointer

    // Helper to read a block of pointers from disk
    auto loadPtrBlock = [&](uint32_t blockNum) -> std::vector<uint32_t>
    {
        std::vector<uint32_t> ptrs(ptrsPerBlock);
        img.seekg((uint64_t)blockNum * blockSize);
        img.read(reinterpret_cast<char *>(ptrs.data()), blockSize);
        return ptrs;
    };

    // Direct Blocks (0-11)
    for (int i = 0; i < 12; i++)
    {
        if (fileInode.i_block[i] != 0)
        {
            allocated_blocks.push_back(fileInode.i_block[i]);
        }
    }

    // Singly Indirect (12)
    uint32_t single_block = fileInode.i_block[12];
    if (single_block != 0)
    {
        allocated_blocks.push_back(single_block);

        std::vector<uint32_t> ptrs = loadPtrBlock(single_block);
        for (uint32_t ptr : ptrs)
        {
            if (ptr != 0)
                allocated_blocks.push_back(ptr);
        }
    }

    //   Triple
    uint32_t double_block = fileInode.i_block[13];
    if (double_block != 0)
    {
        allocated_blocks.push_back(double_block);

        std::vector<uint32_t> l1_ptrs = loadPtrBlock(double_block);
        for (uint32_t l1_ptr : l1_ptrs)
        {
            if (l1_ptr != 0)
            {
                allocated_blocks.push_back(l1_ptr);

                std::vector<uint32_t> data_ptrs = loadPtrBlock(l1_ptr);
                for (uint32_t data_ptr : data_ptrs)
                {
                    if (data_ptr != 0)
                        allocated_blocks.push_back(data_ptr);
                }
            }
        }
    }

    // Triply Indirect (14)
    uint32_t triple_block = fileInode.i_block[14];
    if (triple_block != 0)
    {
        allocated_blocks.push_back(triple_block);

        std::vector<uint32_t> l2_ptrs = loadPtrBlock(triple_block);
        for (uint32_t l2_ptr : l2_ptrs)
        {
            if (l2_ptr != 0)
            {
                allocated_blocks.push_back(l2_ptr);

                std::vector<uint32_t> l1_ptrs = loadPtrBlock(l2_ptr);
                for (uint32_t l1_ptr : l1_ptrs)
                {
                    if (l1_ptr != 0)
                    {
                        allocated_blocks.push_back(l1_ptr);
                        std::vector<uint32_t> data_ptrs = loadPtrBlock(l1_ptr);
                        for (uint32_t data_ptr : data_ptrs)
                        {
                            if (data_ptr != 0)
                                allocated_blocks.push_back(data_ptr);
                        }
                    }
                }
            }
        }
    }

    return allocated_blocks;
}
