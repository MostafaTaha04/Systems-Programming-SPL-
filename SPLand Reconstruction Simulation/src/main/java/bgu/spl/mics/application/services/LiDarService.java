package bgu.spl.mics.application.services;

import bgu.spl.mics.MicroService;
import bgu.spl.mics.application.messages.CrashedBroadcast;
import bgu.spl.mics.application.messages.DetectObjectsEvent;
import bgu.spl.mics.application.messages.TerminatedBroadcast;
import bgu.spl.mics.application.messages.TickBroadcast;
import bgu.spl.mics.application.messages.TrackedObjectsEvent;
import bgu.spl.mics.application.objects.*;

import java.util.List;

/**
 * LiDarService is responsible for processing data from the LiDAR sensor and
 * sending TrackedObjectsEvents to the FusionSLAM service.
 * 
 * This service interacts with the LiDarWorkerTracker object to retrieve and process
 * cloud point data and updates the system's StatisticalFolder upon sending its
 * observations.
 */
public class LiDarService extends MicroService {

    private LiDarWorkerTracker lidarWorkerTracker; // LiDAR Tracker object

    /**
     * Constructor for LiDarService.
     *
     * @param lidarWorkerTracker A LiDAR Tracker worker object that this service will use to process data.
     */
    public LiDarService(LiDarWorkerTracker lidarWorkerTracker) {
        super("LiDarService");
        this.lidarWorkerTracker = lidarWorkerTracker;
    }

    /**
     * Initializes the LiDarService.
     * Registers the service to handle DetectObjectsEvents and TickBroadcasts,
     * and sets up the necessary callbacks for processing data.
     */
    @Override
    protected void initialize() {

        subscribeBroadcast(TickBroadcast.class, tick -> {
            List<TrackedObject> trackedObjects = lidarWorkerTracker.getTrackedAtTick(tick.getTickCount());
            if(!trackedObjects.isEmpty()) {
                StatisticalFolder.getInstance().addTrackedObjects(trackedObjects.size());
                TrackedObjectsEvent trackedEvent = new TrackedObjectsEvent(trackedObjects);
                Errors.getInstance().addLiDarFrame(getName(), trackedObjects);
                sendEvent(trackedEvent);
            }
            if(lidarWorkerTracker.getStatus() == STATUS.DOWN){
                System.out.println("[LiDarService] terminating at time " + tick.getTickCount());
                terminate();
                sendBroadcast(new TerminatedBroadcast(getName()));
            }
        });

        subscribeEvent(DetectObjectsEvent.class, event -> {
            lidarWorkerTracker.addStampedObjects(event.getDetectedObjects(),event.getTick());
        });

        subscribeBroadcast(TerminatedBroadcast.class, terminate -> {
            if(terminate.getServiceName().equals("TimeService")){
                terminate();
                sendBroadcast(new TerminatedBroadcast(getName()));
                lidarWorkerTracker.setStatus(STATUS.DOWN);
            }
        });

        subscribeBroadcast(CrashedBroadcast.class, crashed -> {
            terminate();
        });
    }
}