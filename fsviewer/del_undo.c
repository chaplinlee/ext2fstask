#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>

#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */
#define FD_DEVICE "/dev/loop0"               /* the floppy disk device */

static unsigned int block_size = 0;        /* block size (to be calculated) */

int isPower(int n)
{
  int i;
  int a[12] = { 0, 1, 3, 9, 27, 81, 243, 5, 25, 125, 7, 49 };
  for (i = 0; i < 12; i++)
    if (n == a[i])
      return 1;
  return 0;
}

int main(int argc, char **argv)
{
    char *FD_DEVICE = argv[1];

    struct ext2_super_block super;
    int fd;

    // open文件 打开失败检测

    if ((fd = open(FD_DEVICE, O_RDONLY)) < 0) {
      perror(FD_DEVICE);
      exit(1);  /* error while opening the floppy device */
    }

    // read super_block

    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));

    // 检测文件系统
    if (super.s_magic != EXT2_SUPER_MAGIC) {
      fprintf(stderr, "Not a Ext2 filesystem\n");
      exit(1);
    }

    block_size = 1024 << super.s_log_block_size;

    for(i = 0; i < group_num; i++)
    {
        if (i == 0)
            inode_OFFSET = (1 + 1 + 2 + super.s_reserved_gdt_blocks + 1 + 1) * block_size;//268288
        else if (isPower(i) == 1)
            inode_OFFSET = ((super.s_blocks_per_group * i + 1) + (1 + 2 + super.s_reserved_gdt_blocks + 1 + 1)) * block_size;//268288
        else
            inode_OFFSET = ((super.s_blocks_per_group * i + 1) + (1 + 1)) * block_size;

        lseek(fd, inode_OFFSET, SEEK_SET);


    }

}
