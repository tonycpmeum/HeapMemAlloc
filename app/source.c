#include "headers/stdheaders.h"
#include "headers/memory.h"

// docker images - a | grep none | awk '{ print $3; }' | xargs docker rmi --force

int main() {
   malloc_gay(16);

   printf("HELLO WORLD abwad\n");
   
   return 0;
}
