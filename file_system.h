struct inode {
  long size;
  long indirect;
  long direct[12];
  char file_name[100];
  long self; //own inode number
  //long pd; //parent directory used to make cd .. easier
};
  
struct file_system {
  struct inode inode_list[1000];
  char free_list[3125];
  struct *work_dir;
  //some sort of structure to maintain full path for when cd .. is observed
};

void set_page_free(struct file_system *fs, long num) {
//or with 1
  int div_eight=num/8;
  int mod_eight=num%8;
  f_s->free_list[div_eight]&=~(1<<(7-mod_eight));
}

int find_first_free_page(struct file_system *fs)
{
//finds first free page in free list
  int pos = 0; //position in array with an open array
  int i;
  int j=0; //position of first zero in number

  for (i=0; i < 3125; i++)
  {
    if (fs->free_list[i]!=255)
    {
      pos = i;
      break;
    }
  }

  while ((fs->free_list[pos]>>j)%2==1){
    j++;
  }

  set_page_used(fs, pos*8+j); //set the page as taken
  //should we just move the set_page_used function in here?

  return pos*8+j; //first free page number 
}

void set_page_used(struct file_system *fs, long num) {
// and with ~1
  int div_eight=num/8;
  int mod_eight=num%8;
  f_s->free_list[div_eight]|=(1<<(7-mod_eight));
}
