import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject; 
import java.util.HashMap;

public class Table extends UnicastRemoteObject implements TableInterface{
	HashMap <Integer,String>table;
	
	public Table() throws RemoteException {
		table = new HashMap<Integer,String>();
	}
	
	@Override
	public String get(int key) throws RemoteException {
		return table.get(key);
	}

	@Override
	public void put(int key, String value) throws RemoteException {
		table.put(key, value);		
	}

}
