#ifndef UTILS_H
#define UTILS_H
#include "disk.hpp"
#include <stack>
class DiskUtil
{
    Disk &disk;
    uint64_t curretnInode = 2;
    std::vector<directory> dirEntries;

private:
    void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id);
    void printArr(uint8_t arr[], int size);
    void setDirFiles();
    std::vector<uint32_t> getAllAllocatedBlocks(const inode &fileInode);
    void setBitMap(uint32_t block_id, bool val);


public:
    DiskUtil(Disk &diskIn) : disk(diskIn) {}
    void printSuperBlock();

    void printGDTEntries()
    {
        for (int i = 0; i < disk.getMiscInfo().group_count; i++)
        {
            print_gdt_entry(disk.getBGD(i), i);
        }
    }

    void ls();
    void cd(std::string dir);
    void cat(std::string file);
    void write(std::string content, std::string file);
};

#endif