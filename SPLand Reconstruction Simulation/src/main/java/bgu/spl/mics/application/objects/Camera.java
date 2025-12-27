package bgu.spl.mics.application.objects;

import java.util.ArrayList;
import java.util.List;



/**
 * Represents a camera sensor on the robot.
 * Responsible for detecting objects in the environment.
 */
public class Camera {
    private int id;                        
    private int frequency;                          
    private STATUS status;                            
    private String description;
    private List<StampedDetectedObjects> detectedObjectsList;
    private int numDetectedObject;


    /**
     * @PRE: `id` and `frequency` must be positive integers.
     * @PRE: `detectedObjectsList` must not be null and should contain valid `StampedDetectedObjects` objects.
     * @POST: Initializes the Camera object with the provided parameters, sets status to UP, description to an empty string, and numDetectedObject to 0.
     */
    public Camera(int id, int frequency, List<StampedDetectedObjects> detectedObjectsList) {
        this.id = id;
        this.frequency = frequency;
        this.status = STATUS.UP;
        this.description = "";
        this.detectedObjectsList = detectedObjectsList;
        this.numDetectedObject = 0;
    }

    /**
     * @POST: Returns the ID of the camera.
     */
    public int getId(){ 
        return id; 
    }

    /**
     * @POST: Returns the frequency of the camera.
     */
    public int getFrequency(){
        return frequency; 
    }

    /**
     * @POST: Returns the status of the camera.
     */
    public STATUS getStatus(){
        return status; 
    }

    /**
     * @POST: Returns the list of detected objects.
     */
    public List<StampedDetectedObjects> getDetectedObjectsList(){
        return detectedObjectsList; 
    }

    /**
     * @PRE: `frequency` must be a positive integer.
     * @POST: Sets the frequency of the camera to the provided value.
     */
    public void setFrequency(int frequency){
        this.frequency = frequency; 
    }

    /**
     * @PRE: `status` must be a valid `STATUS` value.
     * @POST: Sets the status of the camera to the provided value.
     */
    public void setStatus(STATUS status){
        this.status = status; 
    }

    /**
     * @PRE: `currentTime` must be a valid integer time greater than or equal to 0.
     * @POST: Returns a `StampedDetectedObjects` if any objects are detected at the current time, else returns null.
     * @POST: Updates the `numDetectedObject` counter and changes the camera status to DOWN if all objects are detected.
     * @POST: If an "ERROR" object is detected, the camera's status is set to ERROR and a crash description is set.
     */
    public StampedDetectedObjects detectObject(int currentTime) {
        StampedDetectedObjects detected = null;
        List<DetectedObject> objects = new ArrayList<>();      
        for (StampedDetectedObjects stampedObject : detectedObjectsList) {
            if (stampedObject.getTime() + frequency == currentTime) {
                detected = stampedObject;
                for (DetectedObject detectedObject : stampedObject.getDetectedObjects()) {
                    if (!detectedObject.getId().equals("ERROR")){
                         objects.add(detectedObject);
                    }
                }
            }
        }

        for (StampedDetectedObjects stampedObject : detectedObjectsList) {
            if (stampedObject.getTime() == currentTime) {
                for (DetectedObject detect : stampedObject.getDetectedObjects()) {
                    if (detect.getId().equals("ERROR")) {
                        description = detect.getDescription();
                        setStatus(STATUS.ERROR);
                        break;
                    }
                break;
                }
            }
        }
         
        if(detected != null) {
            numDetectedObject++;
        }
        if (detectedObjectsList.size() == numDetectedObject) {
            setStatus(STATUS.DOWN);
        }
        if (objects.isEmpty()) return null;
        return new StampedDetectedObjects(currentTime, objects);
    }

    /**
     * @PRE: `object` must not be null.
     * @POST: Adds the provided `StampedDetectedObjects` object to the list of detected objects.
     */
    public void addDetectedObject(StampedDetectedObjects object) {
        detectedObjectsList.add(object);
    }

    /**
     * @POST: Clears the list of detected objects.
     */
    public void clearDetectedObjects() {
        detectedObjectsList.clear();
    }

    /**
     * @POST: Returns the crash description if an "ERROR" object was detected.
     */
    public String getCrashDescription() {
        return description;
    }
}
