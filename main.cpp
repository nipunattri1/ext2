#include "disk.hpp"
#include "struct.hpp"
#include "utils.hpp"
#include <sstream>


// TODO Pass image path onto disk class and let the class handlle the image opeing precedure.
int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
        return 1;
    }
    // read inputed file (img)
    std::fstream img(argv[1], std::ios::binary);
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
        std::cout << "\n> ";
        std::getline(std::cin, in);
        std::vector<std::string> args;
        std::stringstream ss(in);
        std::string arg;

        while (ss >> arg)
        {
            args.push_back(arg);
        }

        if (args[0] == "ls")
        {
            util.ls(img);
        }
        else if (args[0] == "info")
        {
            util.printSuperBlock(img);
            util.printGDTEntries();
        }
        else if (args[0] == "cd")
        {
            if (args.size() != 2)
                std::cout << "Invalid Argument format" << std::endl;
            else
                util.cd(img, args[1]);
        }
        else if (args[0] == "cat")
        {
            if (args.size() != 2)
                std::cout << "Invalid Argument format" << std::endl;
            else
                util.cat(img, args[1]);
        }else if (args[0] == "write")
        {
            if (args.size() != 3)
                std::cout << "Invalid Argument format" << std::endl;
            else
                util.write(img, args[1], args[2]);
        }
        

        else if (in != "exit")
        {
            std::cout << "Unknown command " << in << std::endl;
        }

    } while (in != "exit");

    img.close();

    return 0;
}
