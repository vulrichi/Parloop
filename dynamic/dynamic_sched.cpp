#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atomic>
#include <functional>

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

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <granularity>"<<std::endl;
    return -1;
  }
  auto start = std::chrono::system_clock::now(); //starting the clock to measure performance
  
  int functionid = atoi(argv[1]);	//functionid
  int a = atoi(argv[2]);		//lower bound of the integral
  int b = atoi(argv[3]);		//upper bound of the integral
  int n = atoi(argv[4]);		//number of points to compute the approx. of the integral
  int intensity = atoi(argv[5]);	//intensity of the function
  int nbthreads = atoi(argv[6]);	//number of threads
  int granularity = atoi(argv[6]);	//value of granularity
  std::atomic<float> integral=0;	//integral, atomic to be locksafe.
  
  std::vector<std::function<void(int, float&)>> intfunc[5];
  intfunc[1] =  [&](int i, float& tls) -> void{tls += ((b-a)/n) * f1(a+((i+.5)*((b-a)/n)), intensity); };
  intfunc[2] =  [&](int i, float& tls) -> void{tls += ((b-a)/n) * f2(a+((i+.5)*((b-a)/n)), intensity); };
  intfunc[3] =  [&](int i, float& tls) -> void{tls += ((b-a)/n) * f3(a+((i+.5)*((b-a)/n)), intensity); };
  intfunc[4] =  [&](int i, float& tls) -> void{tls += ((b-a)/n) * f4(a+((i+.5)*((b-a)/n)), intensity); };

  SeqLoop sl; //seg_loop.hpp has SeqLoop class with parfor member.
  sl.parfor(0, n, nbthreads, granularity, 
	[&](float& tls) {tls=0;}, 
	&intfunc[functionid], 
	[&](float& tls) {integral=tls+integral;}
  ); 
//    if(functionid == 1){ //calling parfor loop, sending the numerical integration function
//     sl.parfor(0, n, nbthreads, granularity,
//		[&](float& tls) 	{tls=0;},
//	        func1,  
//	        [&](float& tls) 	{integral=tls+integral;}
//	);  	
//  } else if(functionid == 2){
//      sl.parfor(0, n, nbthreads, granularity,
//		[&](float& tls) 	{tls=0;},
//	        func2,  
//	        [&](float& tls) 	{integral=tls+integral;}
//	);  	
//  } else if(functionid == 3){
//      sl.parfor(0, n, nbthreads, granularity,
//		[&](float& tls)		{tls=0;},
//	        func3, 
//	        [&](float& tls)		{integral=tls+integral;}  
//	);  	
//  } else if(functionid == 4){
//      sl.parfor(0, n, nbthreads, granularity,
//		[&](float& tls) 	{tls=0;},
//	        func4,
//	        [&](float& tls)		{integral=tls+integral;}
//	);  	
//  }  
  
  std::cout<<integral<<std::endl; 
   
  auto finish = std::chrono::system_clock::now();
  std::cerr<<std::chrono::duration_cast<std::chrono::microseconds> (finish - start).count();
  return 0;
}
