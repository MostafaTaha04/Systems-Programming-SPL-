package bgu.spl.net.impl.stomp;


import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        
        if(args.length < 2) {
            System.out.println("Usage: java StompServer <port> <serverType: tpc/reactor>");
            return;
        }

        int port = Integer.parseInt(args[0]);
        String serverType = args[1].toLowerCase();
        int numThreads = Runtime.getRuntime().availableProcessors();

        if(serverType.equals("tpc")) {
            Server.threadPerClient(port, () -> new StompMessagingProtocolImpl<>(), StompMessageEncoderDecoder::new).serve();
        }
        else if(serverType.equals("reactor")){
            Server.reactor(numThreads, port, () -> new StompMessagingProtocolImpl<>(), StompMessageEncoderDecoder::new).serve();
        }else{
            System.out.println("Invalid server type. Use 'tpc' or 'reactor'.");
        }
    }
}
