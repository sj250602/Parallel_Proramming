#include <unistd.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "classify.h"

float timedwork(Data &D, const Ranges &R, unsigned int numt)
{
   // starttimer();
   auto begin = std::chrono::high_resolution_clock::now();
   Data D2 = classify(D, R, numt);
   auto end = std::chrono::high_resolution_clock::now();
   auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
   // return(1e-6 * elapsed.count());
   return(1e-6 * (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)).count());
   // return mssince();
}

void repeatrun(unsigned int reps, Data &D, const Ranges &R, unsigned int numt)
{ // Repeat the same experiment. Time each rep. D, R, and numt remain fixed.
   int it;
   float ms, tms = 0, minms = 1e7;
   for(it=0; it<reps; it++) {
      tms += ms = timedwork(D, R, numt); // This is the real work
      std::cout << ms << " ms\n";
      usleep(1000); // Lose control for 1 ms 
      if(ms < minms) minms = ms;
      D.reset();
   }
   std::cout << reps << " iterations of " << D.ndata << " items in " 
             << R.num() << " ranges with " << numt << " threads: Fastest took "
             << minms << " ms, Average was " << tms/reps << " ms\n";
}

Ranges readRanges(const char *filename)
{
    Ranges R;
    std::fstream fs(filename, std::fstream::in);

    int lo, hi;
    while (fs >> lo >> hi)
        R += Range(lo, hi);
    fs.close();

    return R;
}

Data readData(const char *filename, unsigned int nItems)
{
    assert(nItems > 0);
    Data D(nItems);
    std::fstream fs(filename, std::fstream::in);
    int d = 0;
    while (fs >> D.data[d++].key)
       if(d == nItems)
          break;
    if(d < nItems)
       D.ndata = d-1;
    fs.close();
    return D;
}

int main(int argc, char *argv[])
{
   assert(argc > 4); // <rangefile> <datafile> <max data to read> <number of threads to use>
   Ranges R = readRanges(argv[1]);
   Data D = readData(argv[2], atoi(argv[3]));

   int reps = 6; // 6 reps by default
   if(argc == 6) // If there is a reps arg, use it
      reps = atoi(argv[5]);

   repeatrun(reps, D, R, atoi(argv[4])); // repeat reps times
   return 0;
}
