import java.util.*;
import java.io.*;

public class pipe{
  private File f1, f2;
  public pipe(int pid_in, int pid_out){
    f1 = new File("" + pid_in);
    f2 = new File("" + pid_out);
  }
  public boolean hasNext(){
    try{
      Scanner in = new Scanner(f1);
      boolean next = in.hasNext();
      in.close();
      return next;
    }catch(Exception e){
    }
    return false;
  }
  public String nextLine(){
    try{
      Scanner in = new Scanner(f1);
      String input = in.nextLine();
      in.close();
      return input;
    }catch(Exception e){
    }
    return "";
  }
  public void writeOut(String output){
    try{
      FileWriter out = new FileWriter(f2);
      out.write(output);
      out.flush();
      out.close();
    }catch(Exception e){
    }
  }
}
