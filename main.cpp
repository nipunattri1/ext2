#include "disk.hpp"
#include "struct.hpp"
#include "utils.hpp"
#include <sstream>

int main(int argc, char const *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Invalid args, Run program as: " << argv[0] << "<.img>" << std::endl;
        return 1;
    }

    Disk disk(argv[1]);

    // setup an util class instance
    DiskUtil util(disk);

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
            util.ls();
        }
        else if (args[0] == "info")
        {
            util.printSuperBlock();
            util.printGDTEntries();
        }
        else if (args[0] == "cd")
        {
            if (args.size() != 2)
                std::cout << "Invalid Argument format" << std::endl;
            else
                util.cd(args[1]);
        }
        else if (args[0] == "cat")
        {
            if (args.size() != 2)
                std::cout << "Invalid Argument format" << std::endl;
            else
                util.cat(args[1]);
        }
        else if (args[0] == "write")
        {
            std::string line;
            for (size_t i = 0; i < args.size(); ++i)
            {
                if (i)
                    line += ' ';
                line += args[i];
            }
            size_t firstQuote = line.find('"');
            if (firstQuote == std::string::npos)
            {
                std::cout << "Invalid Argument format" << std::endl;
                return 1;
            }
            size_t secondQuote = line.find('"', firstQuote + 1);
            if (secondQuote == std::string::npos)
            {
                std::cout << "Invalid Argument format" << std::endl;
                return 1;
            }

            std::string content = line.substr(firstQuote, secondQuote - firstQuote + 1);

            size_t pos = secondQuote + 1;
            while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos])))
                ++pos;

            if (pos >= line.size())
            {
                std::cout << "Invalid Argument format" << std::endl;
                return 1;
            }

            std::string filename = line.substr(pos);

            util.write(content, filename);
        }

        else if (in != "exit")
        {
            std::cout << "Unknown command " << in << std::endl;
        }

    } while (in != "exit");

    return 0;
}
