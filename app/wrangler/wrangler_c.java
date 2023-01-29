package dtack.app.robot;

import java.net.*;
import java.awt.*;
import java.io.*;
import java.util.*;
import java.applet.*;
import java.awt.Image;
import java.awt.Event;
import sun.awt.image.*;
import java.awt.image.*;

public
class
applet03_c
extends Applet
  implements callback_i, dtack.app.sync.callback_i
{
  private String debug_id = "";
  private dtack.lib.tab.tabcard_c tabcard;
  private int warm_time = 0;
  private String video_name;
  private Applet video_applet = null;
  private connect_c connect_card;
  private operate2_c operate2_card;
  private dtack.lib.tab.tab_c tab =		// currently raised tab
    null;
  private connection_c connection;
  private boolean idle = true;
  private dtack.app.sync.sync_c sync = null;

/*..........................................................................
 *..........................................................................*/
  public void 
  init()								// initialize the applet           
  {
	this.populate();					// populate the forms

	String s;
	s = getParameter("debug_id");		// get our name for debug		   
	if (s != null && s.length() > 0)
	  this.debug_id = s;
  
	s = getParameter("url");			/* get <PARAM NAME=host>			*/
	if (s == null)						/* no host param given? 			*/
	  s = getCodeBase().getHost();	
	this.host = s;

    s = getParameter("port");			// get <PARAM NAME=port>            
	if (s == null)						/* no port param given? 			*/
	  s = "5770";
	this.port = s;
	
    s = getParameter("file");			// get <PARAM NAME=file>            
	if (s == null)						/* no file param given? 			*/
	  s = "5770";
	this.file = s;

    s = getParameter("video");			// get <PARAM NAME=video>            
	if (s == null)						/* no video param given? 			*/
	  s = "video";
	this.video = s;
  }

/*..........................................................................
 *..........................................................................*/
  public void 
  start()								// start applet
  {
	try
	{
      URL url = new URL(				/* make the url for the image       */
        this.url_string);
	  
	  if (!url.getProtocol().			// make sure url is reasonable
		    equals("http"))
	  {
		error("invalid protocol \"" +
		  url.getProtocol() + "\"");
		return;
	  }

	  this.set_state(">Resolving \"" + 
        url.getHost() + "\"");

	  InetAddress address = 
        InetAddress.getByName(
          url.getHost());

	  int port = url.getPort();
	  if (port == -1)
		port = 80;
	  this.set_state(">Connecting" +
        " to " + url.getHost() + 
        " port " + 
        Integer.toString(port));
	  this.socket = new Socket(			// create the new socket
        address, port, true);

	  this.set_state(">Connected" +
        " to " + url.getHost() + 
        " port " + 
        Integer.toString(port));
	  this.ostream = new PrintStream(	// stream for output on socket
        socket.getOutputStream());
	  this.istream = new				// stream for input from socket
        DataInputStream(
          socket.getInputStream());

	  this.ostream.println("GET " + 
        this.file + " HTTP/1.0");

	  this.set_state(">Waiting for port"
        " from " + url.getHost())

	  while (true)						// while queued
	  {
		String line =					// get notification
          this.istream.readLine();
		if (line.startsWith("port:"))	// clear to go?
		{
		  break;
		}
	  }
	}
	catch(Exception e)
	{
	  this.error(e.toString());
	  this.socket = null;
	}

	if (this.video_name != null)		// supposed to sync?
	{
	  this.sync = new					// create sync object
	    dtack.app.sync.sync_c(
          getParameter("name"),
          getParameter("sync"),
          getAppletContext(),
          this);

	  this.sync.start();				// start syncing
	}
  }

/*..........................................................................
 *..........................................................................*/
  private
  void
  debug(								// debug to java console
    String s)
  {
	if (debug_id != null &&
		!debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
  }	

/*..........................................................................
 *..........................................................................*/
  private
  void
  error(								// show error on applet window
    String s)
  {
	if (debug_id != null &&
		!debug_id.equals(""))
	  System.out.println(debug_id + ": " + s);
	else
	  System.out.println(s);
    this.set_state("Error: " + s);
  }	
