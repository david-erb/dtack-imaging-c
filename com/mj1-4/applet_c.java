package mj14;

import java.awt.*;
import java.applet.*;

/*..........................................................................
 *..........................................................................*/

public
class
applet_c
extends Applet
{
  private dispatch_c dispatch = null;

/*..........................................................................
 *..........................................................................*/
  public
  void
  init()
  {
	this.dispatch = new dispatch_c();	// create a dispatch object
       
	this.dispatch.init(					// init dispatch
      this,								// container
      getParameter("url"),				// url
	  this.getDocumentBase(),			// docbase
      this.dispatch);					// receiver
  }

/*..........................................................................
 *..........................................................................*/
  public
  void
  start()
  {
	this.dispatch.start();				// start video
  }
}
