import java.rmi.Remote;
import java.rmi.RemoteException;

public interface TableInterface extends Remote{

	public String get(int key) throws RemoteException;
	public void put(int key, String value) throws RemoteException;
	
}
