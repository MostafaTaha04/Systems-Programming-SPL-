package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T> {

    private int connectionId;
    private Connections<T> connections;
    private int count = 0;
    private boolean shouldTerminate = false;
    private boolean loggedIn = false;
    

    @Override
    public void start(int connectionId, Connections<T> connections) {
        this.connectionId = connectionId;
        this.connections = connections;    
    }

    @Override
    public T process(T message) {
        String frame = message.toString();
        String command = extractCommand(frame);

        if (!loggedIn && !command.equals("CONNECT")) {
            sendError("Not Connected", "Please login first.");
        }
            switch (command) {
                case "CONNECT":
                    handleConnect(frame);
                    break;
                case "SEND":
                    handleSend(frame);
                    break;
                case "SUBSCRIBE":
                    handleSubscribe(frame);
                    break;
                case "UNSUBSCRIBE":
                    handleUnsubscribe(frame);
                    break;
                case "DISCONNECT":
                    handleDisconnect(frame);
                    break;
                default:
                    sendError("Unknown Command", "Invalid STOMP command: " + command);
                }
            return null;
    }
        
        
    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
    
    private String extractCommand(String frame) {
        int endOfCommand = frame.indexOf("\n");
        if (endOfCommand == -1) {
            return null;
        }
        return frame.substring(0, endOfCommand).trim();
    }

    @SuppressWarnings("static-access")
    private void handleConnect(String frame) {
    
        String username = extractHeaderValue(frame, "login");
        String password = extractHeaderValue(frame, "passcode");
        String version = extractHeaderValue(frame, "accept-version");
        if (username == null || password == null) {
            sendError("Missing Credentials", "CONNECT frame missing 'login' or 'passcode' header.");
            return;
        }

        if (version == null || !version.equals("1.2")) {
            sendError("Missing Version", "CONNECT frame missing 'accept-version' header.");
            return;
        }
        if (((ConnectionsImpl<T>) connections).getUsers().containsKey(username)) {
            if (((ConnectionsImpl<T>) connections).getUsers().get(username).equals(password)) {
                if (!((ConnectionsImpl<T>) connections).getLoggedInUsers().containsValue(username)) {
                    ((ConnectionsImpl<T>) connections).getLoggedInUsers().put(connectionId, username);
                    sendConnectedFrame();
                    loggedIn=true;
                } else {
                    sendError("User already logged in", "The user is already logged in.");
                }
                } else {
                    sendError("Wrong password", "Password does not match.");
                }
            } 
        else {
            ((ConnectionsImpl<T>) connections).getUsers().put(username, password);
            ((ConnectionsImpl<T>) connections).getLoggedInUsers().put(connectionId, username);
            sendConnectedFrame();
            loggedIn =true;
        } 
    }

    @SuppressWarnings("unchecked")
    private void sendConnectedFrame() {
        String response = "CONNECTED\nversion:1.2\n\n";
        connections.send(connectionId, (T) response);
    }

    @SuppressWarnings("unchecked")
    private void handleSend(String frame) {
        String destination = extractHeaderValue(frame, "destination");
        count++;
        String body = extractBody(frame);
        if (destination == null) {
            sendError("Malformed frame", "SEND frame missing 'destination' header");
            return;
        }
        String frame1 = "MESSAGE\n subscription:"+((ConnectionsImpl<T>) connections).getChannels().size() 
                        +"\nmessage-id:"+ count +"\ndestination:" + destination + "\n\n" + body+ "\n";
        connections.send(destination.substring(1), (T) frame1);
    }

    @SuppressWarnings("unchecked")
    private void handleSubscribe(String frame) {
        String destination = extractHeaderValue(frame, "destination");
        String id = extractHeaderValue(frame, "id");

        if (destination == null || id == null) {
            sendError("Malformed frame", "SUBSCRIBE frame missing 'destination' or 'id' header");
            return;
        }

        ((ConnectionsImpl<T>) connections).subscribe(destination, connectionId, id);
        
        String receipt = extractHeaderValue(frame, "receipt");
        
        if (receipt != null) {
            connections.send(connectionId, (T) ("RECEIPT\nreceipt-id:" + receipt + "\n"));
        } 
    }

    @SuppressWarnings("unchecked")
    private void handleUnsubscribe(String frame) {
        String id = extractHeaderValue(frame, "id");
        
        if (id == null) {
            sendError("Malformed Frame", "UNSUBSCRIBE frame missing 'id' header.");
            return;
        }
        
        ((ConnectionsImpl<T>) connections).unsubscribe(id, connectionId);

        String receipt = extractHeaderValue(frame, "receipt");
        
        if (receipt != null) {
            connections.send(connectionId, (T) ("RECEIPT\nreceipt-id:" + receipt + "\n\n"));
        }
    }
    
    @SuppressWarnings({ "unchecked", "static-access" })
    private void handleDisconnect(String frame) {
        String receipt = extractHeaderValue(frame, "receipt");
        
        if (receipt != null) {
            connections.send(connectionId, (T) ("RECEIPT\nreceipt-id:" + receipt + "\n\n"));
        }
        ((ConnectionsImpl<T>) connections).getLoggedInUsers().remove(connectionId);
        ((ConnectionsImpl<T>) connections).disconnect(connectionId);
        shouldTerminate = true;
    }

    @SuppressWarnings("unchecked")
    private void sendError(String message, String description) {
        String errorFrame = "ERROR\n" +
                            "message:" + message + "\n" +
                            "\n" + description + "\n" +
                            "\u0000";
        connections.send(connectionId, (T) errorFrame);

    }

    private String extractHeaderValue(String frame, String headerName) {
        if (frame == null || headerName == null) {
            return null;
        }
        String[] lines = frame.split("\n");
        for (String line : lines) {
            if (line.startsWith(headerName + ":")) {
                return line.substring((headerName + ":").length()).trim();
            }
        }
        return null;
    }
    private String extractBody(String frame) {
        int bodyStartIndex = frame.indexOf("\n\n") + 2;
        return bodyStartIndex > 1 ? frame.substring(bodyStartIndex).trim() : null;
    }
}