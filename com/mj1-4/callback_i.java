package mj14;

public interface callback_i
{
  public void callback_state(String state);
  public void callback_idle();
  public void callback_header(			// called when an http extension header
    String field,						// line starts with "mj"
    String value);
}
