package mj14;

import java.net.*;
import java.awt.*;
import java.io.*;
import java.applet.*;
import java.awt.Image;
import sun.awt.image.*;
import java.awt.image.*;

/*..........................................................................
 * sample class "imagepush_c"
 *
 * The constructor needs a valid URL.  The location referred to by the URL
 * will NOT be fetched.  It is a dummy only.  Applets may use getCodeBase().  
 * Applications may use URL("file").
 *
 * The constructor needs a content type, like "image/gif" or "image/jpeg".
 * It also needs the component on which the created Image will be
 * rendered.  An applet, frame, panel, or window works fine.
 *
 * The constructor just sets up the decoding apparatus.
 * The image() method returns the Image which will result from decoding.
 * Neither the constructor nor the image() method actually starts decoding.
 * You have to use the image (e.g. prepareImage() or drawImage()) to cause 
 * it to be decoded.  This behavior is similar to the familiar getImage().
 *
 * All use permitted for any legal purpose.  No warranty implied.
 * by David Erb erb@dtack.com
 * Independent Java Programmer: "no job too small" 
 * http://www.dtack.com/shingle.html
 * phone 1.800.221.7583 or US +608.767.4000
 *..........................................................................*/

class
decode_c
extends URLConnection
{
  private InputStream stream;           /* for passing to the callback      */
  private String content_type;
  private Image image;                  /* for returning to the caller      */
                                        /* ................................ */
  public
  decode_c(								/* create image from byte array     */
    URL url,
    byte[] bytes,
    String content_type,
    Component component)
  {
    super(url);							/* make us real                     */
    this.stream = 
      new ByteArrayInputStream(bytes);
    this.content_type = content_type;   /* save to return by callback       */

    URLImageSource source;              /* decoder/producer                 */
    source = 
      new URLImageSource(this);         /* instantiate the producer         */

    this.image =
      component.createImage(source);    /* run the decoder as a producer    */
  }
                                        /* ................................ */
  public Image
  image()
  {
    return this.image;                  /* just return image we created     */
  }
                                        /* ................................ */
                                        /* if you want to be considered a 
                                         * URLConnection, then you have to
                                         * have this method.                */
  public void
  connect()
  throws IOException
  {
  }
                                        /* ................................ */
                                        /* called by URLImageSource         */
  public String
  getContentType()
  {
    return this.content_type;           /* just return caller's type        */
  }
                                        /* ................................ */
                                        /* called by URLImageSource         */
  public synchronized InputStream
  getInputStream()
  throws IOException
  {
    return this.stream;                 /* just return caller's stream      */
  }

} 										/* end decode_c                		*/

