#ifndef Meowolf_is_gay
#include "stdheaders.h"
#endif

void malloc_gay(int n) {
   void *start = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
   
   if (start == MAP_FAILED) {
      perror("mmap");
      exit(EXIT_FAILURE);
   }

   printf("%p\n", start);
}