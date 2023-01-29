package mj13;

import java.awt.*;
import java.applet.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
button_c
extends Applet
implements syncback_i
{
  private String debug_id = "";
  private Button button = null;
  private sync_c sync = null;
  private String target_name = null;
  private Applet target_applet = null;
                                        /* ................................ */
  public synchronized void 
  init()                                /* initialize the applet            */
  {
	String s;
	String label = "LABEL";

	s = getParameter("debug_id");		/* get our name for debug			*/
	if (s != null && s.length() > 0)
	  this.debug_id = s;

	s = getParameter("target");			/* target applet's name				*/
	if (s != null && s.length() > 0)
	  this.target_name = s;

	s = getParameter("label");			/* get initial label 				*/
	if (s != null && s.length() > 0)
	  label = s;

	this.setLayout(null);
	this.button = new Button(label);
	this.button.resize(this.size());
	this.add(this.button);

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
  public
  boolean
  action(
    Event event,
    Object object)
  {
	if (event.id == Event.ACTION_EVENT)	/* click? 							*/
	{
      debug(
        " click while label \"" + 
        object.toString() + "\"");
	  if (this.target_applet != null)	/* found target applet? 			*/
	  {
		Event e = event;
		e.id = Event.SAVE_FILE;
		this.target_applet.action(		/* send target the fake event 		*/
          event, object);
	  }
	  else								/* did not find target applet 		*/
        System.out.println(
          this.button.getLabel() +
          ": applet \"" +
          this.target_name + "\" not synced");
	}
	else
	if (event.id == Event.SAVE_FILE &&	/* message from other applet?		*/
		this.button != null)
	{
      debug(
        " new label \"" + 
        object.toString() + "\"");
	  this.button.setLabel(				/* change button label 				*/
        (String)object);
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
	if (this.target_name != null)
	  this.target_applet = 
        getAppletContext().
          getApplet(this. target_name);
  }
}
