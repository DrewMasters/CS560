struct inode {
  long size;
  long indirect;
  long direct[12];
  char file_name[100];
};
  

struct file_system {
  struct inode inode_list[1000];
  //free_list
  long memory;
};
