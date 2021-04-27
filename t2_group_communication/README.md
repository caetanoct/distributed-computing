# Process Group Communication Project

## Ambient Setup

Use this command to check jgroups version.
```sh
java -jar jgroups-5.0.0.Final.jar
```

Use this command to extract files to jgroup directory.
```bash
cd jgroups
jar -xf ../jgroups-5.0.0.Final.jar
```

In two different terminals do this to test Jgroups:

|Terminal 1  | Terminal 2  |
|--|--|
| `java org.jgroups.demos.Draw` | `java org.jgroups.demos.Draw` |

This should open a draw application.

## Useful links

* [1. Site](http://www.jgroups.org/api.html)
* [2. Manual](http://www.jgroups.org/manual5/index.html)
* [3. Javadoc](http://www.jgroups.org/javadoc5/index.html)
* [4. Tutorials](http://www.jgroups.org/tutorial5/index.html)

## Example JGroups code with comments

Adapted from http://www.jgroups.org/tutorial5/index.html.

```Java
import org.jgroups.*;
import org.jgroups.util.Util;

import java.io.*;
import java.util.List;
import java.util.LinkedList;
/*
    register as a Receiver to receive message and view changes. 
    To this end, we implement org.jgroups.Receiver, 
    which has default implementations, 
    and override callbacks receive() and viewChange():
*/
public class SimpleChat implements Receiver {
    JChannel channel;
    String user_name=System.getProperty("user.name", "n/a");
    /*
        ReceiverAdapter defines a callback getState()
        which is called on an existing instance (usually the coordinator)
        to fetch the cluster state. 

        In our demo application,we define the state to be the chat conversation.
        This is a simple list, to the tail of which we add every message we receive.
        (Note that this is probably not the best example for state, as this state always grows.
        As a workaround, we could have a bounded list, which is not done here though).
    */
    final List<String> state=new LinkedList<>();
    /*
        The viewAccepted() callback is called whenever a new instance joins the cluster,
        or an existing instance leaves (crashes included).
        Its toString() method prints out the view ID (an increasing ID)
        and a list of the current instances in the cluster

        instances should have the exact same view with the same ordering of its instances in the list.
    */
    public void viewAccepted(View new_view) {
        System.out.println("** view: " + new_view);
        System.out.println("---\nview members connected to the cluster: \n" + new_view.getMembers() + "\n---");
    }
    /*
        In receive(), we get a Message as argument.
        We simply get its byte array as an object (again using Java serialization) and print it to stdout.
        We also print the sender’s address (Message.getSrc()).
        additionally im printing the dest address (which is null)

        Note that we could also get the byte array (the payload) by calling Message.getArray()
        and then de-serializing it ourselves, e.g. String line=new String(msg.getArray(),
        msg.getOffset(), msg.getLength()).
    */
    public void receive(Message msg) {
        String line="src: (" + msg.getSrc() + ") -> dest: (" + msg.getDest() + ") obj: (" + msg.getObject() + ")";        
        System.out.println(line);        
        synchronized(state) {
            // add each received message to our state
            state.add(line);
        }
    }
    /*
        The getState() method is called in the state provider,
        ie. an existing instance, to return the shared cluster state.
        It is passed an output stream to which the state has to be written.

        Note that JGroups closes that stream automatically after the state has been written,
        even in the case of an exception, so the stream doesn’t need to be closed.

        Since access to state may be concurrent, we synchronize it.
        Then we call Util.objectToStream() which is a JGroups utility method
        writing an object to an output stream.
    */
    public void getState(OutputStream output) throws Exception {
        synchronized(state) {
            Util.objectToStream(state, new DataOutputStream(output));
        }
    }
    /*
        The setState() method is called on the state requester,
        ie. the instance which called JChannel.getState().
        Its task is to read the state from the input stream and set it accordingly:

        We again call a JGroups utility method (Util.objectFromStream())
        to create an object from an input stream.

        Then we synchronize on state, and set its contents from the received state.
        We also print the number of messages in the received chat history to stdout.

        Note that this is not feasible with a large chat history,
        but - again - we could have a bounded chat history list.
    */
    public void setState(InputStream input) throws Exception {
        List<String> list=Util.objectFromStream(new DataInputStream(input));
        synchronized(state) {
            state.clear();
            state.addAll(list);
        }
        System.out.println("received state (" + list.size() + " messages in chat history):");
        list.forEach(System.out::println);
    }


    private void start() throws Exception {
        // creates a jchannel with default configs
        channel=new JChannel().setReceiver(this);
        // connects in specified cluster, the connect creates the cluster if it is the first instance
        channel.connect("ChatCluster");
        /*
            The getState() method’s first argument is the target instance,
            and null means get the state from the first instance (the coordinator).
            The second argument is the timeout; 
            here’s we’re willing to wait for 10 secs to transfer the state.
            If the state cannot be transferred within this time,
            then an exception will be thrown. 0 means wait forever.
        */
        channel.getState(null, 10000);
        eventLoop();
        channel.close();
    }
    // reads from stdin and send to all members in current cluster, if quit or exit is typed exit cluster and close it
    private void eventLoop() {
        BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
        while(true) {
            try {
                System.out.print("> "); System.out.flush();
                String line=in.readLine().toLowerCase();
                if(line.startsWith("quit") || line.startsWith("exit")) {
                    break;
                }
                line="[" + user_name + "] " + line;
                /*
                    The event loop blocks until a new line is ready (from standard input),
                    then sends a message to the cluster. 
                    This is done by creating a new ObjectMessage
                    (a message containing the destination and sender’s addresses,
                    plus an object as payload) and calling Channel.send() with it as argument.

                    The first argument of the ObjectMessage constructor is the destination address.
                    A null destination address sends the message to everyone in the cluster
                    (a non-null address of an instance would send the message to only one instance).

                    The second argument is the line that we read from stdin.
                */
                Message msg=new ObjectMessage(null, line);
                // The message will be sent to the cluster and therefore it will be received by both instances, including the sender.
                channel.send(msg);
            }
            catch(Exception e) {
            }
        }
    }


    public static void main(String[] args) throws Exception {
        new SimpleChat().start();
    }
}
```