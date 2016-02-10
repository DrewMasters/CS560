struct inode {
  long size;
  long indirect;
  long direct[12];
  char file_name[100];
};
  
struct file_system {
  struct inode inode_list[1000];
  char free_list[3125];
};

void set_page_free(struct file_system *, long num) {
//or with 1
  div_eight=num/8;
  mod_eight=num%8;
  file_system->inode_list[div_eight]&=~(1<<(7-mod_eight));
}

void set_page_used(struct file_system *, long num) {
// and with ~1
  div_eight=num/8;
  mod_eight=num%8;
  file_system->inode_list[div_eight]|=(1<<(7-mod_eight));
}
