package bgu.spl.mics.application.objects;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Manages the fusion of sensor data for simultaneous localization and mapping (SLAM).
 * Combines data from multiple sensors (e.g., LiDAR, camera) to build and update a global map.
 * Implements the Singleton pattern to ensure a single instance of FusionSlam exists.
 */
public class FusionSlam {

    // Singleton instance holder
    private static class FusionSlamHolder {
        private static final FusionSlam instance = new FusionSlam();
    }

    // Global data storage
    private List<LandMark> landmarks;
    private List<Pose> poses;         
    private int NumberOfSensors;
    private List<TrackedObject> trackedObjects;
    private boolean crashed;
    /**
     * Private constructor for singleton pattern.
     */
    /**
     * Private constructor for singleton pattern.
     * @PRE: None.
     * @POST: Initializes the `landmarks`, `poses`, `trackedObjects` lists, and sets `crashed` to false.
     */
    private FusionSlam() {
        this.landmarks = new ArrayList<>();
        this.poses = new ArrayList<>();
        this.trackedObjects = new ArrayList<>();
        this.crashed = false;
    }

    /**
     * @POST: Resets the FusionSlam instance by clearing `landmarks`, `poses`, and `trackedObjects`, 
     * and setting `crashed` to false.
     */
    public void reset() {
        this.landmarks = new ArrayList<>();
        this.poses = new ArrayList<>();
        this.trackedObjects = new ArrayList<>();
        this.crashed = false;
    }

    /**
     * @POST: Returns the `crashed` status.
     */
    public boolean crashed() {
        return crashed;
    }

    /**
     * @PRE: `crashed` must be a boolean value (true or false).
     * @POST: Sets the `crashed` status to the provided value.
     */
    public void isCrashed(boolean crashed) {
        this.crashed = crashed;
    }

    /**
     * Provides access to the singleton instance of FusionSlam.
     *
     * @return The single instance of FusionSlam.
     */
    /**
     * @POST: Provides access to the singleton instance of FusionSlam.
     */
    public static FusionSlam getInstance() {
        return FusionSlamHolder.instance;
    }

    /**
     * @POST: Processes each tracked object and attempts to create a new landmark if applicable.
     * @POST: If a new landmark is added, it increments the `StatisticalFolder` landmark counter.
     */
    public void WorkOnTrackedObject(){
        List<TrackedObject> trackedList = new ArrayList<>(trackedObjects);
        trackedObjects.clear();
        for(TrackedObject object : trackedList){
            Pose p = getPoseAtTime(object.getTime());
            if(p == null) {
                trackedObjects.add(object);
                continue;
            }
            LandMark newlandMark = new LandMark(object.getId(),object.getDescription(), changeCoordinates(object.getCoordinates(), p));
            if (addLandmark(newlandMark)) {
                StatisticalFolder.getInstance().incrementLandmarks();
            }
        }
    }

    /**
     * @PRE: `coordinates` and `p` must be valid (non-null) values.
     * @POST: Returns transformed coordinates based on the provided pose.
     */
    private List<CloudPoint> changeCoordinates(List<CloudPoint> coordinates, Pose p) {
        List<CloudPoint> newCoords = new ArrayList<>();
        double cosYaw = Math.cos(Math.toRadians(p.getYaw()));
        double sinYaw = Math.sin(Math.toRadians(p.getYaw()));

        for (CloudPoint point : coordinates) {
            double transformedX = cosYaw * point.getX() - sinYaw * point.getY() + p.getX();
            double transformedY = sinYaw * point.getX() + cosYaw * point.getY() + p.getY();
            newCoords.add(new CloudPoint(transformedX, transformedY));
        }
        return newCoords;
    }

    /**
     * @PRE: `time` must be a valid integer representing the timestamp.
     * @POST: Returns the Pose corresponding to the provided time, or null if not found.
     */
    private Pose getPoseAtTime(int time) {
        for(Pose p : poses){
            if(p.getTime() == time){
                return p;
            }
        }
        return null;
    }

    /**
     * Adds a landmark to the global map.
     *
     * @param object The tracked object to be added as a landmark.
     */
    /**
     * @PRE: `object` must not be null.
     * @POST: Adds the landmark if not already present. If the landmark already exists, 
     * it averages the coordinates with the existing one.
     * @POST: Returns true if the landmark was added, false otherwise.
     */
    public synchronized Boolean addLandmark(LandMark object) {
        if (existingLandMark(object.getId()) != null) {
            LandMark oldLandMark = existingLandMark(object.getId());
            LinkedList<CloudPoint> resultCoordinates = new LinkedList<>();

            for (int i = 0; i < object.getCoordinates().size() && i < oldLandMark.getCoordinates().size(); i++) {
                double x;
                double y;

                x = (object.getCoordinates().get(i).getX() + oldLandMark.getCoordinates().get(i).getX()) / 2;
                y = (object.getCoordinates().get(i).getY() + oldLandMark.getCoordinates().get(i).getY()) / 2;

                CloudPoint point = new CloudPoint(x, y);
                resultCoordinates.add(point);
            }
            oldLandMark.setCoordinates(resultCoordinates);
        } else {
            landmarks.add(object);
            return true;
        }
        return false;
    }


    /**
     * Returns the list of landmarks.
     *
     * @return List of tracked objects representing landmarks.
     */
    /**
     * @POST: Returns the list of landmarks.
     */
    public List<LandMark> getLandmarks() {
        return landmarks;
    }

    /**
     * Adds a pose to the list of robot poses.
     *
     * @param pose The robot's pose to be added.
     */
    /**
     * @PRE: `pose` must not be null.
     * @POST: Adds a pose to the list of poses.
     */
    public synchronized void addPose(Pose pose) {
        poses.add(pose);
    }

    /**
     * Returns the list of poses.
     *
     * @return List of poses.
     */
    /**
     * @POST: Returns the list of poses.
     */
    public synchronized List<Pose> getPoses() {
        return poses;
    }

    /**
     * Clears all stored landmarks and poses.
     */
    /**
     * @POST: Clears all stored landmarks and poses.
     */
    public void clear() {
        landmarks.clear();
        poses.clear();
    }

    /**
     * @PRE: `id` must be a valid string identifier.
     * @POST: Returns the landmark with the matching ID, or null if not found.
     */
    public synchronized LandMark existingLandMark(String id) {
        for (LandMark landMark : landmarks) {
            if (landMark.getId().equals(id)) {
                return landMark;
            }
        }
        return null;
    }

    /**
     * @POST: Decreases the number of sensors.
     */
    public void decrementNumberOfSensors() {
        NumberOfSensors -= 1;
    }

    /**
     * @POST: Returns the current number of sensors.
     */
    public int getNumberOfSensors() {
        return NumberOfSensors;
    }

    /**
     * @PRE: `numberOfSensors` must be a positive integer.
     * @POST: Sets the number of sensors to the provided value.
     */
    public void setNumberOfSensors(int numberOfSensors) {
        this.NumberOfSensors = numberOfSensors;
    }

    /**
     * @PRE: `trackedObjects` must be a non-null list.
     * @POST: Adds the list of tracked objects to the existing list of tracked objects.
     */
    public void addTrackedObjects(List<TrackedObject> trackedObjects) {
        this.trackedObjects.addAll(trackedObjects);
    }

}
