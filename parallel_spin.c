#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

#define NUM_BUCKETS 5     // Buckets in hash table
#define NUM_KEYS 100000   // Number of keys inserted per thread
int num_threads = 1;      // Number of threads (configurable)
int keys[NUM_KEYS];
pthread_spinlock_t spinLock[NUM_BUCKETS];


typedef struct _bucket_entry {
  int key;
  int val;
  struct _bucket_entry *next;
} bucket_entry;

bucket_entry *table[NUM_BUCKETS];

void panic(char *msg) {
  printf("%s\n", msg);
  exit(1);
}

double now() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Inserts a key-value pair into the table
void insert(int key, int val) {
  int i = key % NUM_BUCKETS;
  bucket_entry *e = (bucket_entry *) malloc(sizeof(bucket_entry));
  if (!e) panic("No memory to allocate bucket!");
  pthread_spin_lock(&spinLock[i]); 
  e->next = table[i];
  e->key = key;
  e->val = val;
  table[i] = e;
  pthread_spin_unlock(&spinLock[i]);
}

// Retrieves an entry from the hash table by key
// Returns NULL if the key isn't found in the table
bucket_entry * retrieve(int key) {
  bucket_entry *b;
  for (b = table[key % NUM_BUCKETS]; b != NULL; b = b->next) {
    if (b->key == key) return b;
  }
  return NULL;
}

void * put_phase(void *arg) {
  long tid = (long) arg;
  int key = 0;

  // If there are k threads, thread i inserts
  //      (i, i), (i+k, i), (i+k*2)
  for (key = tid ; key < NUM_KEYS; key += num_threads) {
    insert(keys[key], tid);
  }

  pthread_exit(NULL);
}

void * get_phase(void *arg) {
  long tid = (long) arg;
  int key = 0;
  long lost = 0;

  for (key = tid ; key < NUM_KEYS; key += num_threads) {
    if (retrieve(keys[key]) == NULL) lost++;
  }
  printf("[thread %ld] %ld keys lost!\n", tid, lost);

  pthread_exit((void *)lost);
}

int main(int argc, char **argv) {
  long i;
  pthread_t *threads;
  double istart, iend, rstart, rend;

  if (argc != 2) {
   panic("usage: ./parallel_hashtable <num_threads>");
  }
  if ((num_threads = atoi(argv[1])) <= 0) {
    panic("must enter a valid number of threads to run");
  }
	FILE *fpt;
	fpt = fopen("withSpin.csv", "w+");
	fprintf(fpt,"num_threads, insert, retrieve\n");
  for (int t = 1; t < num_threads; t=t+3){
  
	  srandom(time(NULL));
	  for (i = 0; i < NUM_KEYS; i++)
		keys[i] = random();
		
	  for(i=0; i < NUM_BUCKETS; i++)
		  pthread_spin_init(&spinLock[i], 0);

	  threads = (pthread_t *) malloc(sizeof(pthread_t)*t);
	  if (!threads) {
		panic("out of memory allocating thread handles");
	  }

	  // Insert keys in parallel
	  istart = now();
	  for (i = 0; i < t; i++) {
		pthread_create(&threads[i], NULL, put_phase, (void *)i);
	  }

	  // Barrier
	  for (i = 0; i < t; i++) {
		pthread_join(threads[i], NULL);
	  }
	  iend = now();
		
	  printf("[main] Inserted %d keys in %f seconds\n", NUM_KEYS, iend - istart);

	  // Reset the thread array
	  memset(threads, 0, sizeof(pthread_t)*t);

	  // Retrieve keys in parallel
	  rstart = now();
	  for (i = 0; i < t; i++) {
		pthread_create(&threads[i], NULL, get_phase, (void *)i);
	  }

	  // Collect count of lost keys
	  long total_lost = 0;
	  long *lost_keys = (long *) malloc(sizeof(long) * t);
	  for (i = 0; i < t; i++) {
		pthread_join(threads[i], (void **)&lost_keys[i]);
		total_lost += lost_keys[i];
	  }
	  rend = now();
	  
	  memset(threads, 0, sizeof(pthread_t)*t);

	  printf("[main] Retrieved %ld/%d keys in %f seconds\n", NUM_KEYS - total_lost, NUM_KEYS, rend - rstart);
	  
	  fprintf(fpt,"%d, %f, %f\n", t, (iend - istart), (rend - rstart));
  }
  return 0;
}