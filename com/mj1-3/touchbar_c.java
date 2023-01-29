package mj13;

import java.awt.*;
import java.applet.*;
import java.awt.Event;

/*..........................................................................
 *..........................................................................*/

public
class
touchbar_c
extends Applet
implements syncback_i
{
  private String debug_id = "";
  private String label = "LABEL";
  private sync_c sync = null;
  private String target_name = null;
  private Applet target_applet = null;
  private String minscale = "min";
  private String midscale = "mid";
  private String maxscale = "max";
  private String prefix = "prefix,";
  private int confirmed = -1;
  private int touched = -1;
  private boolean inverted;
                                        /* ................................ */
  public synchronized void 
  init()                                /* initialize the applet            */
  {
	String s;

	s = getParameter("debug_id");		/* get our name for debug			*/
	if (s != null && s.length() > 0)
	  this.debug_id = s;

	s = getParameter("target");			/* target applet's name				*/
	if (s != null && s.length() > 0)
	  this.target_name = s;

	s = getParameter("label");			/* get string label					*/
	if (s != null && s.length() > 0)
	  this.label = s;
	s = getParameter("minscale");		/* get string min					*/
	if (s != null && s.length() > 0)
	  this.minscale = s;
	s = getParameter("midscale");		/* get string mid					*/
	if (s != null && s.length() > 0)
	  this.midscale = s;
	s = getParameter("maxscale");		/* get string max					*/
	if (s != null && s.length() > 0)
	  this.maxscale = s;
	s = getParameter("prefix");			/* command prefix					*/
	if (s != null && s.length() > 0)
	  this.prefix = s;

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

	String s = this.label + "=" + this.touched;
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

	Event e = event;
	e.arg = this.prefix + this.touched;
	
	this.debug(
      "touched at " + this.touched);

	if (this.target_applet != null)		/* found target applet? 			*/
	{
	  e.id = Event.SAVE_FILE;
	  this.target_applet.action(event, 	/* send target the fake event 		*/
		"Feedback");
	}
	else								/* did not find target applet 		*/
	  this.debug(
        "applet \"" +
        this.target_name +
        "\" not synced");

	return true;
  }

/*..........................................................................
 *..........................................................................*/
  public
  boolean
  action(
    Event event,
    Object object)
  {
	String s = (String)object;
	if (s.equals("confirm"))
	{
	  debug(
        "confirmed " + event.x);
	  this.confirmed = event.x;
	  this.touched = this.confirmed;
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
	if (this.target_name != null)
	  this.target_applet = 
        getAppletContext().
          getApplet(this. target_name);
  }
}  
