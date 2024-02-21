#include "headers/stdheaders.h"
#include "headers/memory.h"

int main() {
   char* ptr = heap_alloc(10);
   // printf("ptr = %p\n", ptr);

   // uint32_t val = *(uint32_t*)(ptr - 4);
   // printf("Heap mem size = %d\n", val);
   

   return 0;
}
