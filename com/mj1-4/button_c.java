package mj14;

import java.awt.*;
import java.applet.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
button_c
extends Panel
implements receiver_i
{
  private String sender_id;
  private String debug_id;
  private int width;
  private int height;
  private String label = null;
  private receiver_i receiver = null;

  private Button button = null;

  static final String RECEIVE_LABEL = "receive_label";
	
/*..........................................................................
 *..........................................................................*/
  public void
  setup(								// setup runtime constants
    String sender_id,
    String debug_id,
    int width,
    int height,
    String label,
    receiver_i receiver)
  {
	this.sender_id = sender_id;
	this.debug_id = debug_id;
	this.width = width;
	this.height = height;
	this.label = label;
	this.receiver = receiver;

	this.setLayout(null);
	this.button = new Button(this.label);
	this.button.resize(new Dimension(this.width, this.height));
	this.add(this.button);
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
        "click while label \"" + 
        this.button.getLabel() + "\"");
	  if (this.receiver != null)		/* we have a receiver?				*/
		this.receiver.receive(
          this.sender_id, 
          "click", 
          this.button.getLabel());
	}
	return true;
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
	if (type.equals(this.RECEIVE_LABEL))
	{
	  debug(
        "new label \"" + 
        message + "\"");
	  this.button.setLabel(message);	/* change button label 				*/
	}
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
	return new Dimension(this.width, this.height);
  }	
}
