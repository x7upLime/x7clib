#include "x7ht_structs.h"
#include "x7ht.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

ht*
ht_create(int initCap) {
  ht *table = malloc(sizeof(ht));
  if (table == NULL) return NULL;

  table->length = 0;
  table->capacity = initCap;

  table->entries = calloc(table->capacity, sizeof(ht_entry));
  if (table->entries == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

void
ht_destroy(ht *table) {
  // free all the allocated keys
  for (size_t i = 0; i < table->capacity; i++) {
    free((void*) table->entries[i].key);
  }

  free(table->entries); //frees all the memory allocated for the entries
  free(table); //frees all the memory allocated for the hash table itself
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
static uint64_t
hash_key(const char *key) {
  uint64_t hash = FNV_OFFSET;
  for (const char *p = key; *p != '\0'; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }

  return hash;
}

void*
ht_get(ht *table, const char *key) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

  // This first 4 lines mean the following:
  // we try to index the array with the index we got from our
  // hashing/modulo thingy.
  // we expect to find a non-empty value there,
  // i.e. to find the value we want to GET from the hashtable
  // this is a loop, because the hash/modulo may produce duplicates
  // and in such a case, we go linear probing
  // i.e. try the next one, until we find an empty spot.
  // The loop ends when we find the first empty spot,
  // which means either that our index produced an empty val,
  // or our indx is a duplicate, but none of the already
  // existing duplicates have the same key that we want.
  // in such a case we simply return NULL --> "nothing found, sorry"
  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      return table->entries[index].value;
    }

    // if we arrived here, it means that:
    // we have the same index, but the keys are not the same
    // i.e. our hashing/modulo thingy has produced the same index
    // for different keys.
    // At this point we go {linear probing}.
    index++;
    if (index >= table->capacity) {
      index = 0;
    }
    
    // we added 1 to the index, if its ooboundaries we start
    // from index 0, and we move back to the top of the while loop
  }
  return NULL;
}

/*
  This function is not testable from an external file since
  its storage class is defined as static (the symbol has scope only
  inside this file).
  
  If we want to test it, we either make it non static,
  or we copy/paste the declaration inside the .c test file,
  or we play some preprocessor magic.
  It's uglier, but it works like a charm.

  To make the test file work, we include this .c file.
  Again.. ugly, but it works.
 */
#ifdef TESTABLE
const char*
#else
static const char*
#endif // TESTABLE
ht_set_entry(ht_entry *entries, size_t capacity,
	     const char *key, void *value, size_t *plength) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(capacity-1));

  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      entries[index].value = value;
      return entries[index].key;
    }
    // the code above:
    // if value is already present,
    // we silently return the value of the key in the ht?

    index++;
    if (index >= capacity) {
      index = 0;
    }
  }

  if (plength != NULL) {
    key = strdup(key); // why are we copying this?
			 // why not using the original value instead?
    if (key == NULL) {
      return NULL;
    }

    (*plength)++;
  }
  /// what happens in plength is NULL ?
  // I want to test this..
  // TODO: get examples of how to test in C

  entries[index].key = (char*)key;
  entries[index].value = value;
  return key;
}

// it reallocates another ht with capacity = oldCapacity *2
// and frees the old hash table
static bool
ht_expand(ht *table) {
  size_t new_capacity = table->capacity *2;
  if (new_capacity < table->capacity) {
    return false;
  }
  ht_entry *new_entries = calloc(new_capacity, sizeof(ht_entry));
  if (new_entries == NULL) {
    return false;
  }

  for (size_t i = 0; i < table->capacity; i++) {
    ht_entry entry = table->entries[i];
    if (entry.key != NULL) {
      ht_set_entry(new_entries, new_capacity, entry.key,
		   entry.value, NULL);
    }
  }

  free(table->entries);
  table->entries = new_entries;
  table->capacity = new_capacity;
  return true;
}

const char*
ht_set(ht *table, const char *key, void *value) {
  // The function doc in header file states we don't want
  // a NULL value here.
  // Since this is just an experiment.. we just crash.
  assert(value != NULL);

  if (table->length >= table->capacity /2) {
    if (!ht_expand(table)) {
      return NULL;
    }
  }

  return ht_set_entry(table->entries, table->capacity, key, value,
		      &table->length);
}

size_t
ht_length(ht *table) {
  return table->length;
}

hti*
ht_iterator(ht *table) {
  hti *it;
  
  it = (hti*) malloc(sizeof(hti));
  if (it == NULL) {
    return NULL;
  }

  it->_table = table;
  it->_index = 0;
  return it;
}

bool
ht_next(hti *it) {
  ht *table = it->_table;
  
  while(it->_index < table->capacity) {
    size_t i = it->_index;
    it->_index++;
    
    if(table->entries[i].key != NULL) {
      // why are we making a copy of this?
      // Security purposes?
      // like we don't want the caller to have the
      // actual memory of the entry in the hash table?
      // But here the caller already has the table itself..
      //
      // ..also we're making the caller responsible of
      // freeing the memory. Bad/Good..? I have no idea
      ht_entry entry = table->entries[i];
      
      it->key = entry.key;
      it->value = entry.value;
      
      return true;
    }
  }

  return false;
}

/*
  You want the pointer passed as `int *val` to this function
  to be different for the actual pointer that points to the
  memory you used to ht_set() the hash table's entry.

  Otherwise strange things may happen.
 */
void
htiter_extract(hti *it, char **key, int *val) {
  *key = (char*)it->key;
  *val = *(int*)it->value;
  
  free(it->value);

  return;
}
