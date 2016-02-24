#include <stdio.h>

#define MAX_FILE_NAME_LEN 100
#define MAX_SIZE_DIRECTORY 90
#define NUM_DIRECT_LINKS 12
#define NUM_INODES 1000 
#define NUM_FREE_LIST_BYTES 3125
#define DISK_OFFSET 235136 
#define DISK_SIZE 100000000
#define PAGE_SIZE 10240
#define NUM_FILE_DESC 12

struct directory {
  char files[MAX_SIZE_DIRECTORY][MAX_FILE_NAME_LEN];
  long inodes[MAX_SIZE_DIRECTORY];
};

struct inode {
  char in_use; //0 is not in use, 1 is in use 
  //long self;
  char file_type; //0 is regular, 1 is direcotry
  long size;
  long indirect;
  long direct[NUM_DIRECT_LINKS];
  char file_name[MAX_FILE_NAME_LEN];
  //long pd; //parent directory used to make cd .. easier
};

struct file_descriptor {
  struct inode *i;
  char type; //0 is read //1 is write 
  char in_use; //0 is not in use, 1 is in use 
  long in_offset; //starts at the beginning of file used to point where things are suppose to be read from or wrote to
  long out_offset; //actual position of file in "hard disk"
};
  
struct file_system {
  struct inode inode_list[NUM_INODES];
  unsigned char free_list[NUM_FREE_LIST_BYTES];
  struct file_descriptor fd[NUM_FILE_DESC];
  int root_idx;
  int cur_idx;
};


void set_page_free(struct file_system *fs, long num);
void free_page(struct file_system *fs, long num);
int find_first_free_page(struct file_system *fs);
void set_page_used(struct file_system *fs, long num);
struct file_system connect_fs(char *file_name);
int get_inode(struct file_system * F);
int get_fd(struct file_system * F);
void free_fd(struct file_system * F,int fd);
