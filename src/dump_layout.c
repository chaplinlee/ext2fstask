#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>
#include "utils.h"


#define BASE_OFFSET 1024                   // locates beginning of the super block (first group)

int dump_layout(char *device)
{
    int fd = -1;
    // open device
    if ((fd = open(device, O_RDONLY)) < 0) {
        fprintf(stderr, "Cannot open device %s", device);
        return 1;
    }

    // read superblock
    struct ext2_super_block super;
    // offset to the superblock
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));
    close(fd);

    // is ext2 func
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        return 1;
    }

    unsigned int block_size = 1024 << super.s_log_block_size;
    int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;
    // inode table size in each group
    int block_inodes_per_group = super.s_inodes_per_group * super.s_inode_size / block_size;
    // GDT size in each group
    int GDT_size = sizeof(struct ext2_group_desc) * group_num / block_size + 1;

    // print out required message
    for (int group_index = 0; group_index < group_num; group_index++)
    {
        int block_cursor = group_index * super.s_blocks_per_group + 1;

        // print header
        printf("| %-5s %-11d | %6s | %6s | %6s |\n", "Group", group_index, "Start", "End", "Length");
        printf("|%-17s|%6s|%6s|%6s|\n", ":------------------", ":------:", ":------:", ":------:");

        // special items (Superblock / GDP / Reserved GDT) may not appear in certain groups
        // this group contains backup superblock
        if(is_power(group_index, 3) || is_power(group_index, 5) || is_power(group_index, 7))
        {
            printf("| %-17s | %6d | %6d | %6d |\n", "Superblock", block_cursor, block_cursor, 1);
            block_cursor ++;
            printf("| %-17s | %6d | %6d | %6d |\n", "GDT", block_cursor, block_cursor, GDT_size);
            block_cursor += GDT_size;
            printf("| %-17s | %6d | %6d | %6d |\n", "Reserved GDT", block_cursor, block_cursor, super.s_reserved_gdt_blocks);
            block_cursor += super.s_reserved_gdt_blocks;
        }
        else
        {
            printf("| %-17s | %6s | %6s | %6s |\n", "Superblock", "N/A", "N/A", "N/A");
            printf("| %-17s | %6s | %6s | %6s |\n", "GDT", "N/A", "N/A", "N/A");
            printf("| %-17s | %6s | %6s | %6s |\n", "Reserved GDT", "N/A", "N/A", "N/A");
        }
        printf("| %-17s | %6d | %6d | %6d |\n", "Data block bitmap", block_cursor, block_cursor, 1);
        block_cursor ++;
        printf("| %-17s | %6d | %6d | %6d |\n", "Inode bitmap", block_cursor, block_cursor, 1);
        block_cursor ++;
        printf("| %-17s | %6d | %6d | %6d |\n", "Inode table", block_cursor, block_cursor + block_inodes_per_group - 1, block_inodes_per_group);
        block_cursor += block_inodes_per_group;
        int data_block_end = (group_index + 1) * super.s_blocks_per_group;
        if(data_block_end > super.s_blocks_count - 1)
            data_block_end = super.s_blocks_count - 1;
        printf("| %-17s | %6d | %6d | %6d |\n", "Data block", block_cursor, data_block_end, data_block_end - block_cursor + 1);
        printf("\n");
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Not enough argument.\n\nUsage: %s [device]", argv[0]);
        return 1;
    }
    return dump_layout(argv[1]);
}