#define MAX_FILE_NAME_LEN 100
#define MAX_SIZE_DIRECTORY 100
#define NUM_DIRECT_LINKS 12
#define NUM_INODES 1000 
#define NUM_FREE_LIST_BYTES 3125

struct directory {
  char files[MAX_SIZE_DIRECTORY][MAX_FILE_NAME_LEN];
  long inodes[MAX_SIZE_DIRECTORY];
};

struct inode {
  long self;
  char file_type; //0 is regular, 1 is direcotry
  long size;
  long indirect;
  long direct[NUM_DIRECT_LINKS];
  char file_name[MAX_FILE_NAME_LEN];
  //long pd; //parent directory used to make cd .. easier
};
  
struct file_system {
  struct inode inode_list[NUM_INODES];
  unsigned char free_list[NUM_FREE_LIST_BYTES];
  //struct *work_dir;
  //some sort of structure to maintain full path for when cd .. is observed
};

void set_page_free(struct file_system *fs, long num);
int find_first_free_page(struct file_system *fs);
void set_page_used(struct file_system *fs, long num);
struct file_system connect_fs(char *file_name);
