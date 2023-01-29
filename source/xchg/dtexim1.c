/* Example Program dtexim1                                                  */
/* Illustrates high level image export and import.                          */
/* Program takes a spec as the one required command line argument, like:    */
/*   dtexim1 x.pgm                                                          */
/* Generate an test pattern image, export it to the name, then import it.   */
/* Prints out simple result like:                                           */
/*   dtexim1: exported 64x64, imported 64x64                                */
/* Does not delete the external image it created.                           */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtack/os.h>

int main(int argc, char *argv[])
{
  dtimage_t image1;
  dtimage_t image2;

  dtimage_create(NULL, &image1,       /* space for a small image            */
    64, 64, DT_NTYPE_B08);

  dtimage_gen_color256(NULL,          /* generate some image contents       */
    &image1);

  dtimage_export(NULL, &image1,       /* export the image                   */
    argv[1], 0, 0);

  dtimage_import(NULL, &image2,       /* import the image                   */
    argv[1]);

  printf("dtexim1: "                  /* print results                      */
    "exported %ux%u, "
    "imported %ux%u\n",
    image1.xe, image1.ye,
    image2.xe, image2.ye);

  return 0;
}
