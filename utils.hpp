#ifndef UTILS_H
#define UTILS_H
#include "disk.hpp"
class DiskUtil
{
    Disk disk;

private:
    void print_gdt_entry(const block_group_decriptor &bgd, uint32_t group_id);
    void printArr(uint8_t arr[], int size);


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
};

#endif