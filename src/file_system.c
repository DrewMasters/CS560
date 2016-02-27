#include "file_system.h"

void set_page_free(struct file_system *fs, long num) {
//or with 1
  int div_eight=num/8;
  int mod_eight=num%8;
  fs->free_list[div_eight]&=~(1<<(7-mod_eight));
}

void free_page(struct file_system *fs, long num) {
  num = num - sizeof(struct file_system);
  num = num/PAGE_SIZE;
  set_page_free(fs,num);
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

  /*while ((fs->free_list[pos]>>j)%2==1){
    j++;
  }*/
  while(fs->free_list[pos] & (1<<(7-j))) {
    j++;
  }

  set_page_used(fs, pos*8+j); //set the page as taken
  //should we just move the set_page_used function in here?
  //fs->free_list[pos]|=(1<<(7-j)); //set page as being used
  
  printf("pos: %d\nj: %d\npage location: %d\n",pos,j,(int)sizeof(struct file_system)+(PAGE_SIZE*(pos*8+j)));
  return (int)sizeof(struct file_system) + (PAGE_SIZE*(pos*8+j)); //first free page number 
}

void set_page_used(struct file_system *fs, long num) {
// and with ~1
  int div_eight=num/8;
  int mod_eight=num%8;
  printf("num: %ld\ndiv_eight: %ld\nmod_eight: %ld\n",num,div_eight,mod_eight);
  fs->free_list[div_eight]|=(1<<(7-mod_eight));
}

int get_inode(struct file_system * F) {
  int i;
  for(i=0;i<NUM_INODES;i++) {
    if(F->inode_list[i].in_use==0) {
      F->inode_list[i].in_use = 1;
      return i;
    }
  }
  printf("out of inodes\n");
}
int get_fd(struct file_system * F) {
  int i;
  for(i=0;i<NUM_FILE_DESC;i++) {
    if(F->fd[i].in_use==0) {
      F->fd[i].in_use = 1;
      return i;
    }
  }
  printf("out of fds\n");
  return -1;
}

void free_fd(struct file_system * F,int i) {
  F->fd[i].in_use = 0;
}

extern "C" void fs_exit(FILE * fp, struct file_system * F) {
  fseek(fp, 0, SEEK_SET);
  fwrite(F, sizeof(struct file_system), 1, fp);
  rewind(fp);
}

