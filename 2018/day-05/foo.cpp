
#ifdef __linux__ 
#include <string.h>
#include <stdint.h>
#elif _WIN32
#else 
#error "OS not supported!"
#endif

#include <iostream>
#include <vector>
#include <fstream>


using namespace std; 

int solve_1(string input){
  vector<char> acc; 
  char c2 = 0; 

  for(auto chr: input){
    if(c2 == 0 && acc.size() == 0) 
    {
      c2 = chr;
      continue;
    }

    if(c2 == 0){
      if(acc.size() > 0)
        c2 = acc.back();
    }

    if(tolower(chr) == tolower(c2) && chr != c2 ){
      c2 = 0; 
      if(acc.size() > 0)
        acc.pop_back();
      continue; 
    }

    if(acc.size() == 0){
      acc.push_back(c2);
      acc.push_back(chr);
    }else
      acc.push_back(chr);

    c2 = chr;
  }

  return acc.size();
}

int solve_2(string input){

  string buffer; 
  int hasz[200];

  // rudimentary hashmap.
  memset(hasz, 0, 200);
  int minima = INT32_MAX;

  for(auto _alfa: input){
    int cnt =0;
    char alfa = tolower(_alfa);
    char code = alfa - '0';

    if(hasz[code] == 1){
      continue;
    }

    for(auto c : input){
      if(alfa != tolower(c)){
        buffer+= c;
      }
    }
  
    int r = solve_1(buffer);

    minima = (minima < r)? minima:r;
    hasz[code] = 1;
    buffer.clear();
  }

  return minima;
} 

int main(){

  ofstream myfile;
  string input = "dabAcCaCBAcCcaDA";
  string input2;
  std::ifstream infile("input.txt");
  std::getline(infile, input2);

  cout << "solution 1: " << solve_1(input2) << endl;
  cout << "solution 2: " << solve_2(input2) << endl;
  return 0;
}
