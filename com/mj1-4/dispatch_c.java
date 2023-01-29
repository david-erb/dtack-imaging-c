package mj14;

import java.awt.*;
import java.net.*;

/*..........................................................................
 *..........................................................................*/

public
class
dispatch_c
implements receiver_i
{
  private Container container = null;
  private String url = null;

  private String sender_id = "app";
  private String debug_id = null;
  
  private video_c video = null;
  private touchbar_c quality = null;
  private touchbar_c brightness = null;
  private touchbar_c contrast = null;
  private touchbar_c hue = null;
  private touchbar_c saturation = null;
  private button_c button = null;
  private label_c state = null;
  

  static final String BUTTON_ID = "button";
  static final String BUTTON_WAIT = "...";
  static final String BUTTON_SUSPEND = "Suspend";
  static final String BUTTON_RESUME = "Resume";

  static final int VGAP = 5;			// layout vertical gap

/*..........................................................................
 *..........................................................................*/
  public
  void
  init(
    Container container,
    String url,
    URL docbase,
    receiver_i receiver)
  {
	this.container = container;
	this.url = url;						// the input parameter

	int w = 260;						// nominal control width
	int h = 42;							// nominal control height

	this.video = new video_c();
	this.video.setup(
      "video",							// sender id
      null,								// debug id
      340, 260,							// set up video window
      this.url,							// url
      docbase,							// docbase
      receiver);						// receiver_i
										// ................................
	this.quality = new touchbar_c();
	this.quality.setup(					// set up quality touchbar
      "quality",						// sender_id
      null,								// debug id
      w, h,								// size
      "Quality",						// label
      receiver,							// receiver_i
      "low", "medium", "lossless",
      "jquality,");
										// ................................
	this.brightness = new touchbar_c();
	this.brightness.setup(				// set up brightness touchbar
      "brightness",						// sender_id
      null,								// debug id
      w, h,								// size
      "Brightness",						// label
	  receiver,							// receiver_i
	  "dim", "normal", "bright",
	  "brightness,");
										// ................................
	this.contrast = new touchbar_c();
	this.contrast.setup(				// set up contrast touchbar
      "contrast",						// sender_id
      null,								// debug id
      w, h,								// size
      "Contrast",						// label
	  receiver,							// receiver_i
	  "low", "normal", "high",
	  "contrast,");
										// ................................
	this.hue = new touchbar_c();
	this.hue.setup(						// set up hue touchbar
      "hue",							// sender_id
      null,								// debug id
      w, h,								// size
      "Hue",							// label
	  receiver,							// receiver_i
	  "greenish", "balanced", "reddish",
	  "hue,");
										// ................................
	this.saturation = new touchbar_c();
	this.saturation.setup(				// set up saturation touchbar
      "saturation",						// sender_id
      null,								// debug id
      w, h,								// size
      "Saturation",						// label
	  receiver,							// receiver_i
	  "monochrome", "normal", "neon",
	  "saturation,");
										// ................................
	this.button = new button_c();
	this.button.setup(					// set up start/stop button
      BUTTON_ID,						// id
      null,								// debug id
      60, 30,							// size of button
      BUTTON_WAIT,						// initial label
      receiver);						// receiver_i
										// ................................
	this.state = new label_c();
    this.state.setup(					// set up status field
      "status",							// id
      null,								// debug id
      w-60, 30,							// size of field
      "Starting");					   	// initial label
										// ................................

	FlowLayout flow =
      new FlowLayout(FlowLayout.LEFT);
	container.setLayout(flow);

	vlay_c vlay =						// vertical-only layout
      new vlay_c(this.VGAP);

	Panel c_panel = new Panel();		// panel keeps all controls together
	c_panel.setLayout(vlay);

	c_panel.add(this.quality);			// add quality touchbar
	c_panel.add(this.brightness);		// add brightness touchbar
	c_panel.add(this.contrast);			// add contrast touchbar
	c_panel.add(this.hue);				// add hue touchbar
	c_panel.add(this.saturation);		// add saturation touchbar


	Panel v_panel = new Panel();		// panel keeps all video with button
	v_panel.setLayout(vlay);
	Panel b_panel = new Panel();		// panel keeps state and button together
	b_panel.add(this.button);			// add start/stop button
	b_panel.add(this.state);			// add state field
	v_panel.add(b_panel);				// add button/state
	v_panel.add(this.video);			// add video window

	container.add(c_panel);				// add controls to container
	container.add(v_panel);				// add video and start/stop to container
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

	if (type.equals("mj-fb_brightness"))// brightness confirm from video stream?
	  this.brightness.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM,
        message);

	if (type.equals("mj-fb_contrast"))	// contrast confirm from video stream?
	  this.contrast.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM,
        message);

	if (type.equals("mj-fb_hue"))		// hue confirm from video stream?
	  this.hue.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM,
        message);

	if (type.equals("mj-fb_saturation"))// saturation confirm from video stream?
	  this.saturation.receive(sender_id,
        touchbar_c.RECEIVE_CONFIRM,
        message);

	if (type.equals("mj-xe"))			// width change?
	{
	  int xe;
	  xe = Integer.parseInt(message);	// get new width from message
	  this.video.xsize(xe);				// change video display width
	  this.video.resize(
        this.video.preferredSize());
	}

	if (type.equals("mj-ye"))			// height change?
	{
	  int ye;
	  ye = Integer.parseInt(message);	// get new height from message
	  this.video.ysize(ye);				// change video display height
	  this.video.resize(
        this.video.preferredSize());
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
