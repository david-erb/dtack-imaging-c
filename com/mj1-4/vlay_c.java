package mj14;

import java.awt.*;

/*..........................................................................
 *..........................................................................*/

public
class
vlay_c
implements LayoutManager 
{
  int vgap;

/*..........................................................................
 *..........................................................................*/
  public 
  vlay_c(
	int vgap)
  {
	this.vgap = vgap;
  }

/*..........................................................................
 *..........................................................................*/
  public
  void
  addLayoutComponent(
    String name, 
    Component comp) 
  {
  }

/*..........................................................................
 *..........................................................................*/
  public 
  Dimension 
  preferredLayoutSize(
	Container parent) 
  {
	Insets insets = parent.insets();
	int	n = parent.countComponents();
	int	w = 0;
	int	h = 0;
	int i;

	for (i=0; i<n ; i++)
	{
	  Component c = parent.getComponent(i);
	  Dimension d = c.preferredSize();
	  if (w < d.width) 
		w = d.width;
	  h += d.height;
	  if (i != 0) 
		h += this.vgap;
	}

	return new Dimension(
      insets.left + insets.right + w,
	  insets.top + insets.bottom + h);
  }

/*..........................................................................
 *..........................................................................*/
  public 
  void 
  layoutContainer(
    Container parent) 
  {
	Insets insets = parent.insets();
	int x1 = insets.left;
	int x2 = insets.right;
	int y = insets.top;
	int w = this.preferredLayoutSize(parent).width;
	int n = parent.countComponents();
	int i;
	
	for (i=0; i<n; ++i) 
	{
	  Component c = parent.getComponent(i);
	  Dimension d = c.preferredSize();
	  c.reshape(x1+(w-x1-d.width-x2)/2, y, d.width, d.height);
	  y += (d.height + this.vgap);
	}
  }

/*..........................................................................
 *..........................................................................*/
  public 
  Dimension 
  minimumLayoutSize(
    Container parent) 
  {
	return preferredLayoutSize(parent);
  }

/*..........................................................................
 *..........................................................................*/
  public 
  void 
  removeLayoutComponent(
    Component comp) 
  {
  }
}
