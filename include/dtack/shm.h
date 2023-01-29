typedef struct {
  void *address;
  int sid;
  int created;
  int attached;
} dtshm_t;

#include <dtack/shm.p>
