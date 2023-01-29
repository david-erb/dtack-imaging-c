/* Example Program dtkeybd1                                                 */
/* Illustrates basic keyboard operations                                    */
/* Program takes one optional command line argument: the keyboard spec.     */
/* First wait for a key, then poll every one second for a key.              */
/* Prints out simple result like:                                           */
/*   dtkeybd1: hit any key (waiting)                                        */
/*   dtkeybd1: you hit F1                                                   */
/*   dtkeybd1: hit any key (polling) .....                                  */
/*   dtkeybd1: you hit F2                                                   */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/os.h>

int main(int argc, char *argv[])
{
  dt_ctl_t _ctl, *ctl = &_ctl;
  dtkeybd_t keybd;                    /* the keyboard object descriptor     */
  dt_key_t key;
  char string[16];

  DT_MEMSET(ctl, 0, sizeof(*ctl));
/*ctl->dbg_mask = DT_DBG_MASK_ASYNC;                                        */
  dtkeybd_init(ctl, &keybd,           /* initialize keyboard                */
    argc>1? argv[1]: NULL, 0);        /* give argument if there is one      */

  printf("dtkeybd1:"
    " hit any key (waiting)\n");
  dtkeybd_read(ctl, &keybd, &key);    /* read key from keyboard             */
  dtkeymap_name_str(ctl,              /* make key value printable           */
    &keybd.keymap,
    key, string, sizeof(string));
  printf("dtkeybd1:"                  /* print what we got                  */
    " you hit %s\n",
    string);

  printf("dtkeybd1:"
    " hit any key (polling) ");
  while(1)                            /* polling loop                       */
  {
    int iskey;
    dtkeybd_scan(ctl, &keybd,         /* poll keyboard                      */
      &iskey, &key);
    if (iskey)                        /* a key was hit?                     */
      break;
    printf(".");                      /* no key was hit?                    */
    fflush(stdout);
    dtos_delay(ctl, 1000);
  }
  printf("\n");

  dtkeymap_name_str(ctl,              /* make key value printable           */
    &keybd.keymap,
    key, string, sizeof(string));
  printf("dtkeybd1:"                  /* print what we got                  */
    " you hit %s\n",
    string);

  dtkeybd_uninit(ctl, &keybd);        /* release keyboard resources         */
}
