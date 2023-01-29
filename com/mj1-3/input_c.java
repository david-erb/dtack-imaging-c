package mj13;

import java.net.*;
import java.awt.*;
import java.io.*;
import java.applet.*;
import java.awt.Image;
import sun.awt.image.*;
import java.awt.image.*;

/*..........................................................................
 *..........................................................................*/

public
class
input_c				
extends Thread
{
  private String urlstring;
  private URL docbase;
  Component component;
  ImageObserver observer;
  callback_i callback;
  queue_c queue;
  private String boundary = "";
  private String content_type = "";
  private decode_c decode = null;
  public static final String HOLD_ON = "on";
  public static final String HOLD_OFF = "off";
  public static final String HOLD_PENDING = "pending";
  public static final String HOLD_QUITTING = "quitting";
  public static final String HOLD_CHECK = "check";
  public static final String HOLD_DISPLAYED = "displayed";
  private String hold = HOLD_OFF;	
  private String debug_id = "";
  private int loaded = 0;
  private int displayed = 0;
  
										/* ................................ */
  input_c(
    String urlstring,
    URL docbase,
    Component component,
    ImageObserver observer,
    callback_i callback,
    queue_c queue)
  {
	this.urlstring = urlstring;
	this.docbase = docbase;
	this.component = component;
	this.observer = observer;
	this.callback = callback;
	this.queue = queue;
	this.set_state("constructing");
  }
                                        /* ................................ */
  public void 
  run()                                 /* run the loading loop             */
  {
	debug("running");
	int more = -1;
    try 
    {
	  URL url;
	  if (!this.urlstring.startsWith(
            "http"))
		url = new URL(
          docbase.getProtocol() + "://" +
          docbase.getHost() + ":" +
          docbase.getPort() +"/" + 
          urlstring);
	  else
        url = new URL(this.urlstring);

	  this.set_state("resolving \"" + 
        url.getHost() + "\"");

	  InetAddress address = 
        InetAddress.getByName(
          url.getHost());

	  int port = url.getPort();
	  if (port == -1)
		port = 80;
	  this.set_state("connecting" +
        " to port " + 
        Integer.toString(port));
	  Socket socket = new Socket(		// create the new socket
        address, port, true);

	  this.set_state("requesting");
	  PrintStream request = new	
        PrintStream(socket.
          getOutputStream());

	  String s = "GET " +				// form the url request
        url.getFile() + " HTTP/1.0";
	  this.debug("http request is \"" + 
        s + "\"");
	  request.println(s);				// make the http request
	  request.println();
	  request.flush();

	  DataInputStream stream = new		/* access stream as data 			*/
        DataInputStream(socket.
          getInputStream());

	  if (this.master(stream))
      do {

		this.set_state("loading");
		int l = this.header(stream);	/* read the multipart header 		*/
		if (l == 0)						/* bad header? 						*/
		  break;
		byte bytes[] = new byte[l];		/* space for the image bytes 		*/
		stream.readFully(bytes);		/* read the image bytes 			*/
		more = this.trailer(stream);	/* read multipart trailer 			*/
		this.decode =					/* prepare decoding mechanism 		*/
		  new decode_c(url, bytes,
            this.content_type, 
              this.component);
		if (!this.set_hold(HOLD_CHECK))
		  break;
		this.loaded++;
		component.prepareImage(			/* force decoding to start          */
		  this.decode.image(), 
          observer);

		while(true)						// empty the command queue
		{
		  String cmd;
		  cmd = this.queue.remove();	// get next command queue entry
		  if (cmd == null)				// command queue empty
			break;
		  this.debug("sending " + cmd);
		  request.println(cmd);			// send command on socket
		  request.flush();
		}
      } while(more == 1);				/* loop until last multipart 		*/

	  socket.close();
    }
    catch(Exception e)
    {
      this.set_state("exception");
      error(e.toString());
    }
	if (more == 0)						/* end of slide show? 				*/
	  this.set_state("done");
	
	this.callback.callback_idle();		/* tell caller we are idle 			*/
	debug("ran");
  }
                                        /* ................................ */
  private void 
  set_state(							/* set current state				*/
	String state)
  {
	this.callback.callback_state(state);
  }
                                        /* ................................ */
  public synchronized boolean
  set_hold(								/* set hold flag					*/
    String hold)
  {
	this.debug("was " + this.hold + 
      " now " + hold);

    if (hold.equals(HOLD_OFF))	
	{
	  if (this.hold.equals(HOLD_ON))
	    this.notify();
	  this.hold = hold;
	}
	else
    if (hold.equals(HOLD_DISPLAYED))	
	{
	  this.displayed++;
	  if (this.loaded == this.displayed)
	    this.notify();
	}
	else
	if (hold.equals(HOLD_CHECK))
	{
	  while (this.loaded > this.displayed)
	  {
		try {this.wait();}
		catch(Exception e) {};
	  }
	  if (this.hold.equals(HOLD_PENDING))
	  {
		this.hold = HOLD_ON;
		this.set_state("paused");
		try {this.wait();}
		catch(Exception e) {};
		this.hold = HOLD_OFF;
	  }
	  if (this.hold.equals(HOLD_QUITTING))
	  {
		return false;
	  }
	}
	else
	if (hold.equals(HOLD_PENDING))
	{
	  this.hold = hold;
	  this.set_state("pausing");
	}
	else
	if (hold.equals(HOLD_QUITTING))
	{
	  this.hold = hold;
	  this.set_state("quitting");
	}
	return true;
  }
                                        /* ................................ */
  private String						/* get value before colon in string 	*/
  keyword(
    String s)
  {
	return s.substring(0, 
      s.indexOf(':'));
  }	
                                        /* ................................ */
  private String						/* get value after colon in string 	*/
  value(
    String s)
  {
	int i;
	for (i=s.indexOf(':')+1;			/* start looking after colon		*/
		 s.charAt(i) == ' ';			/* skip spaces after colon 			*/
		 i++);
	return s.substring(i);
  }	
                                        /* ................................ */
  private boolean						
  master(                               /* read master document header		*/
    DataInputStream stream)
  throws IOException
  {
	String s;
	this.content_type = "unknown";
    do {
      s = stream.readLine().			/* get next line from input stream 	*/
        toLowerCase();
	  this.debug("master line is \"" +
         s + "\"");
      if (s.startsWith(					/* this is Content-type? 			*/
            "content-type: "))
        this.content_type =
          this.value(s);
    } while (s.length() > 0);			/* go until blank line is found 	*/
	
	if (!this.content_type.startsWith(	/* content type invalid?			*/
         "multipart/x-mixed-replace"))
	{
	  this.set_state("invalid mime type");
	  error("invalid mime type \"" +
        this.content_type + "\"");
	  return false;
	}
	return true;
  }
                                        /* ................................ */
  private int
  header(                               /* read a multipart header          */
    DataInputStream stream)
  throws IOException
  {
	String s = stream.readLine();		/* read boundary line of header 	*/

	if (this.boundary.length() == 0)	/* first time at boundary? 			*/
	  this.boundary = s;				/* make that be doc standard 		*/
	
	if (s.equals(this.boundary+"--"))	/* end of document? 				*/
	{
	  this.set_state("end of sequence");
	  return 0;
	}
	if (!s.equals(this.boundary))		/* not a boundary line?				*/
	{
	  this.set_state("garbage in header");
	  this.debug("garbage is: \"" + s + "\"");
	  return 0;
	}
	this.content_type = "unknown";
    int l = 0;
    do {
      s = stream.readLine().			/* get next line from input stream 	*/
        toLowerCase();
	  this.debug("header line is \"" + 
        s + "\"");
      if (s.startsWith(					/* this is Content-type? 			*/
            "content-type: "))
        this.content_type =
          this.value(s);
      if (s.startsWith(					/* this is Content-length? 			*/
            "content-length: "))
        l = Integer.parseInt(
          this.value(s));
      if (s.startsWith(					/* this is an mj feedback field?	*/
            "mj-"))
        this.callback.callback_header(
          this.keyword(s), 
          this.value(s));
    } while (s.length() > 0);			/* go until blank line is found 	*/
	
	if (!this.content_type.equals(		/* content type invalid?			*/
          "image/jpeg") &&
		!this.content_type.equals(
          "image/gif"))
	{
	  this.debug("invalid content_type is \"" + this.content_type + "\"");
	  l = 0;
	}

	if (l == 0)
	  this.set_state("incomplete header");
    return l;
  }
                                        /* ................................ */
  private int
  trailer(								/* read a multipart trailer         */
    DataInputStream stream)
  throws IOException
  {
	String s;
	do {								/* read blank lines from stream 	*/
	  s = stream.readLine();	
	} while (s.length() == 0);

	if (s.equals(this.boundary))		/* end of multipart part? 			*/
	  return 1;							/* do another one 					*/
	if (s.equals(this.boundary + "--"))	/* end of multipart document?		*/
	  return 0;							/* do no more						*/
	this.set_state(
      "garbage after image");
	return -1;						/* error 							*/
  }

/*..........................................................................
 *..........................................................................*/
  private
  void
  debug(
    String s)
  {
	if (this.debug_id != null &&
		!this.debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
  }	

/*..........................................................................
 *..........................................................................*/
  private
  void
  error(
    String s)
  {
	if (debug_id != null &&
		!debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
	else
	  System.out.println(s);
  }	

/*..........................................................................
 *..........................................................................*/
  public
  void
  set_debug_id(
    String debug_id)
  {
	this.debug_id = debug_id;
  }	
} 										/* end class 						*/
