import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;

import java.util.Scanner;

public class Client {

	Scanner in; 
	TableInterface table; 
	
	public Client() {
		in = new Scanner(System.in);
	    	if(System.getSecurityManager() == null) {
	    		System.setSecurityManager(new SecurityManager());
	    	}	    	 
	    	try {  
	            table =(TableInterface)Naming.lookup( "rmi://127.0.0.1/Tabela1");  
	    	}
	        catch(RemoteException e ) {  
	            System.out.println();  
	            System.out.println( "RemoteException: " + e.toString() );  
	        }  
	        catch(NotBoundException e ) {  
	            System.out.println();  
	            System.out.println( "NotBoundException: " + e.toString() );  
	        }  
	        catch(Exception e ) {  
	            System.out.println();  
	            System.out.println( "Exception: " + e.toString() );  
	        }
	}
	
	public void execute() {
		String entrada;
		String comando;
		int key;
		String value;
		
		try {
			System.out.println("Entre com um dos comandos a seguir:\n \t\tput <key> <valor>\n\t\tget <key>\n\t\tsair");
			while(true) {
				comando = in.next();
				if(comando.equalsIgnoreCase("put")) {
					key = in.nextInt();
					value = in.next();
					table.put(key,value);
					System.out.println("Adicionou item");
				} else if(comando.equalsIgnoreCase("get")) {
					key = in.nextInt();
					value = table.get(key);
					System.out.println("result: "+ value);
				} else {
					System.out.println("Saindo do programa");
					break;
				}
			}
		in.close();
		}catch(Exception e) {		
			System.out.println( "Exception: " + e.toString()); 
		}
	}
	
	public static void main(String[] args) {
		Client c = new Client();
		c.execute();

	}

}
