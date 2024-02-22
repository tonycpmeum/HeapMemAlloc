#include "headers/stdheaders.h"
#include "headers/memory.h"

int main() {
   char *ptr = heap_alloc(16);
   printf("%p\n\n", ptr);
   int *inte = heap_alloc(20);
   printf("%p\n\n", inte);
   
   heap_free(ptr);
   
   uint64_t *bigmf = heap_alloc(64);
   printf("%p\n\n", bigmf);

   heap_free(inte);

   int *ganggang = heap_alloc(8);
   printf("%p\n\n", ganggang);

   heap_free(bigmf);
   heap_free(ganggang);

   munmap(_HEAP_DATA.ptr, HEAP_SIZE);

   return 0;
}
