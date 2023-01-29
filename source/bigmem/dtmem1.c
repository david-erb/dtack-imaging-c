/* Example Program dtmem1                                                   */
/* Illustrates basic big memory operations                                  */
/* Program takes no command line arguments.                                 */
/* Fill source memory with pattern, copy to second memory, sum result.      */
/* Prints out simple result like:                                           */
/*   dtmem1: sum 290000, correct                                            */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtack/mem.h>

int main(void)
{
  void *mem1, *mem2;                  /* big memory objects                 */
  unsigned int datasize;              /* size of direct-access buffer       */
  long offset;
  long sum = 0;

  dtmem_alloc(NULL, &mem1,            /* big allocate 10000 bytes           */
    0, 0, 100, 100, NULL, NULL);
  dtmem_clear(NULL, mem1);            /* clear big memory to 0              */

  for (offset = 0;                    /* loop over whole big memory         */
       offset < 100L*100L;
       offset += datasize)
  {
    char *ramptr;
    unsigned int i;

    dtmem_get(NULL, mem1, offset,     /* get next offset                    */
      (void **)&ramptr, &datasize);

    for (i=0; i<datasize; i+=2)       /* set every other byte to 14         */
      ramptr[i] = 14;

    dtmem_put(NULL, mem1,             /* unbind offset, mark changed        */
       offset);
  }

  dtmem_alloc(NULL, &mem2,            /* big allocate 20000 bytes           */
    0, 0, 100, 200, NULL, NULL);
  dtmem_set(NULL, mem2, 22);          /* set every byte to 22               */

  dtmem_copy(NULL, mem1, mem2);       /* copy smaller memory to large one   */

  for (offset = 0;                    /* loop over whole big memory         */
       offset < 100L*200L;
       offset += datasize)
  {
    char *ramptr;
    unsigned int i;

    dtmem_get(NULL, mem2, offset,     /* get next offset                    */
      (void **)&ramptr, &datasize);

    for (i=0; i<datasize; i++)        /* sum all data                       */
      sum += ramptr[i];

    dtmem_unmap(NULL, mem2,           /* unbind offset, mark unchanged      */
       offset);
  }

  printf("dtmem1: sum %ld, %s\n",
    sum,
    sum == 10000L*22 + 5000L*14?
      "correct": "incorrect");

  dtmem_free(NULL, mem2, NULL, NULL); /* free second big memory             */

  dtmem_free(NULL, mem1, NULL, NULL); /* free first big memory              */

  return 0;
}
