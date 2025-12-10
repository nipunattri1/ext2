#include "utils.hpp"
#include "disk.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>
#include <bitset>
#include <cstring>

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

void DiskUtil::printSuperBlock()
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

void DiskUtil::setDirFiles()
{

    inode i = disk.getInode(curretnInode);
    dirEntries.clear();
    if (((i.i_mode & 0xF000) == 0x4000) || (i.i_mode & 0xF000) == 0x8000)
    {
        unsigned int offset = 0;
        do
        {
            directory dirEntry;
            disk.img.seekg((i.i_block[0] * disk.getMiscInfo().block_size) + offset);
            disk.img.read(reinterpret_cast<char *>(&dirEntry), 8);
            disk.img.read(reinterpret_cast<char *>(&dirEntry.name), dirEntry.name_len);
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

void DiskUtil::ls()
{

    setDirFiles();
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

void DiskUtil::cd( std::string dir)
{
    bool found = false;
    if (dir == "/")
    {
        found = true;
        curretnInode = 2;
    }
    setDirFiles();

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

void DiskUtil::cat( std::string file)
{
    setDirFiles();
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

                        disk.img.seekg((uint64_t)blockNum * blockSize);
                        disk.img.read(&out[bytesRead], toRead);

                        bytesRead += toRead;
                    };

                    // Helper Lambda to load a block of pointers
                    auto loadPtrBlock = [&](uint32_t blockNum) -> std::vector<uint32_t>
                    {
                        std::vector<uint32_t> ptrs(ptrsPerBlock);
                        if (blockNum == 0)
                            return ptrs; // Return empty/zeroed if hole

                        disk.img.seekg((uint64_t)blockNum * blockSize);
                        disk.img.read(reinterpret_cast<char *>(ptrs.data()), blockSize);
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

void DiskUtil::setBitMap(uint32_t block_id, bool val)
{
    const auto &sb   = disk.getSuperBlock();
    const auto &misc = disk.getMiscInfo();

    uint32_t adjusted_id = block_id - sb.first_data_block;
    uint32_t group_index = adjusted_id / sb.group_block_count;
    uint32_t local_index = adjusted_id % sb.group_block_count;

    uint32_t byte_index = local_index / 8;
    uint32_t bit_index  = local_index % 8;

    uint32_t bitmap_block = disk.getBGD(group_index).block_bitmap;
    uint64_t byte_offset  = static_cast<uint64_t>(bitmap_block) * misc.block_size + byte_index;

    uint8_t b;
    disk.img.seekg(byte_offset);
    disk.img.read(reinterpret_cast<char *>(&b), 1);

    if (val)
        b |= (1u << bit_index);   // mark allocated
    else
        b &= ~(1u << bit_index);  // mark free

    disk.img.seekp(byte_offset);
    disk.img.write(reinterpret_cast<char *>(&b), 1);
}
void DiskUtil::write(std::string content, std::string file)
{
    if (content.size() < 2 || content.front() != '"' || content.back() != '"')
    {
        std::cout << "Err: text should be of format \"<text_here>\"" << std::endl;
        return;
    }

    // Strip quotes, no escaping handled
    std::string payload = content.substr(1, content.size() - 2);
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());

    setDirFiles();
    bool found = false;
    bool err = false;

    for (auto dirEntry : dirEntries)
    {
        if (err)
            break;

        if (dirEntry.name_len != file.length())
            continue;

        bool nameMatch = true;
        for (int i = 0; i < dirEntry.name_len; ++i)
        {
            if (dirEntry.name[i] != file[i])
            {
                nameMatch = false;
                break;
            }
        }
        if (!nameMatch)
            continue;

        found = true;

        if (dirEntry.file_type != 1) // regular file in ext2
        {
            err = true;
            std::cout << "ERR: given input " << file << " is not a Regular File" << std::endl;
            break;
        }

        // Load inode
        inode fileInode = disk.getInode(dirEntry.inode);

        // Free all currently allocated blocks (data + metadata)
        std::vector<uint32_t> allocBlocks = getAllAllocatedBlocks(fileInode);
        for (auto block : allocBlocks)
        {
            setBitMap(block, false);
        }

        // Reset block pointers
        for (int i = 0; i < 15; ++i)
            fileInode.i_block[i] = 0;

        // Update size
        fileInode.i_size = payloadSize;

        uint16_t blockSize = disk.getMiscInfo().block_size;
        if (payloadSize == 0)
        {
            // Just write inode with size 0, no blocks
            disk.setInode(dirEntry.inode, fileInode);
            break;
        }

        // Number of data blocks needed
        uint32_t dataBlocks = (payloadSize + blockSize - 1) / blockSize;

        // Compute total blocks (data + metadata) using your existing logic
        uint32_t totalBlocks = dataBlocks;

        const uint32_t PTRS_PER_BLOCK = blockSize / 4;
        const uint32_t DIRECT_COUNT   = 12;

        const uint32_t SINGLE_CAPACITY = PTRS_PER_BLOCK;
        const uint32_t DOUBLE_CAPACITY = PTRS_PER_BLOCK * PTRS_PER_BLOCK;
        const uint32_t TRIPLE_CAPACITY = PTRS_PER_BLOCK * PTRS_PER_BLOCK * PTRS_PER_BLOCK;

        if (dataBlocks > DIRECT_COUNT)
        {
            uint32_t remaining = dataBlocks - DIRECT_COUNT;
            // Single-indirect root block
            totalBlocks += 1;

            if (remaining > SINGLE_CAPACITY)
            {
                remaining -= SINGLE_CAPACITY;

                // Double-indirect root block
                totalBlocks += 1;

                // Number of single-indirect blocks needed under the double-indirect
                uint32_t doubleIndirectPtrsNeeded =
                    (remaining + PTRS_PER_BLOCK - 1) / PTRS_PER_BLOCK;
                if (doubleIndirectPtrsNeeded > PTRS_PER_BLOCK)
                    doubleIndirectPtrsNeeded = PTRS_PER_BLOCK;

                totalBlocks += doubleIndirectPtrsNeeded;

                if (remaining > DOUBLE_CAPACITY)
                {
                    remaining -= DOUBLE_CAPACITY;

                    // Triple-indirect root block
                    totalBlocks += 1;

                    // L2 and L1 index blocks for triple-indirect
                    uint32_t triple_L2_Needed =
                        (remaining + DOUBLE_CAPACITY - 1) / DOUBLE_CAPACITY;
                    totalBlocks += triple_L2_Needed;

                    uint32_t triple_L1_Needed =
                        (remaining + PTRS_PER_BLOCK - 1) / PTRS_PER_BLOCK;
                    totalBlocks += triple_L1_Needed;
                }
            }
        }

        // Allocate blocks
        std::vector<uint32_t> blocks = disk.getFreeBlocks(totalBlocks);
        if (blocks.size() != totalBlocks)
        {
            std::cout << "ERR: not enough free blocks" << std::endl;
            // Inode already has i_block wiped and size set; leave as empty file
            disk.setInode(dirEntry.inode, fileInode);
            break;
        }

        // Mark all allocated
        for (auto b : blocks)
            setBitMap(b, true);

        // First dataBlocks entries in 'blocks' are data blocks
        std::vector<uint32_t> dataBlockNos(blocks.begin(),
                                           blocks.begin() + dataBlocks);
        // Remaining are metadata blocks
        std::vector<uint32_t> metaBlockNos(blocks.begin() + dataBlocks,
                                           blocks.end());

        size_t dataPos = 0;
        size_t metaPos = 0;

        // 1) Direct blocks
        for (uint32_t i = 0; i < DIRECT_COUNT && dataPos < dataBlocks; ++i)
        {
            fileInode.i_block[i] = dataBlockNos[dataPos++];
        }

        // Helper lambda to write an array of uint32_t into a block
        auto writePtrBlock = [&](uint32_t blockNo,
                                 const std::vector<uint32_t> &ptrs) {
            std::vector<uint32_t> buf(PTRS_PER_BLOCK, 0);
            for (size_t i = 0; i < ptrs.size() && i < PTRS_PER_BLOCK; ++i)
                buf[i] = ptrs[i];
            disk.writeBlock(blockNo, buf.data());
        };

        // 2) Single-indirect blocks (if needed)
        if (dataPos < dataBlocks)
        {
            if (metaPos >= metaBlockNos.size())
            {
                std::cout << "ERR: metadata allocation mismatch (single indirect)" << std::endl;
                disk.setInode(dirEntry.inode, fileInode);
                break;
            }

            uint32_t singleRoot = metaBlockNos[metaPos++];
            fileInode.i_block[12] = singleRoot;

            std::vector<uint32_t> singlePtrs;
            while (dataPos < dataBlocks && singlePtrs.size() < PTRS_PER_BLOCK)
            {
                singlePtrs.push_back(dataBlockNos[dataPos++]);
            }
            writePtrBlock(singleRoot, singlePtrs);
        }

        // 3) Double-indirect blocks (if needed)
        if (dataPos < dataBlocks)
        {
            if (metaPos >= metaBlockNos.size())
            {
                std::cout << "ERR: metadata allocation mismatch (double indirect root)" << std::endl;
                disk.setInode(dirEntry.inode, fileInode);
                break;
            }

            uint32_t doubleRoot = metaBlockNos[metaPos++];
            fileInode.i_block[13] = doubleRoot;

            std::vector<uint32_t> doubleRootPtrs;

            while (dataPos < dataBlocks && doubleRootPtrs.size() < PTRS_PER_BLOCK)
            {
                if (metaPos >= metaBlockNos.size())
                {
                    std::cout << "ERR: metadata allocation mismatch (double indirect l1)" << std::endl;
                    disk.setInode(dirEntry.inode, fileInode);
                    break;
                }

                uint32_t l1Block = metaBlockNos[metaPos++];
                doubleRootPtrs.push_back(l1Block);

                std::vector<uint32_t> l1Ptrs;
                while (dataPos < dataBlocks && l1Ptrs.size() < PTRS_PER_BLOCK)
                {
                    l1Ptrs.push_back(dataBlockNos[dataPos++]);
                }
                writePtrBlock(l1Block, l1Ptrs);
            }

            writePtrBlock(doubleRoot, doubleRootPtrs);
        }

        // 4) Triple-indirect blocks (if needed)
        if (dataPos < dataBlocks)
        {
            if (metaPos >= metaBlockNos.size())
            {
                std::cout << "ERR: metadata allocation mismatch (triple indirect root)" << std::endl;
                disk.setInode(dirEntry.inode, fileInode);
                break;
            }

            uint32_t tripleRoot = metaBlockNos[metaPos++];
            fileInode.i_block[14] = tripleRoot;

            std::vector<uint32_t> tripleRootPtrs; // L2 blocks

            while (dataPos < dataBlocks && tripleRootPtrs.size() < PTRS_PER_BLOCK)
            {
                if (metaPos >= metaBlockNos.size())
                {
                    std::cout << "ERR: metadata allocation mismatch (triple indirect l2)" << std::endl;
                    disk.setInode(dirEntry.inode, fileInode);
                    break;
                }

                uint32_t l2Block = metaBlockNos[metaPos++];
                tripleRootPtrs.push_back(l2Block);

                std::vector<uint32_t> l2Ptrs; // L1 blocks
                while (dataPos < dataBlocks && l2Ptrs.size() < PTRS_PER_BLOCK)
                {
                    if (metaPos >= metaBlockNos.size())
                    {
                        std::cout << "ERR: metadata allocation mismatch (triple indirect l1)" << std::endl;
                        disk.setInode(dirEntry.inode, fileInode);
                        break;
                    }

                    uint32_t l1Block = metaBlockNos[metaPos++];
                    l2Ptrs.push_back(l1Block);

                    std::vector<uint32_t> l1Ptrs; // data blocks
                    while (dataPos < dataBlocks && l1Ptrs.size() < PTRS_PER_BLOCK)
                    {
                        l1Ptrs.push_back(dataBlockNos[dataPos++]);
                    }
                    writePtrBlock(l1Block, l1Ptrs);
                }

                writePtrBlock(l2Block, l2Ptrs);
            }

            writePtrBlock(tripleRoot, tripleRootPtrs);
        }

        // Finally, write payload into data blocks
        for (uint32_t i = 0; i < dataBlocks; ++i)
        {
            uint32_t blockNo = dataBlockNos[i];
            std::vector<char> buf(blockSize, 0);

            uint32_t offset = i * blockSize;
            uint32_t remaining = payloadSize - offset;
            uint32_t toCopy = std::min<uint32_t>(remaining, blockSize);

            if (toCopy > 0)
            {
                std::memcpy(buf.data(),
                            payload.data() + offset,
                            toCopy);
            }

            disk.writeBlock(blockNo, buf.data());
        }

        // Persist inode
        disk.setInode(dirEntry.inode, fileInode);
        break;
    }

    if (!found && !err)
    {
        std::cout << "ERR: file " << file << " not found in current directory" << std::endl;
    }
}

std::vector<uint32_t> DiskUtil::getAllAllocatedBlocks(const inode &fileInode)
{
    std::vector<uint32_t> allocated_blocks;
    uint32_t blockSize = disk.getMiscInfo().block_size;
    uint32_t ptrsPerBlock = blockSize / 4; // 4 bytes per pointer

    // Helper to read a block of pointers from disk
    auto loadPtrBlock = [&](uint32_t blockNum) -> std::vector<uint32_t>
    {
        std::vector<uint32_t> ptrs(ptrsPerBlock);
        disk.img.seekg((uint64_t)blockNum * blockSize);
        disk.img.read(reinterpret_cast<char *>(ptrs.data()), blockSize);
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
