// sync package
// by David Erb
// allows multiple applets to sync on each other before starting
// depends on momma sync applet to wait until all required names
//   are registered before letting them all wake up
// the momma applet is small, maximizing the changes that it will
//   be completely loaded before the other applets need to talk to it

package mj13;

import java.applet.*;
import java.io.*;

/*..........................................................................
 *..........................................................................*/
public
class
sync_c
  extends Thread
{
  private String our_name;
  private String sync_name;
  private AppletContext context;
  private syncback_i syncback;
  
/*..........................................................................
 * constructor just obtains and saves descriptive information
 *..........................................................................*/

  public
  sync_c(
    String our_name,
    String sync_name,
    AppletContext context,
    syncback_i syncback)
  {
	this.sync_name = sync_name;			// save information
	this.our_name = our_name;
	this.context = context;
	this.syncback = syncback;

	if (this.our_name == null)			// must be our name
	{
      this.syncback.syncback_message(
        "no sync applet name");
	  return;
	}


	if (this.sync_name == null)			// must be a sync applet name
	{
      this.syncback.syncback_message(
        "no sync applet name");
	  return;
	}
  }
  
/*..........................................................................
 * run waits for sync applet to be available, then uses it to wait
 *..........................................................................*/
  public
  void
  run()
  {
    this.syncback.syncback_message(
      "(sync_c::run) " +
      "finding " + this.sync_name);

	Applet sync = this.find(			// find the sync applet
      this.context,
      sync_name);
	if (sync == null)
	{
	  this.syncback.syncback_message(
        "(sync_c::run) " +
        "couldn't find synclet \"" +
        sync_name + "\"");
	  return;
	}

    this.syncback.syncback_message(
      "(sync_c::run) " +
      "validating " + sync_name);

	try {Thread.sleep(4000);}			// wait for sync applet to warm up
	  catch(Exception e) {}

    this.syncback.syncback_message(
      "(sync_c::run) " +
      "syncing with other applets");

	sync.getParameter(					// sync with other applets
      "+" + this.our_name);				// prefix name with special character

    this.syncback.syncback_synced();
  }

/*..........................................................................
 * finds an applet, retrying as necessary
 *..........................................................................*/
  private
  Applet
  find(
    AppletContext context,
    String name)
  {
	int wait = 500;
	int waited = 0;
    while (true)
    {
	  try 
	  {
		Thread.sleep(wait);				// wait a little while
		waited += wait;
		Applet applet =					// point to target applet
          context.getApplet(name);
        if (applet != null)				// found applet?
	      return applet;
		if (waited > 2000)
		  return null;
	  }
	  catch(Exception e) 
	  {
		this.syncback.syncback_message(
		  "re-finding after " + e.toString());
	  }
	}
  }
}




