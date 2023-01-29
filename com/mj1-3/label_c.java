package mj13;

import java.awt.*;
import java.applet.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
label_c
extends Applet
implements syncback_i
{
  private String debug_id = "";
  private String text = "";
  private int textheight;
  private sync_c sync = null;
                                        /* ................................ */
  public synchronized void
  init()                                /* initialize the applet            */
  {
	String s;
	s = getParameter("debug_id");		/* get our name for debug			*/
	if (s != null && s.length() > 0)
	  this.debug_id = s;
	s = getParameter("text");			/* get initial label 				*/
	if (s != null && s.length() > 0)
	  this.text = s;

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

/*..........................................................................
 *..........................................................................*/
  public void
  paint(Graphics g)                     /* refresh display                  */
  {
	if (this.textheight == 0)
	{
	  FontMetrics m = getFontMetrics(
        g.getFont());
	  this.textheight = m.getHeight();
	}

	g.drawString(this.text,				// paint the text
	  0, this.textheight);
  }

/*..........................................................................
 *..........................................................................*/
  public
  boolean
  action(
    Event event,
    Object object)
  {
	if (event.id == Event.SAVE_FILE &&	/* message from other applet?		*/
		this.text != null)
	{
	  debug(
        " new text \"" + 
        object.toString() + "\"");
	  this.text = (String)object;		/* change label label 				*/
	  this.repaint();
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
  }
}    
