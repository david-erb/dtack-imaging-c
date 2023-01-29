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
  private dispatch_c dispatch = null;

/*..........................................................................
 *..........................................................................*/
  public 
  static 
  void 
  main(
    String argv[])
  {
	app_c app = new app_c(argv);		// create a receiver object
  }

/*..........................................................................
 *..........................................................................*/
  public 
  app_c(
    String argv[])
  {
	this.dispatch = new dispatch_c();	// create a dispatch object
       
	this.dispatch.init(					// init dispatch
      this,								// container
      argv[0],							// url
      null,								// docbase
      this);							// receiver

	this.setResizable(false);			// don't allow frame resizing
	this.pack();						// resize the frame around applet
	this.show();						// show the frame

	this.dispatch.start();				// start video
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
	this.dispatch.receive(				// pass message to dispatch first
      sender_id, type, message);

	if (type.equals("mj-xe") ||			// width change?
		type.equals("mj-ye"))			// height change?
	  this.pack();						// resize the frame around components
  }

}
