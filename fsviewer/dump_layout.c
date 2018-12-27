#include <stdio.h>
// not sure about the include path, double check
// see https://oss.oracle.com/projects/ocfs2/dist/documentation/disklayout.pdf for more
//#include <linux/fs.h>
//#include <fs/ext2/ext2.h>
//#include <linux/ext2_fs.h>


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