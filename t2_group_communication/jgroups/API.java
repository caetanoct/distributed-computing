import org.jgroups.JChannel;
import org.jgroups.ObjectMessage;
import org.jgroups.Receiver;
import org.jgroups.Message;
public class API {
	public static void main(String[] args) throws Exception {
	JChannel channel=new JChannel();
	channel.setReceiver(new Receiver() {
  		public void receive(Message msg) {
      		System.out.println("received msg from " + msg.getSrc() + ": " + msg.getObject());
  		}
	});
	channel.connect("MyCluster");
	channel.send(new ObjectMessage(null, "hello world"));
	channel.close();        
	}
}