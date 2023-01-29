package mj13;

import java.applet.*;
import java.awt.*;

public
class
synclet_c
extends Applet
{
  private int count;
  private boolean synced;
  private static final int MAX = 10;
  private boolean registered[];
  private String names[];
  private String debug_id = null;
  
/*..........................................................................
 *..........................................................................*/
  public
  void
  init()
  {
	this.synced = false;
	this.names = new String[MAX];
	this.registered = new boolean[MAX];
	this.count = 0;
	this.debug_id = getParameter("debug_id");
	int i;
	for (i=0; i<MAX; i++)
	{
	  this.names[i] = getParameter("name" + i);
	  if (this.names[i] == null)
		break;
	  this.count++;
	  if (this.debug_id != null)
		System.out.println(this.debug_id + 
          ": looking for " + this.names[i]);
	}
  }

/*..........................................................................
 *..........................................................................*/
  public
  void
  start()
  {
	if (false)							// was here for debug once
	while (true)
	{
	  try 
	  {
		Thread.sleep(1000);
		System.out.println(this.debug_id + ": tick");
	  }
	  catch(Exception e) 
	  {
		System.out.println(this.debug_id + ": " + e.toString());
	  }
	}
  }
  
/*..........................................................................
 *..........................................................................*/
  private
  synchronized  
  void
  register(
    String name)			
  {
	if (this.synced)					// already did it?
	  return;
	int i;
	int count = 0;
	for (i=0; i< this.count; i++)		// loop through list of applets
	{
	  if (names[i].equals(name))
	  {
		registered[i] = true;
		if (this.debug_id != null)
		System.out.println(this.debug_id + 
          ": registered " + name);
	  }
	  if (registered[i])
		 count++;
	}
	if (count < this.count)				// somebody not registered yet?
	  try {this.wait();}				// this thread must wait
	    catch(Exception e) {}
	else
	if (!this.synced)
	{
	  this.notifyAll();					// wake everybody up
	  this.synced = true;
	}
  }
  
/*..........................................................................
 *..........................................................................*/
  public 
  String
  getParameter(
    String name)			
  {
	if (name.charAt(0) == '+')			// special name?
	{
	  this.register(name.substring(1));	// register it
	  return null;
	}
	else								// standard name?
	  return super.getParameter(name);	// get it from html
  }
}
