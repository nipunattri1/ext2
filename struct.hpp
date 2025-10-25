#pragma once
#include <cstdint>
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

};
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

struct block_group_decriptor
{
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t addr_inode_table;
    uint16_t unall_blocks;
    uint16_t unall_inodes;
    uint16_t dir_count;
};
