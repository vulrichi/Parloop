#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <thread>
#include <mutex>

class SeqLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  //Parfor Structure should include (tls array, before, middle, join, after)
        template<typename TLS>	
    void parfor(size_t beg, size_t end, size_t inc,size_t granularity,
		std::function<void(TLS &)> before,
		std::function<void(int, TLS &)> f,
		std::function<void(TLS &)> after){
	
	std::atomic<int> i = beg;
        std::mutex afterMutex;
        std::thread threads[inc];
        
        for(int t=i; t<inc; ++t){
          threads[t] = std::thread(
                	[=, &f, &i, &before, &after, &afterMutex](){
				TLS tls; 
		                before(tls);
				  while (true){
			      	    int chunkend = i.fetch_add(granularity);
			      	    int chunkstart = chunkend-granularity;
	     		      	    if (chunkstart> end)
					return;
			      	    for (int j=chunkstart; j<chunkend && j<end; ++j)
		                	f(j);
		        		}
		        	afterMutex.lock();
				after(tls);
        			afterMutex.unlock();
                		}
            );
        }

    }
};

#endif
