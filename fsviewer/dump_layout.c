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

void dump_layout(char *device_name)
{
//  FILE *fp = fopen(device_name, "rb");
//  // disclaimer: i don't know the exact layout of ext2 file system, please refer to the documentation or book
//  // suppose the superblock is located at the first 1024 bytes - 2048 bytes (i assume block size is 1024, which may not
//  // be the case! google for how to get block size!)
//
//  fseek(fp, 1024, SEEK_SET); // offset the file pointer to the right address
//  char buffer[1024];
//  if(fread(buffer, 1, 1024, fp) != 1024)
//    fprintf(stderr, "device file too small, cannot read super block");
//
//  // now parse the binary data
//  // TODO: this might not be the correct struct to cast to, refer to documentation!
//  struct super_block *super_block_data = (struct super_block *) buffer; // force cast the binary char array to struct super_block
//  // now read from super_block_data, etc.
    struct ext2_super_block super;
    struct ext2_group_desc group;

    int fd;

    /* open floppy device */

    if ((fd = open(FD_DEVICE, O_RDONLY)) < 0) {
        perror(FD_DEVICE);
        exit(1);  /* error while opening the floppy device */
    }

    /* read super-block */

    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &super, sizeof(super));

    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }

    block_size =1024 << super.s_log_block_size;

    /* read group descriptor */

    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group, sizeof(group));
    close(fd);

    /* number of inodes per block */
    unsigned int inodes_per_block = block_size / sizeof(struct ext2_inode);

    /* size in blocks of the inode table */
    unsigned int itable_blocks = super.s_inodes_per_group / inodes_per_block;

    /* calculate number of block groups on the disk */
    unsigned int group_count = 1 + (super.s_blocks_count-1) / super.s_blocks_per_group;

    /* calculate size of the group descriptor list in bytes */
    unsigned int descr_list_size = group_count * sizeof(group);
    printf("superblock:%u\n"
           "GDT:%u-%u\n"
           "reserved GDT:%u-%u\n"
           "blocks bitmap block: %u\n"
           "inodes bitmap block: %u\n"
           "inodes table block : %u-%u\n"
           "data block:%u-%u\n"
            ,
           super.s_first_data_block,
           super.s_first_data_block + 1, group.bg_block_bitmap - super.s_reserved_gdt_blocks - 1 ,//1 has something wrong
           group.bg_block_bitmap - super.s_reserved_gdt_blocks, group.bg_block_bitmap - 1,
           group.bg_block_bitmap,
           group.bg_inode_bitmap,
           group.bg_inode_table, group.bg_inode_table + itable_blocks - 1,
           group.bg_inode_table + itable_blocks, block_size - 1
    );
    
}


int main(int argc, char **argv)
{
  // TODO: need MUCH cleaner argument parser
  // this is just an example of concept
  // consider getopt or argp
  // see https://www.gnu.org/software/libc/manual/html_node/Getopt.html
  // and https://www.gnu.org/software/libc/manual/html_node/Argp.html
  // and https://stackoverflow.com/questions/9642732/parsing-command-line-arguments
  if(argc < 2)
    printf("Need device file argument");

  dump_layout(argv[1]);

  return 0;
}