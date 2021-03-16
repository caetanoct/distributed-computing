import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;

public class Client { 
    public static void main(String args[]) {        
        System.setSecurityManager(new RMISecurityManager());
        try {
            String name = "rmi://localhost/Server";
            RMI_Interface myCount = (RMI_Interface) Naming.lookup(name);
            long t0 = System.currentTimeMillis();
            System.out.println("--- Remote Method Invocation ---");
            /* used for testing method getCount()
            for (int i = 0 ; i < 100000 ; i++) {
                System.out.println("The getcount() value is: "+myCount.getCount());
            }
            */
            System.out.println("The getcount() value is: "+myCount.getCount());
            System.out.println("--- End of Remote Method Invocation ---");
            long t = System.currentTimeMillis();
            System.out.println("Average response time: " + ((t - t0)) + "ms");
        } 
        catch(Exception e) { 
            System.err.println("System Exception" + e);
        }
        System.exit(0);
  }
}