#ifndef UTILS_H
#define UTILS_H
#include "disk.hpp"
#include <stack>
class DiskUtil
{
    Disk disk;
    uint64_t curretnInode = 2;
    std::vector<directory> dirEntries;

private:
    void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id);
    void printArr(uint8_t arr[], int size);
    void setDirFiles(std::fstream &img);
    // std::vector<char> getDirectBlockVal(std::fstream &img,uint32_t blockAddr);
    std::vector<uint32_t> getAllAllocatedBlocks(std::fstream &img, const inode &fileInode);
    void setBitMap(uint32_t block_id, bool val, std::fstream &img);


public:
    void setDisk(Disk &diskIn)
    {
        disk = diskIn;
    }
    void printSuperBlock(std::fstream &img);

    void printGDTEntries()
    {
        for (int i = 0; i < disk.getMiscInfo().group_count; i++)
        {
            print_gdt_entry(disk.getBGD(i), i);
        }
    }

    void ls(std::fstream &img);
    void cd(std::fstream &img, std::string dir);
    void cat(std::fstream &img, std::string file);
    void write(std::fstream &img, std::string content, std::string file);
};

#endif