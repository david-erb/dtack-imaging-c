import java.applet.Applet;
import java.awt.Graphics;
import java.awt.Image;
import java.net.*;
import java.io.*;

public class push extends Applet
{
  private URL url;
  private Image image;

/*..........................................................................
 *..........................................................................*/
public
void
init()
{
  System.out.println("push: initializing x");
  try {
    url = new URL("http://192.9.200.4/cgi-bin/nph-push.sh?n=1&camera=cam0&prefix=ftp.cam0.&suffix=.jpg");
    url = new URL("http://192.9.200.4/video/cam0/ftp.cam0.00078328.jpg");
    url = getCodeBase();
    System.out.println("push.init: url instantiated");
    System.out.println("push.init: url.getProtocol=" + url.getProtocol());
    System.out.println("push.init: url.getHost=" + url.getHost());
    System.out.println("push.init: url.getPort=" + url.getPort());
    System.out.println("push.init: url.getFile=" + url.getFile());
    System.out.println("push.init: url.getRef=" + url.getRef());
    image = getImage(url, "x");
    System.out.println("push.init: image instantiated");
  }
  catch(MalformedURLException e)
  {
	System.out.println("push.init: malformed URL: " + e);
	System.exit(0);
  }
//  catch(Exception e)
//  {
//    System.out.println("push.init exception: " + e);
//  }
}

/*..........................................................................
 *..........................................................................*/
public
void
start()
{
  System.out.println("push: starting");
  try {
  }
  catch(Exception e)
  {
	System.out.println("push.start: exception " + e);
  }
}

/*..........................................................................
 *..........................................................................*/
  
public 
void 
paint(
  Graphics g)
{
  g.drawImage(image, 0, 0, this);
  System.out.println("push: image drawn" +
    " w=" + image.getWidth(this) +
    " h=" + image.getHeight(this));
}

/*..........................................................................
 *..........................................................................*/
  
public 
boolean
imageUpdate(
  Image image,
  int flags,
  int x,
  int y,
  int width,
  int height)
{
  if ((flags & this.ALLBITS) != 0)
     System.out.println("push.imageUpdate: ALLBITS");
  if ((flags & this.ERROR) != 0)
     System.out.println("push.imageUpdate: ERROR");
  if ((flags & this.ABORT) != 0)
     System.out.println("push.imageUpdate: ABORT");
  return true;
}

}										/* end class 						*/


