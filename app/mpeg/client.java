import java.applet.*;
import java.awt.*;
import java.net.*;
import java.io.*;

public class umdemo extends Applet
{
  protected TextField host;
  protected TextField port;
  protected TextField filename;
  protected TextField message;

/*..........................................................................
 *..........................................................................*/
public
class
client
  extends Object
  implements Runnable, ReadCallback
{
  protected Socket socket;
  protected DataOutputStream output;
  protected Thread client;
  protected ReadThread reader;

										/* ................................ */
public client(
  String host,
  int port)
throws IOException
{
  socket = new Socket(host, port);
  output = new DataOutputStream(socket.getOutputStream);
  reader = new ReadThread(this, socket);
  reader.start()
}
										/* ................................ */
public
void
start()
{
  client = new Thread(this);
  client.start();
}
										/* ................................ */
public
void
stop()
{
  client.stop();
  client = null;
}
										/* ................................ */
public
synchronized
void
send(
  String string)
throws IOException
{
  System.out.println("client.send: \"" + string + "\"");
  output.writeUTF(string);
}
										/* ................................ */
public
void
run()
{
}

										/* ................................ */
public
void
disconnect()
{
  try {
	reader.closeConnection();
  }
  catch(Exception exception);
  {
  }

public 
synchronized
void
dataReady(
  String string)
{
  System.out.println("client.dataReady: \"" + string + "\"");
}
}

/*..........................................................................
 *..........................................................................*/
public
void
init()
{
  System.out.println("umdemo: initializing x");
  try {

	Panel host_panel = new Panel();
	Panel file_panel = new Panel();
	Panel button_panel = new Panel();
	Panel message_panel = new Panel();

	this.add(host_panel);
	this.add(file_panel);
	this.add(button_panel);
	this.add(message_panel);

	host = new TextField(24);
	port = new TextField(8);
	filename = new TextField(40);
	message = new TextField(40);

	host_panel.add(new Label("Host:"));
	host_panel.add(host);
	host_panel.add(new Label("Port:"));
	host_panel.add(port);

	file_panel.add(new Label("Filename:"));
	file_panel.add(filename);

	button_panel.add(new Button("Play"));
	button_panel.add(new Button("Step"));
	button_panel.add(new Button("Quit"));
	button_panel.add(new Button("Exit"));

	message_panel.add(new Label("Message:"));
	message_panel.add(message);
  }
  catch(Exception e)
  {
	System.out.println("umdemo.init: exception " + e);
  }
}

/*..........................................................................
 *..........................................................................*/
public
boolean
action(
  Event event,
  Object object)
{
  System.out.println("umdemo: action \"" + object + "\"");
  try {
	if (object == "Play")
	{  
	  client_c client = new client_c(
	}
  }
  catch(Exception e)
  {
	System.out.println("umdemo.action: exception " + e);
  }
  return true;
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


