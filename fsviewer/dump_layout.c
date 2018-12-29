#include <stdio.h>
// not sure about the include path, double check
// see https://oss.oracle.com/projects/ocfs2/dist/documentation/disklayout.pdf for more
#include <linux/fs.h>
#include <fs/ext2/ext2.h>
#include <linux/ext2_fs.h>

//static int ext2_fill_super(struct super_block *sb, void *data, int silent){
//
//  struct buffer_head * bh;
//  struct ext2_sb_info * sbi;
//  struct ext2_super_block * es;
//  struct inode *root;
//  unsigned long block;
//  unsigned long sb_block = get_sb_block(&data);
//  unsigned long logic_sb_block;
//  unsigned long offset = 0;
//  unsigned long def_mount_opts;
//  long ret = -EINVAL;
//  //default block size is 1024B
//  int blocksize = BLOCK_SIZE;
//  int db_count;
//  int i, j;
//  __le32 features;
//  int err;
//
//  //allocate memory ext2_super_block in memory
//  sbi = kzalloc(sizeof(*sbi), GFP_KERNEL);
//  if (!sbi)
//    return -ENOMEM;
//
//  sbi->s_blockgroup_lock = kzalloc(sizeof(struct blockgroup_lock), GFP_KERNEL);
//  if(!sbi->s_blockgroup_lock){
//    kfree(sbi);
//    return -ENOMEM;
//  }
//
//  //sb is vfs super_block
//  //sb->s_fs_info is specific file system super block
//  sb->s_fs_info = sbi;
//  sbi->s_sb_lock = sb_block;
//
//  spin_lock_init(&sbi->s_lock);
//  /*
//  * See what the current blocksize for the device is, and
//  * use that as the blocksize.  Otherwise (or if the blocksize
//  * is smaller than the default) use the default.
//  * This is important for devices that have a hardware
//  * sectorsize that is larger than the default.
//  */
//  blocksize = sb_min_blocksize(sb, BLOCK_SIZE);
//  if(!blocksize){
//    ext2_msg(sb, KERN_ERR, "error:unable to set blocksize");
//    goto failed_sbi;
//  }
//  if(blocksize != BLOCK_SIZE){
//    logic_sb_block = (sb_block * BLOCK_SIZE) / blocksize;
//    offset = (sb_block * BLOCK_SIZE) % blocksize;
//  }
//  else{
//    logic_sb_block = sb_block;
//  }
//
//  //read block @logic_sb_block contain super block
//  if(!(bh = sb_bread(sb, logic_sb_block))){
//    ext2_msg(sb, KERN_ERR, "error:unable to read superblock");
//    goto failed_sbi;
//  }
//
//  es = (struct ext2_super_block *)(((char *)bh->b_data) + offset);
//  sbi->s_es = es;
//
//}

void dump_layout(char *device_name)
{
  FILE *fp = fopen(device_name, "rb");
  // disclaimer: i don't know the exact layout of ext2 file system, please refer to the documentation or book
  // suppose the superblock is located at the first 1024 bytes - 2048 bytes (i assume block size is 1024, which may not
  // be the case! google for how to get block size!)

  fseek(fp, 1024, SEEK_SET); // offset the file pointer to the right address
  char buffer[1024];
  if(fread(buffer, 1, 1024, fp) != 1024)
    fprintf(stderr, "device file too small, cannot read super block");

  // now parse the binary data
  // TODO: this might not be the correct struct to cast to, refer to documentation!
  struct super_block *super_block_data = (struct super_block *) buffer; // force cast the binary char array to struct super_block
  // now read from super_block_data, etc.

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