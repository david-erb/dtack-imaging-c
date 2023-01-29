import java.applet.Applet;
import java.awt.Graphics;
import java.net.*;
import java.io.*;

public class umdemo extends Applet
{
  private URL url;
  private Image image;

/*..........................................................................
 *..........................................................................*/
public
void
init()
{
  System.out.println("umdemo: initializing x");
  try {
	Button button;
	button = new Button("Play");
	this.add(button);
	button = new Button("Step");
	this.add(button);
	button = new Button("Quit");
	this.add(button);
	button = new Button("Exit");
	this.add(button);
  }
  catch(Exception e)
  {
	System.out.println("umdemo.init: exception " + e);
  }
}

/*..........................................................................
 *..........................................................................*/
public
void
action(
  Event event,
  Object object)
{
  System.out.println("umdemo: action \"" + object + "\"");
  try {
  }
  catch(Exception e)
  {
	System.out.println("umdemo.action: exception " + e);
  }
}

/*..........................................................................
 *..........................................................................*/
public
void
start()
{
  System.out.println("umdemo: starting");
  try {
  }
  catch(Exception e)
  {
	System.out.println("umdemo.start: exception " + e);
  }
}

}										/* end class 						*/


