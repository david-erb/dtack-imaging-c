import java.applet.*;
import java.awt.*;
import java.net.*;
import java.io.*;
import dt.*;

public 
class 
umdemo 
  extends Applet
  implements dtsquawk_i
{
  protected TextField host;
  protected TextField port;
  protected TextField filename;
  protected Label response;

/*..........................................................................
 *..........................................................................*/
public
String
squawk()
{
  return "umdemo";
}

/*..........................................................................
 *..........................................................................*/
public
void
init()
{
  System.out.println(this.squawk()+".init: " +
    " initializing x");
  try {
	FlowLayout left =
      new FlowLayout(FlowLayout.LEFT);
	GridLayout grid =
      new GridLayout(4, 1);

	Panel host_panel = new Panel();
	Panel file_panel = new Panel();
	Panel button_panel = new Panel();
	Panel response_panel = new Panel();

	this.setLayout(grid);
	this.add(host_panel);
	this.add(file_panel);
	this.add(button_panel);
	this.add(response_panel);

	host_panel.setLayout(left);
	file_panel.setLayout(left);
	button_panel.setLayout(left);
	response_panel.setLayout(left);

	host = new TextField(24);
	port = new TextField(8);
	filename = new TextField(40);

	response = new Label(
      "          " +
      "          " +
      "          " +
      "          " +
      "          " +
      "          " +
      "          " +
      "          " +
      "          " +
      "          ");

	host.setText("d");
	port.setText("5711");

	host_panel.add(new Label("Host:"));
	host_panel.add(host);
	host_panel.add(new Label("Port:"));
	host_panel.add(port);

	file_panel.add(new Label("Filename:"));
	file_panel.add(filename);
	file_panel.add(new Button("Use"));

	button_panel.add(new Label(""));
	button_panel.add(new Button("Play"));
	button_panel.add(new Button("Step"));
	button_panel.add(new Button("Stop"));
	button_panel.add(new Button("Status"));

	response_panel.add(new Label("Response:"));
	response_panel.add(response);
	response_panel.add(new Label("                                      "));
  }
  catch(Exception e)
  {
	System.out.println("umdemo.init: exception " + e);
  }
}

/*..........................................................................
 *..........................................................................*/
private
void
transact(
  String command)
{
  try {
	Socket socket = new Socket(			/* contact server 					*/
      InetAddress.getByName(
        host.getText().trim()),
      Integer.parseInt(
        port.getText().trim()));
	System.out.println(
      "umdemo.transact: created " + 
      socket.toString());

	PrintStream output =				/* create output stream on socket 	*/
      new PrintStream(
        socket.getOutputStream(), 
        true);
    System.out.println(
      "umdemo.transact: sending" +
      " \"" + command + "\"");
	output.println(command);			/* output command to server			*/

	DataInputStream input =				/* create input stream on socket 	*/
      new DataInputStream(
        socket.getInputStream());
	response.setText(input.readLine());	/* get response line from server 	*/

	output.println("");					/* tell server we are done 			*/

	System.out.println(
      "umdemo.transact: response: " +
      response.getText());
  }
  catch(Exception e)
  {
	System.out.println(
      "umdemo.transact: " + e);
	response.setText(e.toString());
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
  System.out.println("umdemo.action: \"" + object + "\"");
  try {
	if (object == "Use" ||
		event.target == filename)
	  this.transact("input " +
        filename.getText().trim());
	else
	if (object == "Play")
	  this.transact("forward");
	else
	if (object == "Step")
	  this.transact("pause");
	else
	if (object == "Stop")
	  this.transact("quit");
	else
	if (object == "Status")
	  this.transact("status");
  }
  catch(Exception e)
  {
	System.out.println(
      "umdemo.action: " + e.toString());
	response.setText(e.toString());
  }
  return true;
}

}										/* end class 						*/


