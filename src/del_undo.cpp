#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <time.h>

#define BASE_OFFSET 1024                   /* locates beginning of the super block (first group) */

static unsigned int block_size = 0;        /* block size (to be calculated) */

int isPower(int n);
//char* changeto(long time, char* str_buffer);

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
	ext2_super_block
	s_inode_size  inode大小(B)
	s_inodes_per_group
	ext2_inode
	i_ctime 创建时间
	i_dtime 删除时间
	i_size  文件长度（字节）
	*/

	int group_num = (super.s_blocks_count - 1) / super.s_blocks_per_group + 1;
	struct ext2_inode inode;
	int i, j, delete_num = 0, check_num = 0;
	int inode_OFFSET = 0;
	int inode_seq = 1;      //inode号 从1开始
							// char s[100];

							// GDT所占的块数
	struct ext2_group_desc group_desc;
	int GDT_size = sizeof(group_desc)*group_num / block_size + 1;
	// 依次访问每个块组
	for (i = 0; i < group_num; i++)
	{
		inode_OFFSET = 2048 + sizeof(group_desc)*i;
		lseek(fd, inode_OFFSET, SEEK_SET);
		read(fd, &group_desc, sizeof(group_desc));
		lseek(fd, group_desc.bg_inode_table * block_size, SEEK_SET);
		inode_seq = i * super.s_inodes_per_group;
		for (j = 0; j < super.s_inodes_per_group; j++)
		{
			inode_seq++;
			read(fd, &inode, sizeof(inode));

			if (inode.i_dtime != 0)
			{
				//
				delete_num++;

				// 输出该inode的信息

				printf(" inode %d:\n 文件长度： %u\n iblock:  %x\n", inode_seq, inode.i_size, inode.i_block[0]);

				const long i_ctime = (const long)inode.i_ctime;
				struct tm *tm_time2 = localtime(&i_ctime);
				char str_buffer2[128];
				strftime(str_buffer2, sizeof(str_buffer2), "%a, %d %b %Y %T UTC%z", tm_time2);
				printf("创建时间 ： %-35s \n", str_buffer2);

				const long i_dtime = (const long)inode.i_dtime;
				struct tm *tm_time1 = localtime(&i_dtime);
				char str_buffer1[128];
				strftime(str_buffer1, sizeof(str_buffer1), "%a, %d %b %Y %T UTC%z", tm_time1);
				printf("删除时间 ： %-35s \n", str_buffer1);
			}

			/*
			if (inode_seq == 12)
			{
			printf("iblock: %u\n", inode.i_block[0]);
			printf("size: %u\n", inode.i_size);
			printf("ctime: %u\n", inode.i_ctime);
			}
			*/

			check_num++;
		}
	}
	printf("\n检查文件节点数：%d\n 已删除文件数：%d\n", check_num, delete_num);
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

//char* changeto(const long time, char* str_buffer)
//{
//	const long i_dtime = time;
//	struct tm *tm_time = localtime(&i_dtime);
//	strftime(str_buffer, sizeof(str_buffer), "%a, %d %b %Y %T UTC%z", tm_time);
//	return str_buffer;
//}
