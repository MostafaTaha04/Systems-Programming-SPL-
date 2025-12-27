package bgu.spl.mics.application.objects;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.LinkedList;

import static org.junit.jupiter.api.Assertions.*;

class FusionSlamTest {


    @BeforeEach
    public void setUp() {
        FusionSlam.getInstance().reset();
    }

    @Test
    public void testAddNewLandMark() {

        FusionSlam fusionSlam = FusionSlam.getInstance();
        StatisticalFolder stats = StatisticalFolder.getInstance();
        stats.resetStatistics();

        LinkedList<CloudPoint> coordinates = new LinkedList<>();
        coordinates.add(new CloudPoint(1.0, 2.0));
        LandMark newMark = new LandMark("L1","Description1", coordinates);

        fusionSlam.addLandmark(newMark);

        assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should be 1");
        LandMark addedMark = fusionSlam.getLandmarks().getFirst();
        assertEquals("L1", addedMark.getId(), "Landmark ID should match");
        assertEquals(1.0, addedMark.getCoordinates().get(0).getX(), "Coordinate X should match");
        assertEquals(2.0, addedMark.getCoordinates().get(0).getY(), "Coordinate Y should match");
    }


    @Test
    public void testUpdateExistingLandMark() {

        FusionSlam fusionSlam = FusionSlam.getInstance();
        StatisticalFolder stats = StatisticalFolder.getInstance();
        stats.resetStatistics();

        LinkedList<CloudPoint> existingCoordinates = new LinkedList<>();
        existingCoordinates.add(new CloudPoint(1.0, 2.0));
        LandMark existingMark = new LandMark("L1","Description", existingCoordinates);
        fusionSlam.addLandmark(existingMark);

        LinkedList<CloudPoint> newCoordinates = new LinkedList<>();
        newCoordinates.add(new CloudPoint(3.0, 4.0));
        LandMark newMark = new LandMark("L1","Description",newCoordinates);

        fusionSlam.addLandmark(newMark);

        assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should remain 1");
        LandMark updatedMark = fusionSlam.getLandmarks().getFirst();
        assertEquals("L1", updatedMark.getId(), "Landmark ID should match");
        assertEquals(2.0, updatedMark.getCoordinates().get(0).getX(), "Updated coordinate X should match the average");
        assertEquals(3.0, updatedMark.getCoordinates().get(0).getY(), "Updated coordinate Y should match the average");
    }

    /**
     * Test case 3: Add a landmark with no coordinates.
     */
    @Test
    public void testAddLandMarkNoCoordinates() {

        FusionSlam fusionSlam = FusionSlam.getInstance();
        StatisticalFolder stats = StatisticalFolder.getInstance();
        stats.resetStatistics();

        LinkedList<CloudPoint> coordinates = new LinkedList<>();
        LandMark newMark = new LandMark("L2","Description", coordinates);


        fusionSlam.addLandmark(newMark);


        assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should be 1");
        LandMark addedMark = fusionSlam.getLandmarks().getFirst();
        assertEquals("L2", addedMark.getId(), "Landmark ID should match");
        assertTrue(addedMark.getCoordinates().isEmpty(), "Coordinates list should be empty");
    }

}




//////////////////////////////
// package bgu.spl.mics.application.objects;

// import org.junit.jupiter.api.BeforeEach;
// import org.junit.jupiter.api.Test;

// import java.util.LinkedList;
// import java.util.List;

// import static org.junit.jupiter.api.Assertions.*;

// class FusionSlamTest {

//     @BeforeEach
//     public void setUp() {
//         FusionSlam.getInstance().reset(); // Reset the FusionSlam instance before each test
//     }

//     @Test
//     public void testAddNewLandMark() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         LinkedList<CloudPoint> coordinates = new LinkedList<>();
//         coordinates.add(new CloudPoint(1.0, 2.0));
//         LandMark newLandMark = new LandMark("L1", "Landmark 1", coordinates);

//         boolean added = fusionSlam.addLandmark(newLandMark);

//         assertTrue(added, "New landmark should be added successfully");
//         assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should be 1");
//         LandMark addedLandMark = fusionSlam.getLandmarks().get(0);
//         assertEquals("L1", addedLandMark.getId(), "Landmark ID should match");
//         assertEquals(1.0, addedLandMark.getCoordinates().get(0).getX(), "Coordinate X should match");
//         assertEquals(2.0, addedLandMark.getCoordinates().get(0).getY(), "Coordinate Y should match");
//     }

