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
    static Address local_address = null;
    // log received messages to local archive with local addr as the archive name
    private void log_to_history(String msg) {
        if (local_address == null) {
            System.out.println("Cant log to history, local address was not defined");
        } else {
            Timestamp timestamp = new Timestamp(System.currentTimeMillis());
            String line = timestamp.toString() + ": " + msg;
            try {
                PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(local_address.toString(), true)));
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
        String line="src: (" + msg.getSrc() + ") -> dest: (" + (msg.getDest() == null ? "everyone" : msg.getDest()) + ") obj: (" + msg.getObject() + ")";        
        System.out.println(line);
        // log received message to local client archive
        this.log_to_history(line);
    }
    // read from stdin and send a message for members in group
    private void send_message(JChannel channel) {
        BufferedReader buffer=new BufferedReader(new InputStreamReader(System.in));
        System.out.println("Type the message you want to send to your group.");
        System.out.print("> ");
        System.out.flush();
        try {
            String line=buffer.readLine();
            line="[" + host_id + "] " + line;
            System.out.println(line);
            // null means, send to everyone in group
            Message msg=new ObjectMessage(null, line);
            try {
                // The message will be sent to the cluster and therefore it will be received by all instances.
                channel.send(msg);            
            }
            catch (Exception e) {
                e.printStackTrace();
                System.out.println(e);
            }    
        }
        catch (IOException ex) {
            ex.printStackTrace();
            System.out.println(ex);
        }
    }
    public static void main(String[] args) throws Exception {
        if (args.length < 1) {
            System.out.println("###### INFO ######\nPlease type the name of the group you want to join as an argument");
            System.exit(1);
        }
        String cluster_name = args[0];
        // client will be the receiver
        WhatsClient client = new WhatsClient();
        // creates a jchannel with default configs
        JChannel channel=new JChannel();
        channel.setReceiver(client);
        // connects in specified cluster, the connect method creates the cluster if it is the first instance
        channel.connect(cluster_name);
        local_address = channel.address();
        try {
            PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(local_address.toString(), true)));
            out.println("#### CLIENT CHAT HISTORY ####");
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println(e);
        }     
        client.send_message(channel);
    }
}