#include "stdheaders.h"
#include "memory.h"

// docker images - a | grep none | awk '{ print $3; }' | xargs docker rmi --force

int main() {
   malloc_gay(16);

   printf("HELLO WORLD BITCHES\n");
   
   return 0;
}
