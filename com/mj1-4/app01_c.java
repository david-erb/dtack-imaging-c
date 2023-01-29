package mj14;

import java.awt.*;
import java.applet.*;

/*..........................................................................
 *..........................................................................*/

public
class
app_c
extends Frame
{
                                        /* ................................ */
public static void main(String argv[])
{
  app_c app = new app_c();				// instantiate the application
  app.go();								// run the application
}
                                        /* ................................ */
private
void
go()
{
  FlowLayout left =						// layout for this application frame
    new FlowLayout(
     FlowLayout.LEFT);
  this.setLayout(left);

  applet_c applet = new applet_c();		// instantiate the applet
  this.add(applet);						// add applet to frame

  applet.init();						// let applet create its components

  this.pack();							// resize the frame around applet
  this.show();							// show the frame

  applet.start();						// let the applet run
}

}
