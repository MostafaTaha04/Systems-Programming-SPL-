Systems Programming – Assignment 2
SPLand Reconstruction Simulation (C++)

Student Information

Mostafa Taha – ID: 326524675 

1. Project Overview

This project implements the SPLand Reconstruction Simulation, as defined in Systems Programming – Assignment 2.
The system simulates post-war reconstruction of settlements by executing reconstruction plans, constructing facilities, and evaluating different selection policies over time.

The assignment extends Assignment 1 by emphasizing:

Correct memory management

Rule of Five

Deep copy semantics

Backup and restore functionality

Robust object-oriented design

The project is written in C++, strictly follows the provided skeleton and specifications, and was tested on Linux systems.

2. Program Execution Flow

The program receives the path to a configuration file as a command-line argument.

The configuration file initializes:

Settlements

Facility types

Initial reconstruction plans

The program prints:

The simulation has started


The simulation enters an interactive loop, waiting for user commands.

Each command is parsed, executed, and logged.

The simulation terminates when the close command is issued, printing final results.

3. Main Components
Simulation

Manages the entire simulation:

Settlements

Plans

Facility types

User actions

Backup and restore state

Settlement

Represents a settlement with a name and type:

Village – up to 1 facility under construction

City – up to 2 facilities under construction

Metropolis – up to 3 facilities under construction

Plan

Represents a reconstruction plan for a settlement:

Selection policy

Facilities under construction

Operational facilities

Accumulated scores (Life Quality, Economy, Environment)

FacilityType

Defines a facility type with:

Category (Life Quality / Economy / Environment)

Construction time

Score contributions

Facility

Represents a concrete facility instance assigned to a plan.

SelectionPolicy (Abstract)

Determines which facility type to construct next.
Implemented policies:

NaiveSelectionPolicy

BalancedSelectionPolicy

EconomySelectionPolicy

SustainabilitySelectionPolicy

BaseAction (Abstract)

Parent class for all user actions.
Supports:

Execution

Error handling

Logging

Polymorphic cloning

4. Supported User Commands

The following commands are supported exactly as defined in the assignment:

step <n> – Advance the simulation by n time units

plan <settlement_name> <policy> – Create a new reconstruction plan

settlement <name> <type> – Add a settlement

facility <name> <category> <price> <life> <eco> <env> – Add a facility type

planStatus <plan_id> – Print plan details

changePolicy <plan_id> <policy> – Change a plan’s selection policy

log – Print the actions log

backup – Save the current simulation state

restore – Restore the last saved simulation state

close – Print final results and terminate the simulation

5. Configuration File Format

The configuration file defines the initial state of the simulation and may include:

Settlements
settlement <name> <type>

Facility Types
facility <name> <category> <price> <life> <eco> <env>

Initial Plans
plan <settlement_name> <selection_policy>


The file is parsed sequentially, and invalid lines are handled according to the specification.

6. Memory Management and Rule of Five

The project strictly follows RAII principles

The Rule of Five is implemented where required

All dynamically allocated memory is properly released

Copy constructor, move constructor, copy assignment, move assignment, and destructor are correctly implemented

Backup and restore use deep copies

No memory leaks were detected using Valgrind

7. Build and Run Instructions
Prerequisites

Linux / Unix environment

C++11 compatible compiler

make

Build
make

Run
bin/simulation <path_to_config_file>


Example:

bin/simulation config.txt

8. Project Structure
src/
  main.cpp
  Simulation.cpp
  Settlement.cpp
  Plan.cpp
  Facility.cpp
  SelectionPolicy.cpp
  Action.cpp
  Auxiliary.cpp
include/
bin/
makefile
README.md

9. Testing

Tested with multiple configuration files

Output compared with provided examples

Compiled with warnings enabled

Passed Valgrind memory checks

Backup and restore tested extensively

Handles invalid input gracefully

10. Notes

Skeleton method signatures were not modified

All required classes and methods were fully implemented

No forbidden STL containers were used

The implementation strictly follows assignment constraints

11. Conclusion

This project demonstrates:

Correct application of object-oriented design in C++

Proper use of inheritance and polymorphism

Safe and efficient memory management

Full compliance with Assignment 2 requirements

The implementation is correct, robust, and ready for grading.