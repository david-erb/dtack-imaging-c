/* Example Program dtmem2                                                   */
/* Illustrates advanced big memory operations                               */
/* Program takes one command line argument: name of a temporary output file.*/
/* Fill source memory with pattern, write it, read it back, compare.        */
/* Prints out simple result like:                                           */
/*   dtmem2: patch at 1011, correct                                         */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtack/mem.h>
#include <dtack/os.h>

int main(int argc, char *argv[])
{
  dtfd_t fd;                          /* file structure                     */
  void *mem1, *mem2;                  /* big memory objects                 */
  long cmp;
  void *data;

  dtfd_open(NULL, &fd, argv[1],       /* open file for writing and reading  */
    "wr");

  dtos_malloc(NULL, &data, 20*20);    /* get some ram                       */
  memset(data, 11, 20*20);            /* set ram bytes to 11                */

  dtmem_alloc(NULL, &mem1,            /* big allocate 10000 bytes           */
    0, 0, 100, 100, NULL, NULL);
  dtmem_clear(NULL, mem1);            /* clear big memory to 0              */

  dtmem_putpatch(NULL, mem1,          /* put 20x20 patch into big memory    */
    data, 10, 10, 20, 20);

  dtmem_realloc(NULL, mem1,           /* increase big memory size           */
    0, 0, 100, 200, NULL, NULL);

  dtmem_set2(NULL, mem1, 100L*100L,   /* set bytes in new part to 0         */
    100L*100L, 0);

  memset(data, 22, 20*20);            /* set ram bytes to 22                */
  dtmem_putpatch(NULL, mem1,          /* put 20x20 patch into new part      */
    data, 10, 110, 20, 20);

  dtmem_write(NULL, mem1, 100L*100L,  /* write second half of big memory    */
    100L*100L, &fd);

  dtmem_alloc(NULL, &mem2,            /* big allocate another 10000 bytes   */
    0, 0, 100, 100, NULL, NULL);
  dtmem_set(NULL, mem2, 99);          /* set big memory bytes to 99         */

  dtfd_seek(NULL, &fd, 0,             /* go back to start of file           */
    DTFD_WHENCE_SET, NULL);

  dtmem_read(NULL, mem2, 0,           /* read new big memory                */
    100L*100L, &fd, NULL, NULL);

  dtmem_cmp(NULL, mem1, mem2, 0,      /* compare memories                   */
    &cmp);

  printf("dtmem2: patch at %ld, %s\n",
    cmp,
    cmp == 1011?
      "correct": "incorrect");

  dtmem_free(NULL, mem2, NULL, NULL); /* free second big memory             */

  dtmem_free(NULL, mem1, NULL, NULL); /* free first big memory              */

  dtos_free(NULL, data);              /* free ram                           */

  dtfd_delete(NULL, &fd);             /* close and delete file              */

  return 0;
}
