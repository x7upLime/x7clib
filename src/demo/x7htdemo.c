#include <x7ht.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTR 10

ht *table;

void destroy_everyrhing(void) {
  ht_destroy(table);
}

int main(void)
{
  hti *it;
  const char *key;
  int *value;

  value = (int*)malloc(sizeof(int));
  if (value == NULL) {
    printf("unable to alloc memory for int value\n");
    return 1;
  }
  *value = 11;

  table = ht_create(ENTR);
  atexit(destroy_everyrhing);

  key = strdup("AKEY");
  if (key == NULL) {
    printf("something went sideways while copying key\n");
    return 1;
  }
  
  if (ht_set(table, key, value) == NULL) {
    printf("Seomthing went sideways while setting ht entry\n");
    return 1;
  }

  it = ht_iterator(table);
  printf("Hash Table entries:\n");
  while (ht_next(it)) {
    int value = 0;
    char *key = NULL;
    
    htiter_extract(it, (char **)&key, &value);
    printf("key: %s, value: %d\n", key, value);
  }
  printf("Length of ht: %zu\n", ht_length(table));

  printf("It works\n");
}
