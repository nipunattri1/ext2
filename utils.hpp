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
    void setDirFiles(std::ifstream &img);
    std::vector<char> getDirectBlockVal(std::ifstream &img,uint32_t blockAddr);


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
    void cd(std::ifstream &img, std::string dir);
    void cat(std::ifstream &img, std::string file);
    void write(std::ifstream &img,std::string content, std::string file);
};

#endif