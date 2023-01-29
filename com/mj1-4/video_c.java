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
video_c
extends Panel
implements callback_i, ImageObserver
{
  private String sender_id;
  private String debug_id;
  private int xe;
  private int ye;
  private String url = null;
  private URL docbase = null;
  private receiver_i receiver = null;

  private queue_c queue;
  private input_c input;
  
  private Image image = null;
  private int count = 0;

  private boolean idle = true;

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

  static final String MODE = "mode";
  static final String MODE_STARTED = "mode_started";
  static final String MODE_STOPPED = "mode_stopped";
  
  static final String RECEIVE_COMMAND = "receive_command";
  static final String RECEIVE_SUSPEND = "receive_suspend";
  static final String RECEIVE_RESUME = "receive_resume";
  static final String RECEIVE_STATE = "receive_state";
  static final String RECEIVE_FEEDBACK = "receive_feedback";

                                        /* ................................ */
  public void
  setup(								// setup runtime constants
    String sender_id,
    String debug_id,
    int xe,
    int ye,
    String url,
    URL docbase,
    receiver_i receiver)
  {
	this.sender_id = sender_id;
	this.debug_id = debug_id;
	this.xe = xe;
	this.ye = ye;
	this.url = url;
	this.docbase = docbase;
	this.receiver = receiver;

	this.queue = new queue_c(32);		// prepare a short command queue
  }
                                        /* ................................ */
  public boolean 
  is_error()							/* return true if error state		*/
  {
	return true;
  }
                                        /* ................................ */
  public void 
  resume()								/* start the loading                */
  {
	if (this.idle)
	{
	  this.set_state(STARTING);
	  this.input = new input_c(
        this.url,
        this.docbase,
        this,							/* component 						*/
        this,							/* observer							*/
        this,							/* callback 						*/
        this.queue);
	  if (this.debug_id != null &&
		  !this.debug_id.equals(""))	// our debug is on?
		this.input.set_debug_id(		// set input's debug on
          "input");
	  this.input.start();				/* start thread to do loading 		*/
	  this.idle = false;
	  this.receiver.receive(			// tell receiver what's happening
        this.sender_id, 
        this.MODE, this.MODE_STARTED);
	}
	else
	{
	  this.set_state(RESUMING);
	  input.set_hold(input.HOLD_OFF);
	}
  }
                                        /* ................................ */
  public void 
  suspend()								/* stop the loading                 */
  {
	if (!this.idle)
	{
	  this.set_state(SUSPENDING);
	  input.set_hold(input.HOLD_QUITTING);
	}
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
  public void 
  set_state(							/* set state						*/
    String state)
  {
	String s = state;
	if (this.count > 0)
      s = s + " Frame=" + 
        Integer.toString(this.count);
	this.receiver.receive(
      this.sender_id, 
      this.RECEIVE_STATE, s);
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
    this.set_state("Idle");
	  this.receiver.receive(			// tell receiver what's happening
        this.sender_id, 
        this.MODE, this.MODE_STOPPED);
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
	this.receiver.receive(sender_id, 
      keyword, value);
  }

/*..........................................................................
 * implements receiver_i
 *..........................................................................*/
  public
  void
  receive(
    String sender_id,
    String type,
    String message)
  {
	debug("received from " + sender_id +
      " (" + type + ")" +
      " \"" + message + "\"");
	
	if (type.equals(this.RECEIVE_COMMAND) &&
		message.equals(this.RECEIVE_SUSPEND))
	{
	  this.suspend();
	}
	
	if (type.equals(this.RECEIVE_COMMAND) &&
		message.equals(this.RECEIVE_RESUME))
	{
	  this.resume();
	}
	
	if (type.equals(this.RECEIVE_FEEDBACK))
	{
	  this.queue.add(message);
	}

  }

/*..........................................................................
 *..........................................................................*/
  public
  Dimension
  minimumSize()
  {
	return this.preferredSize();
  }	

/*..........................................................................
 *..........................................................................*/
  public
  Dimension
  preferredSize()
  {
	return new Dimension(this.xe, this.ye);
  }	

/*..........................................................................
 *..........................................................................*/
  public
  void
  xsize(
    int xe)
  {
	this.xe = xe;
  }	

/*..........................................................................
 *..........................................................................*/
  public
  void
  ysize(
    int ye)
  {
	this.ye = ye;
  }	
}                                       /* end applet_c                  	*/









