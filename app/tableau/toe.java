import java.awt.*;
import java.awt.image.*;
import java.net.*;
import java.applet.*;

/**
 * A TicTacToe applet. A very simple, and mostly brain-dead
 * implementation of your favorite game! <p>
 *
 * In this game a position is represented by a white and black
 * bitmask. A bit is set if a position is ocupied. There are
 * 9 squares so there are 1<<9 possible positions for each
 * side. An array of 1<<9 booleans is created, it marks
 * all the winning positions.
 *
 * @version 	1.2, 13 Oct 1995
 * @author Arthur van Hoff
 * @modified 96/04/23 Jim Hagen : winning sounds
 */
public
class toe extends Applet {
    public void paint(Graphics g) {
	  g.drawString("push", 0, 50);
    }
}
