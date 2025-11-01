#ifndef UTILS_H
#define UTILS_H
#include "disk.hpp"
#include <stack>
class DiskUtil
{
    Disk disk;
    std::stack<unsigned int> folderStack;
    std::vector<directory> dirEntries;

private:
    void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id);
    void printArr(uint8_t arr[], int size);
    void setDirFiles(std::ifstream &img);


public:
    void setDisk(Disk &diskIn)
    {
        disk = diskIn;
    }
    void printSuperBlock(std::ifstream &img);

    void printGDTEntries()
    {
        for (int i = 0; i < disk.getMiscInfo().group_count; i++)
        {
            print_gdt_entry(disk.getBGD(i), i);
        }
    }

    void ls(std::ifstream &img);
};

#endif