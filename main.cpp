#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <format>

// uint32_t extractInfo(std::vector<Byte> &buffer, int startByte, int size);
// void superBlock(std::vector<u_short> &buffer, std::ofstream &out);

// class bufferUtils
// {


// public:
//     void setBuffer(std::vector<u_short> &in)
//     {
//         buffer = in;
//     }
//     uint32_t extract(int startByte, int size);
//     time_t extract_t(int startByte, int size);
// };

// uint32_t bufferUtils::extract(int startByte, int size)
// {
//     int base = 1023 + startByte;
//     uint32_t ans = 0;
//     for (u_short i = 0; i < size; i++)
//     {
//         ans = (ans << 8);
//         ans = ans | (buffer[base + i]);
//     }
//     return ans;
// }


// time_t bufferUtils::extract_t(int startByte, int size)
// {
//     int base = 1023 + startByte;
//     time_t ans = 0;
//     for (u_short i = 0; i < size; i++)
//     {
//         ans = (ans << 8);
//         ans = ans | (buffer[base + i]);
//     }
//     return ans;
// } 


struct super_block
{
    uint32_t inode_count;
    uint32_t block_count;
    uint32_t reserve_count;
    uint32_t unallo_blocks;
    uint32_t unallo_inodes;
    uint32_t super_block_no;
    uint32_t block_size;
    uint32_t frag_size;
    uint32_t group_block_count;
    uint32_t groyp_frag_count;
    uint32_t group_inode_count;
    uint32_t mount_time;
    uint32_t written_time;
    uint16_t mount_count;
    uint16_t max_mnt;
    uint16_t ext2_sig;
    uint16_t fs_state;
    uint16_t error_act;
    uint16_t minor_ver;
    uint32_t const_check;
    uint32_t force_check;
    uint32_t os_id;
    uint32_t major_ver;
    uint16_t user_id;
    uint16_t group_res;

} sb;

int main(int argc, char const *argv[])
{
    // read inputed file (img)
    std::ofstream log(std::format("{}.log", argv[1]));
    log << "LOG: Opeing the file " << argv[1] << std::endl;
    std::ifstream img(argv[1], std::ios::binary);
    if (!img)
    {
        std::cerr << "ERR: Couldn't Open the file" << std::endl;
        return 1;
    }
    log << "LOG: Successfully opened the file " << argv[1] << std::endl;
    std::vector<u_short> inBuffer(std::istreambuf_iterator<char>(img), {});
    img.seekg(2024);
    img.read(reinterpret_cast<char *>(&sb), sizeof(sb));
    img.close();
    std::clog << "LOG: Closed Img" << std::endl;

    // Read Superblock
    // superBlock(inBuffer, log);

    return 0;
}

// void superBlock(std::vector<u_short> &buffer, std::ofstream &log)
// {
//     bufferUtils img{};
//     img.setBuffer(buffer);

//     // std::cout << "Inodes Count: " << extractInfo(buffer, 0, 4) << std::endl;
//     // std::cout << "Blocks Count: " << extractInfo(buffer, 4, 4) << std::endl;
//     // std::cout << "Blocks Reserved for superuser: " << extractInfo(buffer, 8, 4) << std::endl;
//     // std::cout << "Unallocated Blocks: " << extractInfo(buffer, 12, 4) << std::endl;
//     // std::cout << "Unallocated Inodes: " << extractInfo(buffer, 16, 4) << std::endl;
//     // std::cout << "Superblock Block: " << extractInfo(buffer, 20, 4) << std::endl;
//     // std::cout << "log2 (block size) - 10: " << extractInfo(buffer, 24, 4) << std::endl;
//     // std::cout << "log2 (fragment size) - 10: " << extractInfo(buffer, 28, 4) << std::endl;
//     // std::cout << "Blocks in Block Groups: " << extractInfo(buffer, 32, 4) << std::endl;
//     // std::cout << "Frags in Block Groups: " << extractInfo(buffer, 36, 4) << std::endl;
//     // std::cout << "Inodes in Block Groups: " << extractInfo(buffer, 40, 4) << std::endl;
//     // time_t lastMount = extractInfo(buffer, 44, 4);
//     // log << "LOG: got epoche as: " << lastMount << std::endl;
//     // std::cout << "Last mount: " << std::asctime(std::localtime(&lastMount)) << std::endl;

//     std::cout << "Inodes Count: " << img.extract(0, 4) << std::endl;
//     std::cout << "Blocks Count: " << img.extract(4, 4) << std::endl;
//     std::cout << "Blocks Reserved for superuser: " << img.extract(8, 4) << std::endl;
//     std::cout << "Unallocated Blocks: " << img.extract(12, 4) << std::endl;
//     std::cout << "Unallocated Inodes: " << img.extract(16, 4) << std::endl;
//     std::cout << "Superblock Block: " << img.extract(20, 4) << std::endl;
//     std::cout << "log2 (block size) - 10: " << img.extract(24, 4) << std::endl;
//     std::cout << "log2 (fragment size) - 10: " << img.extract(28, 4) << std::endl;
//     std::cout << "Blocks in Block Groups: " << img.extract(32, 4) << std::endl;
//     std::cout << "Frags in Block Groups: " << img.extract(36, 4) << std::endl;
//     std::cout << "Inodes in Block Groups: " << img.extract(40, 4) << std::endl;
//     time_t lastMount = img.extract_t(44, 4);
//     log << "LOG: got epoche as: " << lastMount << std::endl;
//     std::cout << "Last mount: " << std::asctime(std::localtime(&lastMount)) ;

//     std::cout << "Mount Count: " << img.extract(52, 2) << std::endl;

// }

// Extract byte no base to base+size (starting from 0)
// uint32_t extractInfo(std::vector<Byte> &buffer, int startByte, int size)
// {
//     // base 1023 is byte 1024
//     int base = 1023 + startByte;
//     int32_t ans = 0;
//     for (u_short i = 0; i < size; i++)
//     {
//         ans = (ans << 8);
//         ans = ans | (buffer[base + i]).to_ulong();
//     }
//     return ans;
// }