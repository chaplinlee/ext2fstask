#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ext2fs/ext2_fs.h>
#include "utils.h"

#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */

int dump_del(char *device)
{
    int fd = -1;

    // open device file
    if ((fd = open(device, O_RDONLY)) < 0) {
        fprintf(stderr, "Cannot open device %s", device);
        return 1;
    }

    // read super_block
    struct ext2_super_block super;
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));

    // check if it is ext2 filesystem
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        return 1;
    }

    unsigned int block_size = 1024 << super.s_log_block_size;
    // number of block groups
    int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;

    struct ext2_inode inode;
    int delete_num = 0, check_num = 0;
    int inode_offset = 0;

    // iterate over the group and search inode table
    for (int group_index = 0; group_index < group_num; group_index++)
    {
        // find offset to inode table
        if (group_index == 0)
            inode_offset = (1 + 1 + 2 + super.s_reserved_gdt_blocks + 1 + 1) * block_size; //268288
        else if (is_power(group_index, 3) || is_power(group_index, 5) || is_power(group_index, 7))
            inode_offset = ((super.s_blocks_per_group * group_index + 1) + (1 + 2 + super.s_reserved_gdt_blocks + 1 + 1)) * block_size;//268288
        else
            inode_offset = ((super.s_blocks_per_group * group_index + 1) + (1 + 1)) * block_size;

        // read inode table
        lseek(fd, inode_offset, SEEK_SET);
        for (int j = 0; j < 2032; j++) // question: what is 2032?
        {
            read(fd, &inode, sizeof(inode));
            if (inode.i_dtime != 0)
            {
                const long i_dtime = (const long)inode.i_dtime;
                struct tm *tm_time = localtime(&i_dtime);
                char str_buffer[1024];
                strftime(str_buffer, sizeof(str_buffer), "%a, %d %b %Y %T UTC%z", tm_time);

                printf("group %d\n", group_index);
                printf("deleted inode time:%s\n",str_buffer);

                delete_num++;
            }
            check_num++;
        }
    }
    printf("检查文件节点数：%d\n 被删除节点数：%d\n", check_num, delete_num);
    close(fd);
    return 0;
}

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Not enough argument.\n\nUsage: %s [device]", argv[0]);
        return 1;
    }
    return dump_del(argv[1]);
}
