#include "disk.hpp"
#include "struct.hpp"
#include "utils.hpp"

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
        return 1;
    }
    // read inputed file (img)
    std::ifstream img(argv[1], std::ios::binary);
    if (!img)
    {
        std::cerr << "ERR: Couldn't Open the file" << std::endl;
        return 1;
    }

    Disk disk;
    disk.setSuperBlock(img);
    disk.setGDT(img);
    disk.setinodeTable(img);
    

    // setup an util class instance
    DiskUtil util;
    util.setDisk(disk);

    std::string in;
    do
    {
        std::cout<<"\n> ";
        std::cin>>in;
        if (in == "ls")
        {
            util.ls(img);
        }
        else if (in == "info")
        {
            std::cout<<"===Printing Info==="<<std::endl;
            util.printSuperBlock(img);
            util.printGDTEntries();
        } else if (in != "exit"){
            std::cout<<"Unknown command " << in <<std::endl;
        }

    } while (in != "exit");

    img.close();

    return 0;
}
