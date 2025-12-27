Systems Programming – Assignment 1
Reconstruction Simulation (C++)

Student Information

Mostafa Taha – ID: 326524675


1. Project Overview

This project implements a reconstruction simulation system for settlements in the fictional land of SPLand, as defined in Systems Programming – Assignment 1.
The simulation models post-war reconstruction by executing different reconstruction plans for settlements using various selection policies.

The system is implemented in C++, follows object-oriented design principles, and strictly adheres to the provided assignment specification and skeleton code.

2. Program Flow

The program receives a configuration file path as a command-line argument.

The configuration file initializes settlements, facility types, and initial reconstruction plans.

The program prints:
The simulation has started

The simulation enters an interactive command loop.

User actions are executed sequentially and logged.

The simulation terminates upon receiving the close command, printing final results.

3. Main Components

Simulation – Manages the overall system, including settlements, plans, facilities, actions, and backup/restore functionality.

Settlement – Represents a settlement (Village, City, or Metropolis), each with a different capacity for concurrent construction.

Plan – Represents a reconstruction plan, including its selection policy, construction progress, operational facilities, and accumulated scores.

FacilityType – Defines a type of facility, including category, construction time, and contribution scores.

Facility – Represents an instance of a facility assigned to a plan.

SelectionPolicy – Abstract class that determines which facility type to construct next. Implemented policies include Naive, Balanced, Economy-focused, and Sustainability-focused.

BaseAction – Abstract class for all user actions, supporting execution, error handling, and logging.

4. Supported User Commands

The simulation supports the following commands:

step <n> – Advance the simulation by n time units

plan <settlement_name> <policy> – Create a new reconstruction plan

settlement <name> <type> – Add a new settlement

facility <name> <category> <price> <life> <eco> <env> – Add a facility type

planStatus <plan_id> – Display plan details

changePolicy <plan_id> <policy> – Change a plan’s selection policy

log – Print the action log

backup – Save the current simulation state

restore – Restore the last saved state

close – Print final simulation results and terminate

All commands behave exactly as defined in the assignment specification.

5. Configuration File

The configuration file defines the initial state of the simulation and may contain:

Settlements:
settlement <name> <type>

Facility types:
facility <name> <category> <price> <life> <eco> <env>

Initial plans:
plan <settlement_name> <selection_policy>

The file is parsed sequentially, and invalid lines are handled according to the specification.

6. Memory Management

The project follows RAII principles

Dynamic memory is managed safely

The Rule of Five is implemented where required

Backup and restore use deep copies

The program was tested using Valgrind

No memory leaks were detected

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

Compared output with provided examples

Compiled with warnings enabled

Passed Valgrind memory checks

Handles invalid input gracefully

10. Notes

Skeleton method signatures were not modified

All required classes and methods were fully implemented

No forbidden STL containers were used

The solution fully complies with assignment constraints

11. Conclusion

This project demonstrates correct use of C++ object-oriented programming, inheritance and polymorphism, and safe memory management.
The implementation is robust, efficient, and fully compliant with the assignment requirements.