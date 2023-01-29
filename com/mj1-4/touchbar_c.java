package mj14;

import java.awt.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
touchbar_c
extends Panel
implements receiver_i
{
  private String sender_id;
  private String debug_id;
  private int width;
  private int height;
  private String label;
  private receiver_i receiver = null;
  private String minscale = "min";
  private String midscale = "mid";
  private String maxscale = "max";
  private String prefix = "prefix,";

  private int confirmed = -1;
  private int touched = -1;
  private boolean inverted;

  static final String RECEIVE_CONFIRM = "receive_confirm";
  static final String RECEIVE_TOUCHED = "receive_touched";

                                        /* ................................ */
  public void
  setup(								// setup runtime constants
    String sender_id,
    String debug_id,
    int width,
    int height,
    String label,
    receiver_i receiver,
    String minscale,
    String midscale,
    String maxscale,
    String prefix)
  {
	this.sender_id = sender_id;
	this.debug_id = debug_id;
	this.width = width;
	this.height = height;
	this.label = label;
	this.receiver = receiver;
	this.minscale = minscale;
	this.midscale = midscale;
	this.maxscale = maxscale;
	this.prefix = prefix;
  }
	  
/*..........................................................................
 *..........................................................................*/
  public void
  outline(								// draw tab outline
    Graphics g,
    int x0, 
    int y0, 
    int xe, 
    int ye,
    Color fill_color,
    Color outline_color)
  {
    g.setColor(fill_color);
	g.fillRect(x0, y0, xe, ye);
    g.setColor(outline_color);
	g.drawRect(x0, y0, xe, ye);
  }
	  
/*..........................................................................
 *..........................................................................*/
  public void
  mark(									// draw mark position
    Graphics g,
    int x0, 
    int y0, 
    int xe, 
    int ye,
    int position, 
    Color color)
  {
	if (position < 0)
	  return;
	
    g.setColor(color);
	int i;
	int t;
	
	if (this.inverted)
	  t = 100 - position;
	else
	  t =  position;

	if (xe > ye)
	{
	  int x = (t*xe)/100;
	  for (i=-1; i<=1; i++)
	    g.drawLine(x+i, 1, x+i, ye-1);
	}
	else
	{
	  int y = (t*ye)/100;
	  for (i=-1; i<=1; i++)
		g.drawLine(1, y+i, xe-1, y+i);
	}
  }
	  
/*..........................................................................
 *..........................................................................*/
  public void
  paint(Graphics g)                     /* refresh display                  */
  {
	Color fill_color;
	Color outline_color;
	Color touched_color;
	Color confirmed_color;
	
	fill_color = Color.yellow;
	outline_color = Color.black;
	touched_color = Color.red;
	confirmed_color = Color.black;
	int tw, th;
	FontMetrics metrics;

	int w = this.size().width;
	int h = this.size().height;

	Font font = g.getFont();
	metrics = getFontMetrics(
      font);
    th = metrics.getHeight();

	int x0 = 0;
	int y0 = 0;
	int xe = w-1;
	int ye = h-1-th;

	this.outline(g, x0, y0, xe, ye,
      fill_color, outline_color);

	this.mark(g, x0, y0, xe, ye,
      this.touched, touched_color);
	this.mark(g, x0, y0, xe, ye, 
      this.confirmed, confirmed_color);

										// ................................
	g.setColor(Color.black);
	g.setFont(font);

	tw = metrics.stringWidth(
      this.minscale);
	g.drawString(this.minscale,
	  0,
	  (h - th) + 
      metrics.getAscent());

	tw = metrics.stringWidth(
      this.midscale);
	g.drawString(this.midscale,
	  (xe - tw) / 2,
	  (h - th) + 
      metrics.getAscent());

	tw = metrics.stringWidth(
      this.maxscale);
	g.drawString(this.maxscale,
	  (xe - tw),
	  (h - th) + 
      metrics.getAscent());

										// ................................
	Font bold = new Font(
      g.getFont().getName(), 
      Font.BOLD,
      g.getFont().getSize());
	metrics = getFontMetrics(
      bold);

	String s;
	if (this.touched != -1)
	  s = this.label + "=" + this.touched;
	else
	  s = this.label;
	tw = metrics.stringWidth(s);
    th = metrics.getHeight();

	this.outline(g, 
	  (xe - tw) / 2 - 2,
	  (ye - th) / 2 - 2,
      tw + 4, th + 4,
      Color.white, outline_color);

	g.setColor(Color.black);
	g.setFont(bold);
	g.drawString(s,
	  (xe - tw) / 2,
	  (ye - th) / 2 + 
        metrics.getAscent());
  }

/*..........................................................................
 *..........................................................................*/
  public
  boolean
  mouseDown(
    Event event,
    int x,
    int y)
  {
	int w = this.size().width;
	int h = this.size().height;
		
	if (w > h)
	  this.touched = (100*x) / w;
	else
	  this.touched = (100*y) / h;
	
	if (this.inverted)
	  this.touched = 100 - this.touched;

	this.repaint();						// redraw with touch mark
	
	this.debug(
      "touched at " + this.touched);

	if (this.receiver != null)			// receiver wants notification?
	  this.receiver.receive(			// notify receiver
		this.sender_id,
        this.RECEIVE_TOUCHED,
        this.prefix + this.touched);

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
	if (type.equals(this.RECEIVE_CONFIRM))
	{
	  this.confirmed = Integer.parseInt(message);
	  this.touched = this.confirmed;
	  debug(
        "confirmed " + this.confirmed);
	  this.repaint();
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
