#ifndef Meowolf_is_gay
#include "stdheaders.h"
#endif

#define HEAP_SIZE sysconf(_SC_PAGE_SIZE) * 10
#define HEAP_FAILURE (void*)-1
#define ALLOC_STAT(x) ((x & 1) == true)
#define DISPLACE_PTR(ptr_type, ptr, byte) (ptr = (ptr_type) ((char*)ptr + byte))

struct {
   void *ptr;
} _HEAP_DATA = { NULL };

void update_block_data(void *ptr, uint32_t block_size, bool allocated) {
   uint32_t *start = (uint32_t*)ptr;
   uint32_t *end = ptr;
   // header size = 4 bytes, at the start + end of block
   // block size = payload + padding + 2x 4 byte headers
   DISPLACE_PTR(uint32_t*, end, block_size - 4);

   *start = block_size | allocated;
   *end = block_size | allocated;
}

void _heap_init(void) {
   void *start = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
   
   if (start == MAP_FAILED) {
      perror("mmap");
      exit(1);
   }

   update_block_data(start, HEAP_SIZE, 0);
   _HEAP_DATA.ptr = start;
}

void *heap_alloc(uint8_t size) {
   if (_HEAP_DATA.ptr == NULL) { _heap_init(); }

   // block size = payload + padding + 2x 4 byte headers
   size_t block_size = 8 + ( size / 8 + (size % 8 != 0) ) * 8;
   printf("%ld\n", block_size);

   // find free block with enough space first;


   return _HEAP_DATA.ptr;
}

void heap_free(void *ptr);