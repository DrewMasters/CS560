/******************
  CS 560- PA #1
  Basic shell
  By: Joe Dorris
      Drew Masters
 *****************/

#include <iostream>
#include "file_system.h"

using namespace std;

int main(int argc, char *argv[]) {
  string prompt = "sh: ";
  string temp;
  cout << prompt;
  while(cin >> temp) {
    if(temp=="mkfs") {
      cout << "good job Joe\n";
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
