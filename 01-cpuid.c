#include <stdio.h>
#include <stdlib.h> 

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
{
        /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "2" (*ecx));
}

// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

// Assumes little endian
void printASCII(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    int i;
    
    for (i = 0; i < size; i++) {
        printf("%c ", b[i]);
    }
    puts("");
}

int main(int argc, char **argv)
{
  unsigned eax, ebx, ecx, edx;

  eax = atoi(argv[1]); /* processor info and feature bits */
  native_cpuid(&eax, &ebx, &ecx, &edx);

  printf("eax: %p\n", (void*)(unsigned long)eax);
  printf("ebx: %p\n", (void*)(unsigned long)ebx);
  printf("ecx: %p\n", (void*)(unsigned long)ecx);
  printf("edx: %p\n", (void*)(unsigned long)edx);

  printBits(sizeof(eax),&eax);
  printBits(sizeof(ebx),&ebx);
  printBits(sizeof(ecx),&ecx);
  printBits(sizeof(edx),&edx);

  printASCII(sizeof(eax),&eax);
  printASCII(sizeof(ebx),&ebx);
  printASCII(sizeof(ecx),&ecx);
  printASCII(sizeof(edx),&edx);

  return 0;
}
