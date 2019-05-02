import java.util.Scanner;

public class SimpleIO{
    
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        String query = "";
        while(!query.equals("quit")){
            query = input.nextLine();
            System.err.println(">>> received: " + query);
            try{
                Thread.sleep(50);
            }
            catch(Exception e){
                continue;
            }
            System.out.println("echo: " + query);
            System.out.flush();
        }
    }
}
