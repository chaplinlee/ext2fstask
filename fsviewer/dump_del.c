#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>

#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */

static unsigned int block_size = 0;        /* block size (to be calculated) */

int isPower(int n);

int main(int argc, char * argv[])
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

  /*
  ext2_inode
      i_ctime 创建时间
      i_dtime 删除时间
      i_size  文件长度（字节）
  */

  int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;
  struct ext2_inode inode;
  int i, j, delete_num = 0, check_num = 0;
  int inode_OFFSET = 0;

  // 依次访问每个块组
  for (i = 0; i < group_num; i++)
  {
    if (i == 0)
      inode_OFFSET = (1 + 1 + 2 + super.s_reserved_gdt_blocks + 1 + 1)*block_size;//268288
    else if (isPower(i) == 1)
      inode_OFFSET = ((super.s_blocks_per_group *i + 1) + (1 + 2 + super.s_reserved_gdt_blocks + 1 + 1))*block_size;//268288
    else
      inode_OFFSET = ((super.s_blocks_per_group *i + 1) + (1 + 1))*block_size;

    //定位到 inode table 读出删除时间
    lseek(fd, inode_OFFSET, SEEK_SET);
    for (j = 0; j < 2032; j++)
    {
      read(fd, &inode, sizeof(inode));
      if (inode.i_dtime != 0)
        delete_num++;
      check_num++;
    }
  }
  printf("检查文件节点数：%d\n 被删除节点数：%d\n", check_num, delete_num);

  close(fd);
  exit(0);
}
int isPower(int n)
{
  int i;
  int a[12] = { 0, 1, 3, 9, 27, 81, 243, 5, 25, 125, 7, 49 };
  for (i = 0; i < 12; i++)
    if (n == a[i])
      return 1;
  return 0;
}