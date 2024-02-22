#ifndef Meowolf_is_gay
#include "stdheaders.h"
#endif

#define HEAP_SIZE sysconf(_SC_PAGE_SIZE) * 10
#define HEAP_FAILURE (void*)-1
#define DISPLACE_PTR(ptr_type, ptr, byte) (ptr = (ptr_type) ((char*)ptr + (byte)))

struct {
   void *ptr;
} _HEAP_DATA = { NULL };

#define DISPLACE_STRT_PTR(byte) ((void*) (((char*)_HEAP_DATA.ptr + (byte))))

void _update_block_data(void *ptr, uint32_t block_size, bool allocated) {
   uint32_t *start = (uint32_t*)ptr;
   uint32_t *end = ptr;
   // header size = 4 bytes, at the start + end of block
   // block size = payload + padding + 2x 4 byte headers
   DISPLACE_PTR(uint32_t*, end, block_size - 4);

   uint32_t prev_bdata = *start;
   *start = block_size | allocated;
   *end = block_size | allocated;
   printf("Header -> %d\n", *start);

   if (block_size == HEAP_SIZE) { return; }

   if (allocated) {
      // update separated free block 
      DISPLACE_PTR(uint32_t*, start, block_size);
      *start = prev_bdata - block_size;

      DISPLACE_PTR(uint32_t*, end, prev_bdata - block_size);
      *end = prev_bdata - block_size;
   } else {
      if (start != _HEAP_DATA.ptr) {
         // coalescing free blocks, x = prev; y = next;
         uint32_t x_block_data = *(start - 1);
         if ((x_block_data & 1) == false) {
            DISPLACE_PTR(uint32_t*, start, -1 * x_block_data);
            *start += *end;
            *end += *start;
         }
      }

      if (end + 1 != DISPLACE_STRT_PTR(HEAP_SIZE)) {
         uint32_t y_block_data = *(end + 1);
         if ((y_block_data & 1) == false) {
            DISPLACE_PTR(uint32_t*, end, y_block_data);
            *start += *end;
            *end += *start;
         }
      }
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

   printf("--- Heap init ---\n\n");
}

void *_find_free_block(uint32_t OGblock_size) {
   char *curr_block;
   uint32_t block_data, block_size = 0;

   // stuck here
   do {
      curr_block = DISPLACE_STRT_PTR(block_size);
      printf("O BLOCKSIZE %d\n", block_size);
      printf("AT BLOCK %p\n", curr_block);
      if ((void*)curr_block > DISPLACE_STRT_PTR(HEAP_SIZE)) {
         printf("uhm\n");
         exit(1);
      }
      // 3 LSB used as trigger bits
      block_data = *(uint32_t*)curr_block;
      block_size = block_data & 0xFFFFFFF8;
   // LSB == 1: allocated; LSB == 0: free;
   } while (block_data & 1 || OGblock_size > block_size);

   printf("found %p\n", curr_block);

   return (void*)curr_block;
}

void *heap_alloc(uint32_t payload) {
   if (_HEAP_DATA.ptr == NULL) { _heap_init(); }

   // block size = payload + padding + 2x 4 byte headers
   size_t block_size = 8 + ( payload / 8 + (payload % 8 != 0) ) * 8;
   printf("block size: %ld\n", block_size);
   
   void *ptr = _find_free_block(block_size);
   _update_block_data(ptr, block_size, true);

   return ptr;
}

void heap_free(void *ptr) {
   if (ptr < _HEAP_DATA.ptr || ptr > DISPLACE_STRT_PTR(HEAP_SIZE)) {
      printf("bonk\n");
      exit(1);
   }

   uint32_t block_data = *(uint32_t*)ptr;
   
   _update_block_data(ptr, block_data & 0xFFFFFFF8, block_data & 1);
}