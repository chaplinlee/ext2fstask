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

    /*
    ext2_super_block
    super.s_blocks_per_group       块组大小（块）
    super.s_reserved_gdt_blocks    reserved_GDT大小
    super.s_inodes_per_group       每组节点数
    super.s_inode_size             节点大小（字节）
    */

    int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;
    struct ext2_inode inode;

    int a, b;   //visiting cursor
    int mark = 1;   //visiting mark

    // 每个块组中 inode table 所占的块数
    int block_inodes_per_group = super.s_inodes_per_group * super.s_inode_size / block_size;

    // GDT所占的块数
    struct ext2_group_desc group_desc;
    int GDT_size = sizeof(group_desc) * group_num / block_size + 1;

    // 依次访问每个块组
    char *start = "start";
    char *end = "end";
    char *lenth = "lenth";
    char *placeholder = "------";
    char *table_column[] = {"Group", "Superblock", "GDT", "Reserved GDT",
                            "Data block bitmap", "Inode bitmap", "Inode table", "Data block"};
    for (int i = 0; i < group_num; i++)
    {
        mark = 1 + i * super.s_blocks_per_group;
        if (is_power(i, 3) || is_power(i, 5) || is_power(i, 7))
        {
            printf("%-5s %-11d:|%6s|%6s|%6s|\n", table_column[0],i, start, end, lenth);
            a = mark, b = mark + 1 - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[1], a, b, 1);
            a = b + 1, b = a + GDT_size - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[2], a, b, GDT_size);
            a = b + 1, b = a + super.s_reserved_gdt_blocks - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[3], a, b, super.s_reserved_gdt_blocks);
            a = b + 1, b = a + 1 - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[4], a, b, 1);
            a = b + 1, b = a + 1 - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[5], a, b, 1);
            a = b + 1, b = a + block_inodes_per_group - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[6], a, b, block_inodes_per_group);
            a = b + 1, b = mark + super.s_blocks_per_group - 1;
            if (b > super.s_blocks_count - 1)
                b = super.s_blocks_count - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[7], a, b, b - a + 1);
        }
        else
        {
            printf("%-5s %-11d:|%6s|%6s|%6s|\n", table_column[0], i, start, end, lenth);
            for(int j = 1;j < 4;j++)
                printf("%-17s:|%6s|%6s|%6s|\n", table_column[j], placeholder,placeholder,placeholder);

            a = mark, b = mark + 1 - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[4], a, b, 1);
            a = b + 1, b = a + 1 - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[5], a, b, 1);
            a = b + 1, b = a + block_inodes_per_group - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[6], a, b, block_inodes_per_group);
            a = b + 1, b = mark + super.s_blocks_per_group - 1;
            if (b > super.s_blocks_count - 1)
                b = super.s_blocks_count - 1;
            printf("%-17s:|%6d|%6d|%6d|\n", table_column[7], a, b, b - a + 1);
        }
        printf("\n");
    }
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