package mj14;

import java.awt.*;
import java.applet.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
label_c
extends Panel
implements receiver_i
{
  private String id = "";
  private String debug_id = "";
  private int width;
  private int height;
  private String text = "";
  
  private int textheight = 0;
  private int ascent = 0;

/*..........................................................................
 *..........................................................................*/
  public void
  setup(								// setup runtime constants
    String id,
    String debug_id,
    int width,
    int height,
    String text)
  {
	this.id = id;
	this.debug_id = debug_id;
	this.width = width;
	this.height = height;
	this.text = text;
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
	  this.ascent = m.getAscent();
	}

	g.drawString(this.text,				// paint the text
	  0, 
      (this.size().height - this.textheight) / 2 + this.ascent);
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
 * implements receiver_i
 *..........................................................................*/
  public
  void
  receive(
    String sender_id,
    String type,
    String message)
  {
	debug(
      "new label \"" + 
      message + "\"");
	this.text = message;
	this.repaint();
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
