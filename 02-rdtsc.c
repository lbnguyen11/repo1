#include <stdio.h>
// valid C99 and C++

#include <stdint.h>  // <cstdint> is preferred in C++, but stdint.h works.

#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

// optional wrapper if you don't want to just use __rdtsc() everywhere
inline
uint64_t readTSC() {
    // _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
    uint64_t tsc = __rdtsc();
    // _mm_lfence();  // optionally block later instructions until rdtsc retires
    return tsc;
}

// requires a Nehalem or newer CPU.  Not Core2 or earlier.  IDK when AMD added it.
inline
uint64_t readTSCp() {
    unsigned dummy;
    uint64_t tsc = __rdtscp(&dummy); // waits for earlier insns to retire
    // _mm_lfence();  // optionally block later instructions until rdtscp retires
    // in practice most CPUs seem to implement rdtscp similar to lfence; rdtsc, so later instructions won't start while waiting for earlier to finish.  But not guaranteed on paper.
    // see also the section below about out of order exec.
    return tsc;  
}

int main()
{
    // this can determine base frequency of cpu???
    uint64_t a = __rdtsc();
    sleep(1);
    uint64_t b = __rdtsc();
    printf("%ld\n", b-a);
}
