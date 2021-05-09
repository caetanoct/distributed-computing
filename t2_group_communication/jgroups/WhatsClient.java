import org.jgroups.*;
import java.io.*;
import java.sql.Timestamp;
/*
    The WhatsClient will be a receiver in order to receive messages and views.

    receive messages and log it to unique files, each file will have the local address indentifier as the name.
    send messages to all members in group.
    log members leaving and joining the group.
*/
public class WhatsClient implements Receiver {
    String host_id=System.getProperty("user.name", "n/a");
    Address local_address = null;
    static String cluster_name = null;
    View local_view = null;    
    /*
        The viewAccepted() callback is called whenever a new instance joins the cluster,
        or an existing instance leaves (crashes included).
        Its toString() method prints out the view ID (an increasing ID)
        and a list of the current instances in the cluster

        instances should have the exact same view with the same ordering of its instances in the list.
    */
    public void viewAccepted(View new_view) {        
        this.local_view = new_view;        
        if (this.local_address != null) {
        	System.out.println("A member joined/left the group. Members in Group now are:");
	        this.log_to_history("A member joined/left the group. Members in Group now are:");
	        for (Address addr : this.local_view.getMembers()) {
	        	if (addr.toString() == this.local_address.toString()) {
	        		this.log_to_history(addr.toString()+"(me)");
	        		System.out.println(addr.toString()+"(me)");
	        	} else {
	        		this.log_to_history(addr.toString());
	        		System.out.println(addr.toString());
	        	}
            }
        }
    }
    // log received messages to local archive with local addr as the archive name
    private void log_to_history(String msg) {
        if (local_address == null) {
            System.out.println("Cant log to history, local address was not defined");
        } else {
            Timestamp timestamp = new Timestamp(System.currentTimeMillis());
            String line = timestamp.toString() + ": " + msg;
            try {
                PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(cluster_name+"-"+local_address.toString()+".history", true)));
                out.println(line);
                out.close();
            } catch (IOException e) {
                e.printStackTrace();
                System.out.println(e);
            }
        }
    }
    /*
        In receive(), we get a Message as argument.
        We get its byte array as an object print it and log it
    */
    public void receive(Message msg) {    
        //String line="src: (" + msg.getSrc() + ") -> dest: (" + (msg.getDest() == null ? "everyone" : msg.getDest()) + ") message: (" + msg.getObject() + ")";
        //String line="(" + msg.getSrc() + ") -> me message: " + msg.getObject();
    	String line = msg.getObject();
    	System.out.println(msg.getSrc().toString() == this.local_address.toString() ? line + " (sent message)" : line + " (received message)");
        // log received message to local client archive
        this.log_to_history(msg.getSrc().toString() == this.local_address.toString() ? line + " (sent message)" : line + " (received message)");
    }
    // read from stdin and send a message for members in group
    private boolean send_message(JChannel channel) {
    	boolean stop = false;
        BufferedReader buffer=new BufferedReader(new InputStreamReader(System.in));
        //System.out.println("Type the message you want to send to your group.");
        System.out.print("\n> ");
        System.out.flush();
        try {
            String line=buffer.readLine();
            if(line.startsWith("quit")) {                
                stop = true;
            }
            line="[" + this.local_address + "] " + line;                       
            for (Address addr : this.local_view.getMembers()) {
            	Message msg=new ObjectMessage(addr, line);
	            try {
	                // The message will be sent to the cluster and therefore it will be received by all instances.
	                channel.send(msg);
            		System.out.println(stop == true ? "Sending \"quit\" message to member " + addr + "." : "Message to member "+ addr + " was sent.");
	            }
	            catch (Exception e) {
	                e.printStackTrace();
	                System.out.println(e);
	            }    
            }
        }
        catch (IOException ex) {
            ex.printStackTrace();
            System.out.println(ex);
        }
        return stop;
    }
    public static void main(String[] args) throws Exception {
        if (args.length < 1) {
            System.out.println("###### INFO ######\nPlease type the name of the group you want to join as an argument");
            System.exit(1);
        }
        String cluster_name = args[0];
        // client will be the receiver
        WhatsClient client = new WhatsClient();
        // creates a jchannel with tcp configs       
        JChannel channel=new JChannel("tcp.xml");
        channel.setReceiver(client);
        // connects in specified cluster, the connect method creates the cluster if it is the first instance
        channel.connect(cluster_name);
        client.cluster_name = cluster_name;
        client.local_address = channel.address();        
        try {
            PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(client.cluster_name+"-"+client.local_address.toString()+".history", true)));
            out.println("#### CLIENT CHAT HISTORY ####");
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println(e);
        }
        while (true) {
        	boolean stop = client.send_message(channel);
        	if (stop) {
        		client.log_to_history("I "+client.local_address.toString()+" am leaving the group.");
        		channel.close();
        		break;
        	}
        }     
    }
}