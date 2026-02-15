package bgu.spl.net.srv;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private static final ConcurrentHashMap<String, String> users = new ConcurrentHashMap<>();
    private static final ConcurrentHashMap<Integer, String> loggedInUsers = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<Integer, ConnectionHandler<T>> clients = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<String, Set<Integer>> Subscribers = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<String,String> IdChannels = new ConcurrentHashMap<>();
    private static ConnectionsImpl<?> instance;

    private ConnectionsImpl() {}

    public boolean send(int connectionId, T msg){
        ConnectionHandler<T> CH = clients.get(connectionId);
        if(CH != null){
            CH.send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg){
        Set<Integer> channelSubscribers = Subscribers.get(channel);
        if(channelSubscribers != null){
            for(Integer connectionId : channelSubscribers){
                send(connectionId, msg);
            }
        }
    }

    public void disconnect(int connectionId){
        Subscribers.forEach((channel,users) -> {
        users.remove(connectionId);});
    }    
        
    
    public void subscribe(String channel, int connectionId, String id) {
        if(!Subscribers.containsKey(channel)){
            Subscribers.put(channel, new HashSet<Integer>());
        }
        Subscribers.get(channel).add(connectionId);
        IdChannels.put(id, channel);

    }
    
    public void unsubscribe(String id, int connectionId) {
        Set<Integer> channelSubscribers = Subscribers.get(IdChannels.get(id));
        channelSubscribers.remove(connectionId);
    }

    public static synchronized <T> ConnectionsImpl<T> getInstance() {
        if (instance == null) {
            instance = new ConnectionsImpl<>();
        }
        return (ConnectionsImpl<T>) instance;
    }

    public void addClient(int connectionId,  ConnectionHandler<T> handler) {
        clients.put(connectionId, handler);
    }

    public ConcurrentHashMap<String, String> getChannels() {
        return IdChannels;
    }
    public static ConcurrentHashMap<Integer, String> getLoggedInUsers() {
        return loggedInUsers;
    }

    public static ConcurrentHashMap<String, String> getUsers() {
        return users;
    }
}