//     @Test
//     public void testUpdateExistingLandMark() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         LinkedList<CloudPoint> existingCoordinates = new LinkedList<>();
//         existingCoordinates.add(new CloudPoint(1.0, 2.0));
//         LandMark existingLandMark = new LandMark("L1", "Existing Landmark", existingCoordinates);
//         fusionSlam.addLandmark(existingLandMark);

//         LinkedList<CloudPoint> newCoordinates = new LinkedList<>();
//         newCoordinates.add(new CloudPoint(3.0, 4.0));
//         LandMark newLandMark = new LandMark("L1", "Updated Landmark", newCoordinates);

//         boolean updated = fusionSlam.addLandmark(newLandMark);

//         assertFalse(updated, "Updating an existing landmark should not return 'true'");
//         assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should remain 1");
//         LandMark updatedLandMark = fusionSlam.getLandmarks().get(0);
//         assertEquals("L1", updatedLandMark.getId(), "Landmark ID should match");
//         assertEquals(2.0, updatedLandMark.getCoordinates().get(0).getX(), "Updated coordinate X should match the average");
//         assertEquals(3.0, updatedLandMark.getCoordinates().get(0).getY(), "Updated coordinate Y should match the average");
//     }

//     @Test
//     public void testAddLandMarkNoCoordinates() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         LinkedList<CloudPoint> emptyCoordinates = new LinkedList<>();
//         LandMark emptyLandMark = new LandMark("L2", "No Coordinates Landmark", emptyCoordinates);

//         boolean added = fusionSlam.addLandmark(emptyLandMark);

//         assertTrue(added, "Landmark with no coordinates should be added successfully");
//         assertEquals(1, fusionSlam.getLandmarks().size(), "Landmarks list size should be 1");
//         LandMark addedLandMark = fusionSlam.getLandmarks().get(0);
//         assertEquals("L2", addedLandMark.getId(), "Landmark ID should match");
//         assertTrue(addedLandMark.getCoordinates().isEmpty(), "Coordinates list should be empty");
//     }

//     @Test
//     public void testWorkOnTrackedObject() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         LinkedList<CloudPoint> coordinates = new LinkedList<>();
//         coordinates.add(new CloudPoint(1.0, 1.0));
//         TrackedObject trackedObject = new TrackedObject("T1", "Tracked Object", coordinates, 1);
//         fusionSlam.addTrackedObjects(List.of(trackedObject));

//         Pose pose = new Pose(1, 2.0, 3.0, 45.0); // Example pose
//         fusionSlam.addPose(pose);

//         fusionSlam.WorkOnTrackedObject();

//         assertEquals(1, fusionSlam.getLandmarks().size(), "One tracked object should be converted to a landmark");
//         LandMark newLandMark = fusionSlam.getLandmarks().get(0);
//         assertEquals("T1", newLandMark.getId(), "Tracked object ID should match landmark ID");
//     }

//     @Test
//     public void testGetPoseAtTime() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         Pose pose1 = new Pose(1, 1.0, 2.0, 30.0);
//         Pose pose2 = new Pose(2, 2.0, 3.0, 60.0);
//         fusionSlam.addPose(pose1);
//         fusionSlam.addPose(pose2);

//         Pose result = fusionSlam.getPoseAtTime(2);

//         assertNotNull(result, "Pose at time 2 should not be null");
//         assertEquals(2.0, result.getX(), "Pose X coordinate should match");
//         assertEquals(3.0, result.getY(), "Pose Y coordinate should match");
//         assertEquals(60.0, result.getYaw(), "Pose yaw should match");
//     }

//     @Test
//     public void testReset() {
//         FusionSlam fusionSlam = FusionSlam.getInstance();

//         Pose pose = new Pose(1, 1.0, 2.0, 30.0);
//         LandMark landMark = new LandMark("L1", "Landmark", new LinkedList<>());

//         fusionSlam.addPose(pose);
//         fusionSlam.addLandmark(landMark);

//         fusionSlam.reset();

//         assertTrue(fusionSlam.getPoses().isEmpty(), "Poses list should be empty after reset");
//         assertTrue(fusionSlam.getLandmarks().isEmpty(), "Landmarks list should be empty after reset");
//     }
// }
