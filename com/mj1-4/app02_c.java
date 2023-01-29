package mj14;

import java.awt.*;
import java.applet.*;

/*..........................................................................
 *..........................................................................*/

public
class
app_c
extends Frame
implements receiver_i
{
  private String sender_id = "app";
  private String debug_id = "app";
  
  private video_c video = null;
  private touchbar_c quality = null;
  private touchbar_c contrast = null;
  private button_c button = null;
  private label_c state = null;
  
  private String url = null;

  static final String BUTTON_ID = "button";
  static final String BUTTON_WAIT = "...";
  static final String BUTTON_SUSPEND = "Suspend";
  static final String BUTTON_RESUME = "Resume";

  static final int VGAP = 5;			// layout vertical gap

/*..........................................................................
 *..........................................................................*/
  public static void main(String argv[])
  {
	app_c app = new app_c();			// instantiate the application
	app.url = argv[0];					// extract program arguments
	app.init();							// init the application
	app.start();						// run the application
  }

/*..........................................................................
 *..........................................................................*/
  public
  void
  init()
  {
	this.video = new video_c();
	this.video.setup(
      "video",							// sender id
      "video",							// debug id
      340, 50,							// set up video window
      this.url,							// url
      null,								// docbase
      this);								// receiver_i
										// ................................
	this.quality = new touchbar_c();
	this.quality.setup(					// set up quality touchbar
      "quality",						// sender_id
      "quality",						// debug_id
      300, 50,							// size
      "Quality",						// label
      this,								// receiver_i
      "low", "medium", "lossless",
      "jquality,");
										// ................................
	this.contrast = new touchbar_c();
	this.contrast.setup(				// set up contrast touchbar
      "contrast",						// sender_id
      "contrast",						// debug_id
      300, 50,							// size
      "Contrast",						// label
	  this,								// receiver_i
	  "low", "normal", "high",
	  "contrast,");
										// ................................
	this.button = new button_c();
	this.button.setup(					// set up start/stop button
      BUTTON_ID,						// id
	  BUTTON_ID,						// debug_id
      60, 30,							// size of button
      BUTTON_WAIT,						// initial label
      this);							// receiver_i
										// ................................
	this.state = new label_c();
    this.state.setup(					// set up status field
      "status",							// id
      "status",							// debug_id
      240, 30,							// size of field
      "Starting");					   	// initial label
										// ................................

	vlay_c vlay =						// vertical-only layout
      new vlay_c(this.VGAP);
	this.setLayout(vlay);

	this.add(this.quality);				// add quality touchbar to frame
	this.add(this.contrast);			// add contrast touchbar to frame
	Panel p = new Panel();				// panel keeps state and button together
	p.add(this.button);					// add start/stop button to frame
	p.add(this.state);					// add state field to frame
	this.add(p);
	this.add(this.video);				// add video window to frame

	this.setResizable(false);	
	this.pack();						// resize the frame around applet
	this.show();						// show the frame
  }

/*..........................................................................
 *..........................................................................*/
  public
  void
  start()
  {
    video.receive(this.sender_id,		// tell video to start
      video_c.RECEIVE_COMMAND,
      video_c.RECEIVE_RESUME);
  }	

/*..........................................................................
 * implements receiver_i
 * this is the central message dispatch function for the application
 * it routes messages from the controls to the video
 * it also routes video information to the controls
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

	if (type.equals("mj-jpeg_quality"))	// quality confirm from video stream?
	  this.quality.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM, 
        message);

	if (type.equals("mj-fb_contrast"))	// contrast confirm from video stream?
	  this.contrast.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM,
        message);

	if (type.equals("mj-xe"))			// width change?
	{
	  int xe;
	  xe = Integer.parseInt(message);	// get new width from message
	  this.video.xsize(xe);				// change video display width
	  this.pack();						// resize the frame around components
	}

	if (type.equals("mj-ye"))			// height change?
	{
	  int ye;
	  ye = Integer.parseInt(message);	// get new height from message
	  this.video.ysize(ye);				// change video display height
	  this.pack();						// resize the frame around components
	}

	if (type.equals(					// state change from anybody?
          video_c.RECEIVE_STATE))
	  this.state.receive(sender_id,
        type, message);

	if (type.equals(					// any touchbar is touched?
		  touchbar_c.RECEIVE_TOUCHED))
	  this.video.receive(sender_id,		// send feedback to video source
        video_c.RECEIVE_FEEDBACK,
        message);
	  
	if (type.equals(video_c.MODE) &&	// video stream started?
		message.equals(video_c.
          MODE_STARTED))
	  this.button.receive(sender_id,
        button_c.RECEIVE_LABEL, 
        this.BUTTON_SUSPEND);

	if (type.equals(video_c.MODE) &&	// video stream stopped?
		message.equals(video_c.
          MODE_STOPPED))
	  this.button.receive(sender_id,
        button_c.RECEIVE_LABEL, 
        this.BUTTON_RESUME);

	if (sender_id.equals(BUTTON_ID) &&	// suspend clicked?
		message.equals(
          this.BUTTON_SUSPEND))
	  this.video.receive(sender_id,
        video_c.RECEIVE_COMMAND,
        video_c.RECEIVE_SUSPEND);

	if (sender_id.equals(BUTTON_ID) &&	// resume clicked?
		message.equals(
          this.BUTTON_RESUME))
	  this.video.receive(sender_id,
        video_c.RECEIVE_COMMAND,
        video_c.RECEIVE_RESUME);
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

}
