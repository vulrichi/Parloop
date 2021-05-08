#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <thread>
<<<<<<< HEAD
=======
#include <mutex>
>>>>>>> c8b8f51a3dffc0e182dc65a538d7a8faf7ed06d3

class SeqLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  
<<<<<<< HEAD
  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    std::thread threads[inc];
    for(int t=beg; t<inc; ++t){
      for (size_t i=t; i<end; i+= inc) {
        threads[i]= std::thread(f, i);
      }
    }
    for (size_t i=beg; i<end; ++i) {
      threads[i].join();
    }
  }

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
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
	       std::function<void(TLS&)> before,
	       std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after,
	       int nb
	       ) {
    TLS tls;
    std::thread threads[nb];
    before(tls);    
    for (size_t i=beg; i<end; i+= increment) {
      threads[i]= std::thread(f, i, tls);
    }
    for (size_t i=beg; i<end; i+= increment) {
      threads[i].join();
    }
    after(tls);
  }
  
=======
    void parfor(size_t beg, size_t end, size_t inc,
        	std::function<void(int)> f){
        std::thread threads[inc];
        for (int t = beg; t < inc; ++t) {
            threads[t] = std::thread(
                [=, &f]()
                {
                    for (size_t i = t; i < end; i += inc) {
                        f(i);
                    }
                }
            );
        }
        for (size_t i = 0; i < inc; ++i) {
            threads[i].join();
        }
    }

        template<typename TLS>
    void parfor(size_t beg, size_t end, size_t inc,
		std::function<void(TLS &)> before,
		std::function<void(int, TLS &)> f,
		std::function<void(TLS &)> after,
        	int nb){
        
        std::mutex afterMutex;
        std::thread threads[inc];
        for (int t = beg; t < inc; ++t) {
            threads[t] = std::thread(
                [=, &f, &before, &after, &afterMutex]()
                {
                    TLS tls;
                    before(tls);
                    for (size_t i = t; i < end; i += inc) {
                        f(i, tls);
                    }
                    afterMutex.lock();
                    after(tls);
                    afterMutex.unlock();
                }
            );
        }
    }
>>>>>>> c8b8f51a3dffc0e182dc65a538d7a8faf7ed06d3
};

#endif
