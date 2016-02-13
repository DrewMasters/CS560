#include "file_system.h"

void set_page_free(struct file_system *fs, long num) {
//or with 1
  int div_eight=num/8;
  int mod_eight=num%8;
  fs->free_list[div_eight]&=~(1<<(7-mod_eight));
}

int find_first_free_page(struct file_system *fs)
{
//finds first free page in free list
  int pos = 0; //position in array with an open array
  int i;
  int j=0; //position of first zero in number

  for (i=0; i < NUM_FREE_LIST_BYTES; i++)
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
  //fs->free_list[pos]|=(1<<(7-j)); //set page as being used

  return pos*8+j; //first free page number 
}

void set_page_used(struct file_system *fs, long num) {
// and with ~1
  int div_eight=num/8;
  int mod_eight=num%8;
  fs->free_list[div_eight]|=(1<<(7-mod_eight));
}

struct file_system connect_fs(char *file_name){
  //creates a file system out of the file specified
  //called from sh.c either with specified file from 
  //command line or from fs_mkfs()
}
