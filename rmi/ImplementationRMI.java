import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class ImplementationRMI extends UnicastRemoteObject implements RMI_Interface {
    private int monotonic_counter;    
    public ImplementationRMI (String name) throws RemoteException {
        super();
        try {
            Naming.rebind(name, this);
            monotonic_counter = 0;
        } 
        catch (Exception e) { 
            System.out.println("Exception: " + e.getMessage());
            e.printStackTrace();
        }
    }
    public synchronized int getCount() throws RemoteException { 
        monotonic_counter++;
        return monotonic_counter;
    }
}