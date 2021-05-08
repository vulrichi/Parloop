#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <thread>
#include <mutex>

#include "seq_loop.hpp"

#include "seq_loop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

	float f1(float x, int intensity);
	float f2(float x, int intensity);
	float f3(float x, int intensity);
	float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {
  
  if (argc < 7) { //verifying correct # of inputs
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  auto start = std::chrono::system_clock::now(); //starting the clock to measure performance
  
  int functionid = atoi(argv[1]);	//functionid
  int a = atoi(argv[2]);		//lower bound of the integral
  int b = atoi(argv[3]);		//upper bound of the integral
  float n = atof(argv[4]);		//number of points to compute the approx. of the integral
  int intensity = atoi(argv[5]);	//intensity of the function
  int nbthreads = atoi(argv[6]);	//number of threads
  
  float integral=0;
  
  SeqLoop sl; //seg_loop.hpp has SeqLoop class with parfor member.

    if(functionid == 1){ //calling parfor loop, sending the numerical integration function
      sl.parfor(0, n, nbthreads,
	        [&](int i) -> void{
	          integral += ((b-a)/n) * f1(a+((i+.5)*((b-a)/n)), intensity);
	        }  
	);  	
  } else if(functionid == 2){
      sl.parfor(0, n, nbthreads,
	        [&](int i) -> void{
	          integral += ((b-a)/n) * f2(a+((i+.5)*((b-a)/n)), intensity);
	        }  
	);  	
  } else if(functionid == 3){
      sl.parfor(0, n, nbthreads,
	        [&](int i) -> void{
	          integral += ((b-a)/n) * f3(a+((i+.5)*((b-a)/n)), intensity);
	        }  
	);  	
  } else if(functionid == 4){
      sl.parfor(0, n, nbthreads,
	        [&](int i) -> void{
	          integral += ((b-a)/n) * f4(a+((i+.5)*((b-a)/n)), intensity);
	        }  
	);  	
    }
  
  
  std::cout<<integral<<std::endl; 
   
  auto finish = std::chrono::system_clock::now();
  std::cerr<<std::chrono::duration_cast<std::chrono::microseconds> (finish - start).count();
  return 0;
}
