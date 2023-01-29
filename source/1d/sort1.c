/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




http://133.14.50.41/power/Shigeru/maked/sortd/heap.html

/* heap sort */

void shift (int *rox, int parent, int num)
{
        int son;
        extern comparecount;
        son = 2 * parent;
        while (son <= num) {
                comparecount++;
                if (son < num && rox[son-1] < rox[son])
                        son++;
                comparecount++;
                if (rox[parent-1] >= rox[son-1])
                        break;
                swap (&rox[parent-1], &rox[son-1]);
                parent = son;
                son = 2 * parent;
        }
}

void sort (int *box, int length)
{
        int l, j;
        l = length;
        for (j = (l/2); j >= 1 ; j--)
                shift (box, j, l);
        while (l > 1) {
                swap (&box[0], &box[l-1]);
                shift (box, 1, --l);
        }
}





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
