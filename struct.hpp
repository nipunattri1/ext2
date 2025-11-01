#ifndef STRUCT_H
#define STRUCT_H
#include <cstdint>
#pragma pack(push, 1)

struct ext_super_block
{
    uint32_t first_inode;
    uint16_t inode_size;
    uint16_t sb_block;
    uint32_t opt_feat;
    uint32_t req_feat;
    uint32_t read_only_feat;
    uint8_t file_sys_id[16];
    uint8_t vol_name[16];
    uint8_t vol_path[64];
    uint32_t comp_algo;
    uint8_t pre_allo_files;
    uint8_t pre_allo_dir;
    uint16_t _;
    uint8_t journal_id[16];
    uint32_t journal_inode;
    uint32_t jourrnal_device;
    uint32_t inode_list_head;
};
#pragma pack(pop)
#pragma pack(push, 1)

struct misc_info
{
    unsigned int block_size;
    unsigned int frag_size;
    unsigned int group_count;
    unsigned int root_inode;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct super_block
{
    uint32_t inode_count;
    uint32_t block_count;
    uint32_t reserve_count;
    uint32_t unallo_blocks;
    uint32_t unallo_inodes;
    uint32_t first_data_block;
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
    ext_super_block ext_sb;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct block_group_decriptor
{
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t addr_inode_table;
    uint16_t unall_blocks;
    uint16_t unall_inodes;
    uint16_t dir_count;
    uint16_t pad;
    uint32_t extra[3];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct i_osd2
{
    uint8_t h_i_frag;
    uint8_t h_i_fsize;
    uint16_t h_i_mode_high;
    uint16_t h_i_uid_high;
    uint16_t h_i_gid_high;
    uint32_t h_i_author;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct inode
{
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_flags;
    uint32_t i_osdl;
    uint32_t i_block[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;
    i_osd2 i_osd2_val;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct directory
{
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[256];
};
#pragma pack(pop)

#endif