import java.rmi.*;
import java.rmi.server.*;

public class Server {
    public static void main(String args[]) {         
        System.setSecurityManager(new RMISecurityManager());
        try {            
            ImplementationRMI monotonic_counter_implementation = new ImplementationRMI("rmi://localhost/Server");
            System.out.println("Monotonic counter created.");
        }
        catch (Exception e){
            System.out.println("Exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
}