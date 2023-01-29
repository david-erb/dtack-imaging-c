/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: heapmon.c,v $ $Revision: 1.3 $");
#include <dtack/heap.h>
#include <dtack/os.h>
#include <dtack/prj.h>

/* ........................................................................ *
| NAME
| dt_heapmon_init() - commence heap monitoring
| 
| DESCRIPTION
|_index heap,, monitoring
| dt_heapmon_init() causes subsequent heap allocation
| operations like malloc() and free() to be tracked.
|
| Bookkeeping resources for subsequent heap monitoring are created
| and stored at ~ctl->heap~.
|
| After this call, all heap memory allocation and free operations
| are tracked.
| You may obtain a report of the outstanding allocations
| at any time using dt_heapmon_report().
| 
| You may discontinue tracking and release the bookkeeping resources
| by calling dt_heapmon_uninit().
| If you choose, you can start tracking again by another call to
| dt_heapmon_init(), but the tracking starts fresh assuming no allocations.
|
| If the environment variable :DT_MEMAVAIL: is defined
|_aside Hidden feature.
|_index environment variable,, :DT_MEMAVAIL:
|_index DT_MEMAVAIL, :DT_MEMAVAIL:, environment variable
| to any value, then the amount of free memory is printed
| on the console and the program delays briefly.
| The amount of free heap memory is estimated by actually trying
| to allocate it all.
| Allocation attempts cease when space is exhausted or one megabyte is
| successfully allocated.
| Two different buffer allocation sizes are used: 256 and 4000.
| This feature is mainly intended
| for memory-starved operating systems such as real-mode DOS.
| It doesn't make much sense to define the :DT_MEMAVAIL:
| environment variable on a virtual memory computer
| because one megabyte is almost assured to be available.
| 
| There are currently several compile-time choices
| for heap monitoring implementations.
|_aside Choose implementation at compile time.
| As of this writing, these are:
| 
| 1. 	no heap monitoring
| 2. 	Dtack-developed implementation
| 3.	MemCheck by StratosWare
|
| These mutually exclusive choices are selected
| at compile time in the following ways:
| 
| no heap monitoring				:-DDTPRJ_NOHEAPMON:
| Dtack-developed implementation	default if none of the other options is
| 									given
| MemCheck by StratosWare			:-DDTPRJ_STRATOS_MEMCHECK=n:
| 
|.No Heap Monitoring
| Choice 1, no heap monitoring,
| can be chosen at compile-time with the option :-DDTPRJ_NOHEAPMON:.
| If it has been deselected at compile time,
| then dt_heapmon_init() does nothing.
| Even if it is compiled into your program,
| you can avoid heap monitoring 
| simply by not calling dt_heapmon_init().
|
| Dtack does not recommend running without heap monitoring.
|_aside Not recommended.
| The cost in program size and execution time is negligible
| especially in large number-crunching applications.
| The programming time savings realized by the extra diagnostic information
| made available through heap checking is well worth the cost.
| 
|.Dtack-developed Implementation
| This is the most portable choice, and
| currently the only choice for any operating system besides real-mode DOS.
| It is also the least comprehensive choice 
| (short of excluding all heap monitoring).
| This is the default compile-time choice if you don't
| define one of the other choice's variables.
| 
| This implementation is only selected if you call dt_heapmon_init().
|_aside Your program must explicitly start it.
| Otherwise it is inactive.
| This implementation requires a table in which all memory allocation
| addresses and sizes are saved.
| This table is allocated by dt_heapmon_init()
| and its address placed in the housekeeping
| variable ~ctl->heap~.
| 
| The only checking performed by this implementation is that you
| don't try to free a memory block which is not allocated.
| You can also inquire the current number of allocated blocks and bytes.
| No overrun checking is done.
| Reports issued by this implementation contain individual allocations
| and their sizes.
| This implementation uses some heap memory itself.
|
|.MemCheck by StratosWare
|_index MemCheck,, heap checking
| This excellent commercial package may be used by Dtack libraries
| compiled for real-mode DOS.
| You enable calls to the MemCheck API through the compile-time
| variable :-DDTPRJ_STRATOS_MEMCHECK=n:.
| Before compiling with this option,
| you must purchase the MemCheck package with include file and libraries
| directly from StratosWare.
| 
| Dtack's use of MemCheck can be on one of four different levels.
| The level is defined at compile-time by the :n: in the
| variable :-DDTPRJ_STRATOS_MEMCHECK=n:.
| 
| Level 1.	:memcheck.h: included, but :NOMEMCHECK: defined.
|			This gets you source compatibility but no run-time operations.
| Level 2.	:memcheck.h: included, but no API calls.
|			This gets you source line information in run-time
| 			error and warning messages.
| 			You must use the standard MemCheck mechanisms,
| 			control panel :MC.EXE: and environment variable :MEMCHECK:,
| 			to activate and autoinit MemCheck.
| Level 3.	makes API calls to integrate MemCheck error reporting with
| 			the :err: Function Group.
| 			Like Level 2, you activate and start MemCheck in the standard ways.
| 			This is the best Level if your application already uses
| 			MemCheck.
| Level 4.	Like Level 3, but MemCheck started explicitly.
| 			This ignores the standard MemCheck startup mechanisms 
|			(environment variable and config file).
| 			A mc_get_settings() function is declared.
| 			This Level makes sure MemCheck is called in the
| 			most consistent and rigorous way.
| 			This level should the used for the Dtack test programs.
| 			Will not work correctly if your program already uses
| 			MemCheck.
|
| RETURN VALUES
| ~ctl->heap~ is set to point to bookkeeping resources allocated.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to malloc() memory for bookkeeping
| - heap monitoring already in effect
| - error starting MemCheck
| 
| CAVEATS
| Don't forget to discontinue heap checking before your program exits.
| Otherwise you may receive memory leak error messages.
|
| SEE ALSO
| Please refer to the
|_hyper section, The :heapmon: Function Group
| 
| The following functions are also related:
| dt_heapmon_uninit()		discontinue heap checking
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
|
|:  dt_ctl_t ctl = {0};
|:  dt_heapmon_init(&ctl);				// commence heap monitoring
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_heapmon_init(						/* begin heap monitoring		    */
  dt_ctl_t *ctl)						/* environment control 				*/
{
  DT_F("dt_heapmon_init");
  
  DTCSTACK_HIGHWATER;

  if (ctl != NULL)
  {
	void *heap;
	char env[32];
    int l;

	if (ctl->heap != NULL)
	  return dt_err(ctl, F,
        "heap monitoring already in effect");

    DT_Q(dtheap_init,(ctl,				/* init heap structure 				*/
      &heap, 0));		

	ctl->heap = heap;					/* begin automatic tracking 		*/
	
    DT_Q(dtos_getenv,(ctl, 
      "DT_MEMAVAIL", env, 
      sizeof(env), &l));
    if (l > 0)
    {
      dtheap_avail_say(ctl, ctl->heap, 4000, F);
      dtheap_avail_say(ctl, ctl->heap, 256, F);
      dtos_delay(ctl, 3000);
    }
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
 * dt_assert_memclean - complain if all memory not cleaned up
 *..........................................................................*/

static
dt_rc_e
assert_memclean(
  dt_ctl_t *ctl,
  char *F,
  char *desc)
{
	long blocks, bytes;
	DT_Q(dtheap_outstanding,(ctl,
      ctl->heap,
      &blocks, &bytes));
    if (blocks || bytes)
      return dt_assert(ctl, 0,
        NULL, "unfreed memory (%ld blocks, %ld bytes)", 
        blocks, bytes);
	else
      return dt_assert(ctl, 1,
        NULL, "all memory freed");
}


/* ........................................................................ *
| NAME
| dt_heapmon_uninit() - discontinue heap monitoring
| 
| DESCRIPTION
| dt_heapmon_uninit() discontinues checking of heap malloc() and free()
| operations.
|
| If there are outstanding memory allocations,
| ~*assert_fail~ is incremented if not NULL,
| and an assertion message issued through dt_assert().
| If memory statistics debug is enabled,
| a heap usage report is output through dt_heapmon_report().
| Bookkeeping resources for heap monitoring stored at ~ctl->heap~ are freed.
| ~ctl->heap~ is set to NULL.
|
| After this call, all heap memory allocation and free operations
| are no longer tracked.
| Tracking may be started afresh by a subsequent call to dt_heapmon_init().
| 
| If you call this function before dt_heapmon_init()
| then it simply does nothing.
| If heap monitoring has been disabled at compile-time
| then this function does nothing.
| 
| If ~ctl->dbg_mask~ has the :DT_DBG_MASK_MEMSTATS: debug class enabled,
|_index DT_DBG_MASK_MEMSTATS, :DT_DBG_MASK_MEMSTATS: debug class
| then a heap usage report is output through dt_dbg().
|
| A memory leak situation is indicated to the caller by sending
| incrementing ~*assert_fail~.
| A message is also issued through the assertion output mechanism 
| at dt_assert().
| This results in a debug message of class :DT_DBG_MASK_ALWAYS:.
|_index DT_DBG_MASK_ALWAYS, :DT_DBG_MASK_ALWAYS: debug class
| This situation will not result in an error returned by this function.
|
| If no memory leak is detected, then a message to this effect
| is output through dt_dbg() with class :DT_DBG_MASK_ASSERT:.
|_index DT_DBG_MASK_ASSERT, :DT_DBG_MASK_ASSERT: debug class
| 
| If MemCheck by StratosWare has been selected
| at compile-time, and the Dtack usage Level is less than 4,
| then heap checking is not actually disabled by a call to mc_endcheck().
| However, the current number of blocks
| is compared to the number of blocks at were allocated
| when dt_heapmon_init() was called.
| A memory leak is assumed if the difference is not 0.
|
| RETURN VALUES
| Bookkeeping resources stored at ~ctl->heap~
| are freed and ~ctl->heap~ set to NULL.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to free() memory for bookkeeping
| - error accessing MemCheck
| 
| CAVEATS
| Don't forget to discontinue heap checking 
| using this function before your program exits.
| Otherwise you may receive memory leak error messages.
|
| SEE ALSO
| Please refer to the
|_hyper section, The :heapmon: Function Group
| 
| The following functions are also related:
| dt_heapmon_init()		commence heap checking
| dt_heapmon_report()	produce heap usage report
| dt_dbg()				issue debug message
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
|
|:  dt_ctl_t ctl = {0};
|:  int assert_fail = 0;
|:  dt_heapmon_uninit(&ctl,				// quit heap monitoring
|:    &assert_fail);
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_heapmon_uninit(						/* terminate heap monitoring	    */
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dt_heapmon_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (ctl != NULL &&					/* heap monitoring not off? 		*/
	  ctl->heap != NULL)
  {
	void *heap = ctl->heap;

    if (DT_DBG(ctl,						/* debugging is on?				    */
        DT_DBG_MASK_MEMSTATS))
	{
      DT_G(dt_heapmon_report,(ctl,		/* spit out report				    */
        F));
	}

	if (assert_fail != NULL)
    *assert_fail += DT_RC_GOOD !=		/* check on memory cleanup		    */
      assert_memclean(ctl, F,			/* should be last thing program	    */
      "at end");						/* prints						    */
  
    ctl->heap = NULL;					/* quit automatic tracking 			*/

    DT_Q(dtheap_uninit,(ctl, heap));	/* clean up heap tracking			*/
  }

  return rc;
}


/* ........................................................................ *
| NAME
| dt_heapmon_report() - issue heap usage report
| 
| DESCRIPTION
| dt_heapmon_report() issues a heap usage report as debug output.
|
| A heap usage report using the heap bookkeeping kept at ~ctl->heap~
| is output through dt_dbg().
| ~*caller~ becomes part of the message if not NULL.
| 
| Each line of the report is given to a separate call to dt_dbg()
|_Aside Debug not filtered out.
| with the :DT_DBG_MASK_ALWAYS: debug class enabled,
| meaning the message is never filtered out.
| If you want to base your report conditionally on ~ctl->dbg_mask~,
| use the DT_DBG() macro before calling this function.
|_index DT_DBG_MASK_ALWAYS, :DT_DBG_MASK_ALWAYS: debug class
| 
| If you call this function before dt_heapmon_init()
|_aside No report if no heap monitoring.
| or after dt_heapmon_uninit(), then it simply does nothing.
| If heap monitoring has been disabled at compile-time
| then this function does nothing.
|
| The actual report format may differ between implementations.
|_aside Report format may differ.
| At least the number of blocks allocated and freed is generated.
| If there are outstanding memory allocations,
| these are listed individually with whatever information is known
| about the allocation.
|
| RETURN VALUES
| Calls to dt_dbg() are made.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error accessing MemCheck
| 
| CAVEATS
| If you don't want dt_dbg() output,
| then don't call this function since it uses :DT_DBG_MASK_ALWAYS:.
|
| SEE ALSO
| Please refer to the
|_hyper section, The :heapmon: Function Group
| 
| The following functions are also related:
| dt_heapmon_init()		commence heap checking
| dt_dbg()				issue debug message
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
|
|:  dt_ctl_t ctl = {0};
|:  dt_heapmon_init(&ctl);				// commence heap monitoring
|:  dt_heapmon_report(&ctl,				// print heap status
|:    "myprog");
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_heapmon_report(						/* report on heap usage			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller)					/* caller string for message 		*/
{
  DT_F("dt_heapmon_report");

  if (ctl != NULL &&					/* heap monitoring not off? 		*/
	  ctl->heap != NULL)
  {
    DT_Q(dtheap_report,(ctl,			/* spit out report					*/
      ctl->heap, caller));
  }

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dt_heapmon_check() - check heap integrity
| 
| DESCRIPTION
| dt_heapmon_check() checks heap integrity at the moment called.
|
| The heap integrity of the heap described by ~ctl->heap~ is checked.
| It is considered an error if the heap is found to be corrupted.
|
| Outstanding allocated blocks to not cause this function to generate
| at error message.
| Therefore, this function may be called at any time during
| the execution of your program.
| 
| If you call this function before dt_heapmon_init()
| or after dt_heapmon_uninit(), then it simply does nothing.
|
| If heap monitoring has been disabled at compile-time
| then this function does nothing.
|
| The completeness of the integrity check differs between
| heap monitoring implementations.
| For implementations such as MemCheck by StratosWare Corp.,
| guard bytes are placed around memory blocks so that overruns
| can be detected.
| Where this is available, all blocks are checked for overruns.
| 
| The completeness of the integrity check differs between compilers as well.
| Some compilers have a library call for heap checking.
| This is called by this function where available.
|
| RETURN VALUES
| Error messages are issued through dt_err() and :DT_RC_BAD:
| is returned if the heap is found to be corrupted.
| Otherwise the function returns :DT_RC_GOOD:.
| 
| ERRORS
| Unsuccessful completion may be due to:
| - error accessing MemCheck
| - heap is corrupted
| 
| CAVEATS
| dt_err() may be called multiple times.
| 
| The thoroughness of the integrity check differs between
| heap monitoring implementations and between compilers.
|
| SEE ALSO
| Please refer to the
|_hyper section, The :heapmon: Function Group
| 
| The following functions are also related:
| dt_heapmon_init()		commence heap checking
| dt_err()				issue error message
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
| 
|:  dt_ctl_t ctl = {0};
|:  dt_heapmon_init(&ctl);				// commence heap monitoring
|:  dt_heapmon_check(&ctl,				// check heap status
|:    "testpoint");
| 
| END
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dt_heapmon_check(						/* check heap integrity				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *caller)							/* caller string for messages 		*/
{
  DT_F("dt_heapmon_check");

  if (ctl != NULL &&
	  ctl->heap != NULL)
  {
    DT_Q(dtheap_check,(ctl,				/* check integrity					*/
      ctl->heap, caller));
  }

  return DT_RC_GOOD;
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
