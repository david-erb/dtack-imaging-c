package mj14;

import java.net.*;
import java.awt.*;
import java.io.*;
import java.applet.*;
import java.awt.Image;
import java.awt.Event;
import sun.awt.image.*;
import java.awt.image.*;

/*..........................................................................
 *..........................................................................*/

public
class
video_a
extends Applet 
implements syncback_i, receiver_i
{
  private String debug_id = "";
  private String BUTTON_RESTART = "Restart";
  private String BUTTON_SUSPEND = "Suspend";
  private String BUTTON_RESUME = "Resume";
  private String FEEDBACK = "Feedback";

  private video_c video = null;
  
  private sync_c sync = null;
  private boolean synced = false;

										// ................................
  private String our_name;
  
  private String button_name;
  private Applet button_applet = null;

  private String state_name;
  private Applet state_applet = null;

  private String quality_name;
  private Applet quality_applet = null;

  private String contrast_name;
  private Applet contrast_applet = null;
  
  private String urlstring;

/*..........................................................................
 *..........................................................................*/
  public void 
  init()                                /* initialize the applet            */
  {
	String s;

	s = getParameter("debug_id");		/* get our name for debug			*/
	if (s != null && s.length() > 0)
	  this.debug_id = s;

	this.our_name = 
      getParameter("name");
	this.button_name = 
      getParameter("button");
	this.state_name = 
      getParameter("state");
	this.quality_name = 
      getParameter("quality");
	this.contrast_name = 
      getParameter("contrast");

    this.urlstring =					/* get <PARAM NAME=url>             */
      getParameter("url");

	this.video = new video_c();			// instantiate video object
    this.video.setup(					// set up video object
      this.our_name,
      this.debug_id,
      this.width,
      this.height,
      this.urlstring,					// url
      this.documentBase(),				// document base
	  this);							// receiver_i

	s = getParameter("sync");
	if (s != null)
	{
	  this.sync = new					// create sync object
	    sync_c(
          this.our_name,
          getParameter("sync"),
          getAppletContext(),
          this);
	  this.sync.start();				// start the sync thread
	}
  }
/*..........................................................................
 *..........................................................................*/
  public void 
  start()                               /* start the loading                */
  {
	if (!this.synced)					/* not synced yet?					*/
	  return;

	this.video.resume();
  }
/*..........................................................................
 *..........................................................................*/
  public void 
  stop()                                /* stop the loading                 */
  {
	this.video.suspend();
  }

/*..........................................................................
 *..........................................................................*/
  private
  void
  debug(
    String s)
  {
	if (debug_id != null &&
		!debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
  }	

/*..........................................................................
 *..........................................................................*/
  public
  void
  syncback_message(String message)
  {
	this.debug(message);
  }

/*..........................................................................
 *..........................................................................*/
  public 
  synchronized
  void
  syncback_synced()
  {
	if (this.button_name != null)
	  this.button_applet = 
        getAppletContext().
          getApplet(this.button_name);
	if (this.state_name != null)
	  this.state_applet = 
        getAppletContext().
          getApplet(this.state_name);
	if (this.quality_name != null)
	  this.quality_applet = 
        getAppletContext().
          getApplet(this.quality_name);
	if (this.contrast_name != null)
	  this.contrast_applet = 
        getAppletContext().
          getApplet(this.contrast_name);
	this.synced = true;
	this.start();						/* synced, so now start applet		*/
  }

}                                       /* end applet_c                  	*/









