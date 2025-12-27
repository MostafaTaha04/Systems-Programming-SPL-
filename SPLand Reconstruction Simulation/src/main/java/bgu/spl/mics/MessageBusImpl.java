package bgu.spl.mics;

import java.util.concurrent.*;

/**
 * The {@link MessageBusImpl} class is the implementation of the MessageBus interface.
 * It manages message delivery and event handling for microservices.
 */
public class MessageBusImpl implements MessageBus {
    private static class MessageBusImplHolder {
        private static MessageBusImpl instance = new MessageBusImpl();
    }
    
    private final ConcurrentHashMap<MicroService, LinkedBlockingQueue<Message>> micros = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<Event<?>, Future<?>> futureMap = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<Class<? extends Event<?>>, BlockingQueue<MicroService>> eventMessages = new ConcurrentHashMap<>();
    private final ConcurrentHashMap<Class<? extends Broadcast>, BlockingQueue<MicroService>> broadcastMessages = new ConcurrentHashMap<>();

    private MessageBusImpl() {}

    public static MessageBusImpl getInstance() {
        return MessageBusImplHolder.instance;
    }

    /**
     * @PRE: `type` is a non-null class of a Broadcast, and `m` is a non-null MicroService.
     * @POST: Subscribes the given microservice to the broadcast type.
     */
    @Override
    public <T> void subscribeEvent(Class<? extends Event<T>> type, MicroService m) {
        eventMessages.putIfAbsent(type, new LinkedBlockingQueue<>());
        eventMessages.get(type).add(m);
    }

    /**
     * @PRE: `type` is a non-null class of a Broadcast, and `m` is a non-null MicroService.
     * @POST: Subscribes the given microservice to the broadcast type.
     */
    @Override
    public void subscribeBroadcast(Class<? extends Broadcast> type, MicroService m) {
        broadcastMessages.putIfAbsent(type, new LinkedBlockingQueue<>());
        broadcastMessages.get(type).add(m);
    }

    /**
     * @PRE: `e` is a non-null Event, and `result` is a valid result of type `T`.
     * @POST: Completes the event with the provided result and resolves the associated Future.
     */
    @Override
    public <T> void complete(Event<T> e, T result) {
        Future<T> future = (Future<T>) futureMap.remove(e);
        if (future != null) 
            future.resolve(result);
    }

    /**
     * @PRE: `b` is a non-null Broadcast.
     * @POST: Sends the broadcast message to all subscribed microservices.
     */
    @Override
    public void sendBroadcast(Broadcast b) {
        BlockingQueue<MicroService> subscribers = broadcastMessages.get(b.getClass());
        if (subscribers != null) {
            for (MicroService m : subscribers) {
                LinkedBlockingQueue<Message> q = micros.get(m);
                if (q != null) {
                    q.add(b);
                }
            }
        }
    }

    /**
     * @PRE: `e` is a non-null Event.
     * @POST: Sends the event to a subscribed microservice and returns a Future for the result.
     * @POST: If no microservices are available for the event, returns `null`.
     */
    @Override
    public <T> Future<T> sendEvent(Event<T> e) {
        BlockingQueue<MicroService> subscribers = eventMessages.get(e.getClass());
        if (subscribers == null || subscribers.isEmpty()) {
            return null;
        }
        MicroService m;
        synchronized(subscribers){
            m = subscribers.poll();
            if (m == null) {
                return null;
            }
            subscribers.add(m);
        }

        Future<T> future = new Future<>();
        futureMap.put(e, future);

        LinkedBlockingQueue<Message> q = micros.get(m);
        if (q != null) {
            q.add(e);
        }
        return future;
    }

    /**
     * @PRE: `m` is a non-null MicroService.
     * @POST: Registers the microservice by adding it to the list of microservices.
     */
    @Override
    public void register(MicroService m) {
        micros.putIfAbsent(m, new LinkedBlockingQueue<>());
    }

    /**
     * @PRE: `m` is a non-null MicroService.
     * @POST: Unregisters the microservice by removing it from the system and cleaning up pending messages.
     */
    @Override
    public void unregister(MicroService m) {
        LinkedBlockingQueue<Message> q = micros.remove(m);
        synchronized(q){
            if (q != null) {
                while (!q.isEmpty()) {
                    Message msg = q.poll();
                    if (msg instanceof Event) {
                        Future<?> future = futureMap.remove(msg);
                        if (future != null) {
                            future.resolve(null);
                        }
                    }
                }
            }
        }


        synchronized(broadcastMessages){
            for (BlockingQueue<MicroService> micros : broadcastMessages.values()){
                micros.remove(m);
            }
        }

        synchronized(eventMessages){
            for (BlockingQueue<MicroService> micros : eventMessages.values()){
                micros.remove(m);
            }
        }
    }

    /**
     * @PRE: `m` is a non-null MicroService.
     * @POST: Returns the next available message for the microservice to process. 
     * @POST: If the message queue is empty, the method will block until a message is available.
     */
    @Override
    public Message awaitMessage(MicroService m) throws InterruptedException {
        LinkedBlockingQueue<Message> q = micros.get(m);
        if (q == null) {
            throw new IllegalStateException("MicroService is not registered");
        }
        return q.take();
    }
}