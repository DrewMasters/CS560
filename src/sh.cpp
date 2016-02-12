/******************
  CS 560- PA #1
  Basic shell
  By: Joe Dorris
      Drew Masters
 *****************/

#include <iostream>
extern "C" {
#include "file_system.h"
}

using namespace std;

int main(int argc, char *argv[]) {
  string prompt = "sh: ";
  string temp;
  cout << prompt;
  while(cin >> temp) {
    if(temp=="mkfs") {
      cout << "making file system" << endl;
    }
    else if(temp=="exit") {
      break;
    }
    else {
      cout << "Unrecognized" << endl;
    }
    cout << prompt;
  }
  return 0;
}
