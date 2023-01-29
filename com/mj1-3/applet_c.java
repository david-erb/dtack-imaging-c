package mj13;

import java.net.*;
import java.awt.*;
import java.io.*;
import java.applet.*;
import java.awt.Image;
import java.awt.Event;
import sun.awt.image.*;
import java.awt.image.*;

public static void main(String args)
{
  Frame frame = new Frame("title");
  applet_c applet = new applet_c();
  f.add("Center", applet);
  f.resize(300, 500);	
  f.show();
  applet.init();
  applet.start();
}

/*..........................................................................
 * sample applet "imagepush"
 *
 * Shows how to decode image "server push".
 * "Server push" is the common name for mime type multipart/x-mixed-replace.
 * This applet expects each part of the document to be either mime type
 * image/jpeg or image/gif.
 * 
 * The rules specifying the sequence of lines expected by this applet is:
 * A.	standard http header, Content-type multipart/x-mixed-replace;boundary
 * B.	boundary
 * C.	standard http header, must include Content-type and Content-length
 * D.	exactly Content-length bytes
 * E.	0 or more blank lines
 * F.	boundary (go to B) or boundary with "--" appended (done)
 *
 * You must either make sure your CGI is pushing according to the rules
 * above, or modify the applet to conform to what your CGI pushes.
 *
 * All use permitted for any legal purpose.  No warranty implied.
 * by David Erb erb@dtack.com
 * Independent Java Programmer: "no job too small" 
 * http://www.dtack.com/shingle.html
 * phone 1.800.221.7583 or US +608.767.4000
 *..........................................................................*/

