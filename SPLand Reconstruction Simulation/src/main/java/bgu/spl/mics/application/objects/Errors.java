package bgu.spl.mics.application.objects;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Errors {
    private static class ErrorFolderHolder {
        private static Errors instance = new Errors();
    }

    private Map<String, StampedDetectedObjects> lastCamerasFrame;
    private Map<String, List<TrackedObject>> lastLiDarWorkerTrackersFrame;
    private List<Pose> poses;
    private StatisticalFolder statistics;
    private String error;
    private String faultySensor;

    public static Errors getInstance(){
        return Errors.ErrorFolderHolder.instance;
    }
    private Errors(){
        this.error = "";
        this.faultySensor = "";
        this.lastCamerasFrame = new ConcurrentHashMap<>();
        this.lastLiDarWorkerTrackersFrame = new ConcurrentHashMap<>();
        this.poses = new ArrayList<>();
        this.statistics = StatisticalFolder.getInstance();
    }

    public void setError(String error) {
        this.error = error;
    }

    public void FaultSensor(String faultySensor) {
        this.faultySensor = faultySensor;
    }

    public synchronized void addCameraFrame(String name, StampedDetectedObjects objects){
        lastCamerasFrame.put(name, objects);
    }

    public synchronized void addLiDarFrame(String name, List<TrackedObject> objects){
        lastLiDarWorkerTrackersFrame.put(name, objects);
    }

    public void addPose(Pose p){
        poses.add(p);
    }
}
