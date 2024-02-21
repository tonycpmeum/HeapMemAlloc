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

void _update_block_data(void *ptr, uint32_t block_size, bool allocated) {
   uint32_t *start = (uint32_t*)ptr;
   uint32_t *end = ptr;
   // header size = 4 bytes, at the start + end of block
   // block size = payload + padding + 2x 4 byte headers
   DISPLACE_PTR(uint32_t*, end, block_size - 4);

   uint32_t prev_bdata = *start;
   *start = block_size | allocated;
   *end = block_size | allocated;

   if (block_size == HEAP_SIZE) { return; }

   // update affected neighboring block datas
   if (allocated) {
      DISPLACE_PTR(uint32_t*, start, block_size);
      *start = prev_bdata - block_size;

      DISPLACE_PTR(uint32_t*, end, prev_bdata - block_size);
      *end = prev_bdata - block_size;
   } else {

   }
}

void _heap_init(void) {
   void *start = mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
   
   if (start == MAP_FAILED) {
      perror("mmap");
      exit(1);
   }

   _HEAP_DATA.ptr = start;
   _update_block_data(start, HEAP_SIZE, 0);
}

void *_find_free_block(uint32_t OGblock_size) {
   char *curr_block;
   uint32_t block_data, block_size = 0;
   do {
      curr_block = (char*)_HEAP_DATA.ptr + block_size;
      if (curr_block > (char*)_HEAP_DATA.ptr + HEAP_SIZE) {
         printf("uhm\n");
         exit(1);
      }
      block_data = *(uint32_t*)curr_block;
      // 3 LSB used as trigger bits
      block_size = block_data & 0xfffffff8;
   // LSB == 1: allocated; LSB == 0: free;
   } while (block_data & 1 || OGblock_size > block_size);

   return (void*)curr_block;
}

void *heap_alloc(uint32_t payload) {
   if (_HEAP_DATA.ptr == NULL) { _heap_init(); }

   // block size = payload + padding + 2x 4 byte headers
   size_t block_size = 8 + ( payload / 8 + (payload % 8 != 0) ) * 8;

   void *ptr = _find_free_block(block_size);
   _update_block_data(ptr, block_size, true);

   return ptr;
}

void heap_free(void *ptr);