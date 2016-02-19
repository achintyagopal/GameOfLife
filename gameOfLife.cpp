#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include "Pair.hpp"
#include <string>
#include <stdexcept>
#include <sys/ioctl.h>

using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::ofstream;
using std::ifstream;
using std::out_of_range;


void swap(bool value, int x, int y) {
  printf("\033[%d;%dH", x+1,2*y+1);

  if(value == false){
    cout << "\033[0m";
  } else {
    cout << "\033[31m";
    cout << "\033[1m";
  }
  
  cout << "\u2588";
  cout << "\u2588";
}

void print(ostream& stream, vector<vector<bool>> array){
  for(int i = 0; i < array.size(); i++){
    vector<bool> row = array[i];
    for(int j = 0; j < row.size(); j++){
      
      if(row[j] == false){
	cout << "\033[0m";
      } else {
	cout << "\033[31m";
	cout << "\033[1m";
      }
      
      cout << "\u2588";
      cout << "\u2588";
    }
    cout << "\n";
  }
  cout << "\033[0m";
}

int main(int argc, char** argv){

  cout << "\033[2J\033[1;1H";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  
  int numRows = w.ws_row - 1;
  int numCols = w.ws_col / 2 - 1;
  int numRandPts = 1000;
  
  vector<vector<bool>> life;
  for(int i = 0; i < numRows; i++){
    vector<bool> row;
    life.push_back(row);
    for(int j = 0; j < numCols; j++){
      life[i].push_back(false);
    } 
  }
  
  srand(time(NULL));
  
  vector<Pair> initialPts;
  if (argc == 1){
    
    for(int i = 0; i < numRandPts; i++){
      int x = rand() % numRows;
      int y = rand() % numCols;
      Pair pair(x, y);
      initialPts.push_back(pair);
    }

  } else {
    string filename = argv[1];
    ifstream programfile(filename);

    if(!programfile.is_open()){
      std::cerr << "could not open file\n";
      return 1;
    }

    while(!programfile.eof()){
      int x;
      int y;
      programfile >> x;
      programfile >> y;
      if(programfile.eof()){
	break;
      }

      Pair pair(x,y);
      initialPts.push_back(pair);
    }
    programfile.close();
  }

  int alive = 0;
  
  for(int i = 0; i < initialPts.size(); i++){
    life[initialPts[i].getX()][initialPts[i].getY()] = true;
    alive++;
  }
  
  print(cout, life);
  while(alive > 0){
    usleep(100000);
    
    vector<vector<bool>> temp(life);
    for(int i = 0; i < numRows; i++){
      for(int j = 0; j < numCols; j++){
	
	int neighbors = 0;
	for(int k = -1; k <= 1; k++){
	  for(int l = -1; l <= 1; l++){
	    if (k == 0 && l == 0){
	      continue;
	    }
	    if(i + k == -1){
	      continue;
	    }
	    if(i + k == numRows){
	      continue;
	    }
	    if(j + l == -1){
	      continue;
	    }
	    if(j + l == numCols) {
	      continue;
	    }
	    
	    bool value = temp[i + k][j + l];
	    if (value == true) {
	      neighbors++;
	    }
	  }
	}
	
	if (temp[i][j]){
	  if (neighbors < 2){
	    //die
	    life[i][j] = false;
	    swap(false, i, j);
	    alive--;
	  } else if (neighbors > 3) {
	    //die
	    life[i][j] = false;
	    swap(false, i, j);
	    alive--;
	  }
	} else {
	  if (neighbors == 3){
	    //live
	    life[i][j] = true;
	    swap(true, i, j);
	    alive++;
	  }
	}
      }
    }
    printf("\033[%d;%dH", numRows+4,1);
    cout.flush();
  }  
}
