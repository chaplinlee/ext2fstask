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
	close(fd);

	// 检测文件系统是否为ext2
	if (super.s_magic != EXT2_SUPER_MAGIC) {
		fprintf(stderr, "Not a Ext2 filesystem\n");
		exit(1);
	}
	block_size = 1024 << super.s_log_block_size;

	/*
	ext2_super_block
	s_blocks_per_group      块组大小（块）
	s_reserved_gdt_blocks      reserved_GDT大小
	s_inodes_per_group              每组节点数
	super.s_inode_size              节点大小（字节）
	*/

	int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;
	struct ext2_inode inode;
	int i, j, a, b, delete_num = 0, check_num = 0;
	int mark = 1;  //访问标记

				   // 每个块组中 inode table 所占的块数
	int block_inodes_per_group = super.s_inodes_per_group*super.s_inode_size / block_size;

	// GDT所占的块数
	struct ext2_group_desc group_desc;
	int GDT_size = sizeof(group_desc)*group_num / block_size + 1;

	printf("\n 显示布局: \n\n");

	// 依次访问每个块组
	for (i = 0; i < group_num; i++)
	{
		mark = 1 + i * super.s_blocks_per_group;
		if (isPower(i) == 1)
		{
			printf("Group %d:          开始     结束     长度\n", i);
			a = mark, b = mark + 1 - 1;
			printf("Superblock       :  %d     %d     %d  \n", a, b, 1);
			a = b + 1, b = a + GDT_size - 1;
			printf("GDT              :  %d     %d     %d  \n", a, b, GDT_size);
			a = b + 1, b = a + super.s_reserved_gdt_blocks - 1;
			printf("Reserved GDT     :  %d     %d     %d  \n", a, b, super.s_reserved_gdt_blocks);
			a = b + 1, b = a + 1 - 1;
			printf("Data block bitmap:  %d     %d     %d  \n", a, b, 1);
			a = b + 1, b = a + 1 - 1;
			printf("Inode bitmap     :  %d     %d     %d  \n", a, b, 1);
			a = b + 1, b = a + block_inodes_per_group - 1;
			printf("Inode table      :  %d     %d     %d  \n", a, b, block_inodes_per_group);
			a = b + 1, b = mark + super.s_blocks_per_group - 1;
			if (b > super.s_blocks_count - 1)
				b = super.s_blocks_count - 1;
			printf("Data block       :  %d     %d     %d  \n", a, b, b - a + 1);
		}
		else
		{
			printf("Group %d           开始     结束     长度\n", i);
			printf("Superblock       :    -         -         -  \n");
			printf("GDT              :    -         -         -  \n");
			printf("Reserved GDT     :    -         -         -  \n");

			a = mark, b = mark + 1 - 1;
			printf("Data block bitmap:  %d     %d     %d  \n", a, b, 1);
			a = b + 1, b = a + 1 - 1;
			printf("Inode bitmap     :  %d     %d     %d  \n", a, b, 1);
			a = b + 1, b = a + block_inodes_per_group - 1;
			printf("Inode table      :  %d     %d     %d  \n", a, b, block_inodes_per_group);
			a = b + 1, b = mark + super.s_blocks_per_group - 1;
			if (b > super.s_blocks_count - 1)
				b = super.s_blocks_count - 1;
			printf("Data block       :  %d     %d     %d  \n", a, b, b - a + 1);
		}
		printf("\n");
	}

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
