import java.rmi.*;
public interface RMI_Interface extends Remote {
	public int getCount() throws RemoteException;
}