public
class
applet_c
/* extends Applet */
implements callback_i, syncback_i
{
  private String debug_id = "";
  private String BUTTON_RESTART = "Restart";
  private String BUTTON_SUSPEND = "Suspend";
  private String BUTTON_RESUME = "Resume";
  private String FEEDBACK = "Feedback";
  private boolean idle = true;

  private sync_c sync = null;
  private boolean synced = false;

  private String button_name;
  private Applet button_applet = null;

  private String state_name;
  private Applet state_applet = null;

  private String quality_name;
  private Applet quality_applet = null;

  private String contrast_name;
  private Applet contrast_applet = null;

  private queue_c queue;
  private input_c input;
  
  private String urlstring;
  private Image image = null;
  private int count = 0;
  
  static final String INITIALIZING = "Initializing applet.";
  static final String STARTING =     "Loading first image.";
  static final String LOADING =      "Image started.";
  static final String LOADED =       "Loaded.";
  static final String DONE =         "End of image sequence.";
  static final String SUSPENDING =    "Loading suspending.";
  static final String RESUMING =     "Loading resuming.";
  static final String ABORTED =      "Sorry, image loading aborted.";
  static final String ERROR =        "Sorry, error loading image.";
  static final String EXCEPTION =    "Sorry, exception loading image.";
  static final String INTERRUPTION = "Sorry, interruption loading image.";
  static final String BADHEADER =    "Sorry, server sent something invalid.";
  static final String BADTRAILER =   "Sorry, data has become garbled.";
  static final String STRETCH      = "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

                                        /* ................................ */
  public void 
  init()                                /* initialize the applet            */
  {
	String s;

	s = getParameter("debug_id");		/* get our name for debug			*/
	if (s != null && s.length() > 0)
	  this.debug_id = s;

	this.set_state(INITIALIZING);

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

	this.queue = new queue_c(32);		// prepare a short command queue

	s = getParameter("sync");
	if (s != null)
	{
	  this.sync = new					// create sync object
	    sync_c(
          getParameter("name"),
          getParameter("sync"),
          getAppletContext(),
          this);
	  this.sync.start();				// start the sync thread
	}
  }
                                        /* ................................ */
  public boolean 
  is_error()							/* return true if error state		*/
  {
	return true;
  }
                                        /* ................................ */
  public void 
  start()                               /* start the loading                */
  {
	if (!this.synced)					/* not synced yet?					*/
	  return;

	this.set_button(BUTTON_SUSPEND);
	this.set_state(RESUMING);

	if (this.idle)
	{
	  this.input = new input_c(
        this.urlstring,
        this.getDocumentBase(),
        this,							/* component 						*/
        this,							/* observer							*/
        this,							/* callback 						*/
        this.queue);
	  if (!this.debug_id.equals(""))
		this.input.set_debug_id(
          debug_id + "(input)");
	  this.input.start();				/* start thread to do loading 		*/
	  this.idle = false;
	}
	else
	{
	  input.set_hold(input.HOLD_OFF);
	}
  }
                                        /* ................................ */
  public void 
  stop()                                /* stop the loading                 */
  {
	this.set_button(BUTTON_RESTART);
	input.set_hold(input.HOLD_QUITTING);
  }
                                        /* ................................ */
  public void 
  update(Graphics g)                    /* paint the image without clearing */
  {
    this.paint(g);                      /* don't blank screen               */
  }
                                        /* ................................ */
  public void
  paint(Graphics g)                     /* refresh display                  */
  {
	if (this.image != null)				/* an image is ready? 				*/
      g.drawImage(this.image,			/* draw it 							*/
        0, 0, this);
  }
                                        /* ................................ */
  public boolean
  imageUpdate(                          /* get notified when image is done  */
    Image image,
    int f,
    int x0,
    int y0,
    int xe,
    int ye)
  {
    if ((f & ImageObserver.ALLBITS) != 0)   
    {
      this.debug("got ALLBITS of frame " + 
        Integer.toString(this.count));
      this.count++;
	  this.image = image;
	  this.repaint();					/* force image to redraw 			*/
	  if (this.input != null)
	    this.input.set_hold(
          input.HOLD_DISPLAYED);
    }
    else
    if ((f & ImageObserver.ERROR) != 0)
    {
      this.set_state(ERROR);
    }
    else
    if ((f & ImageObserver.ABORT) != 0)
    {
      this.set_state(ABORTED);
    }
    return true;
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
  private
  void
  error(
    String s)
  {
	if (debug_id != null &&
		!debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
	else
	  System.out.println(s);
  }	

/*..........................................................................
 *..........................................................................*/
public
boolean
action(
  Event event,
  Object object)
{
  String s = object.toString();
  debug(
    "event id=" + Integer.toString(event.id) +
    " object=\"" + s + "\"");
  if (object == BUTTON_RESTART)
	this.start();
  else
  if (object == BUTTON_SUSPEND)
	this.stop();
  else
  if (object == BUTTON_RESUME)
	this.start();
  else
  if (s.equals(FEEDBACK))
  {
	s = event.arg.toString();
	this.debug("got feedback \"" + s + "\"");
	this.queue.add(s);
  }

  return true;
}

/*..........................................................................
 *..........................................................................*/
public
Applet
find_applet(
  String name)
{
  AppletContext context;
  context = getAppletContext();			/* get this applet's context 		*/
  Applet applet;
  int i;
  for (i=0; i<20; i++)
  {
	applet = context.getApplet(name);	/* point to target applet 			*/
	if (applet != null)					/* found target applet? 			*/
	  return applet;
	try {Thread.sleep(200);}
	catch(Exception e) {};
  }
  error(
    "can't find button applet \"" + 
    name + "\"");
  return null;
}

/*..........................................................................
 *..........................................................................*/
  public void
  set_button(
    String label)
  {
    if (this.button_applet != null)
	{
	  Event event = new Event(this,
        Event.SAVE_FILE, "");
	  this.button_applet.action(event, 	/* send target the fake event 		*/
        label);	
	}
  }

/*..........................................................................
 *..........................................................................*/
  public void 
  set_state(							/* set state						*/
    String state)
  {
    if (this.state_applet != null)		/* state applet exists? 			*/
	{
	  Event event = new Event(this,
        Event.SAVE_FILE, "");
	  String s = state;
	  if (this.count > 0)
        s = s + "  Frame=" + 
          Integer.toString(this.count);	
	  this.state_applet.action(event, 	/* send target the fake event 		*/
        s);
	}
  }

/*..........................................................................
 *..........................................................................*/
public
void
callback_state(
  String state)
{
  this.set_state(state);
}

/*..........................................................................
 *..........................................................................*/
public
void
callback_idle()
{
  this.set_button(BUTTON_RESTART);
  this.set_state("Idle");
  this.idle = true;
}

/*..........................................................................
 * called when an mj extension header is found in the http response header
 * for example "Mj-jpeg_quality: 60"
 *..........................................................................*/
public
void
callback_header(
  String keyword,
  String value)
{
  Event e = new Event(this, 0, null);
  if (keyword.equals("mj-jpeg_quality"))
  {
	if (this.quality_applet != null)
	{
	  e.x = Integer.parseInt(value);
	  this.quality_applet.action(e, "confirm");
	}
  }
  else
  if (keyword.equals("mj-fb_contrast"))
  {
	if (this.contrast_applet != null)
	{
	  e.x = Integer.parseInt(value);
	  this.contrast_applet.action(e, "confirm");
	}
  }
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









