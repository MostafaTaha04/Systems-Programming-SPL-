Systems Programming Lab (SPL) – Assignment 3
Multithreaded SPLand Reconstruction Simulation (C++)

Student Information

Mostafa Taha – ID: 326524675 

1. Project Overview

This project implements Assignment 3 of the Systems Programming Lab (SPL) course.
The assignment extends the SPLand Reconstruction Simulation from previous assignments by introducing multithreading, synchronization, and concurrency control.

The simulation models post-war reconstruction of settlements while supporting concurrent execution of reconstruction plans and actions. The system is designed to be thread-safe, scalable, and compliant with all assignment constraints.

The project is written in C++, uses the C++11 threading library, and strictly follows the provided skeleton and instructions.

2. Main Goals of Assignment 3

Convert the simulation into a multithreaded system

Support parallel execution of reconstruction plans

Ensure thread safety and correct synchronization

Prevent race conditions and deadlocks

Preserve correctness of simulation logic from Assignments 1 and 2

Maintain proper memory management and object ownership

3. Multithreaded Design
Worker Threads

Each reconstruction plan may be executed concurrently

Threads handle facility construction progress and plan updates

Threads run independently but synchronize on shared resources

Synchronization Mechanisms

Mutexes are used to protect shared data structures

Condition variables are used when required for coordination

Access to settlements, plans, logs, and backup state is synchronized

Thread Safety Guarantees

No data races

No deadlocks

Deterministic behavior under concurrent execution

Safe shutdown when the simulation ends

4. Core Components

Simulation – Central controller of the simulation and thread lifecycle

Settlement – Thread-safe representation of a settlement

Plan – Executes reconstruction logic, potentially in parallel

Facility / FacilityType – Construction units handled by plans

SelectionPolicy – Determines facility selection (unchanged logic, thread-safe access)

BaseAction – User actions executed in a synchronized manner

All components from Assignments 1 and 2 were updated where necessary to support concurrency.

5. Supported User Commands

All commands from previous assignments are supported and remain unchanged:

step <n>

plan <settlement_name> <policy>

settlement <name> <type>

facility <name> <category> <price> <life> <eco> <env>

planStatus <plan_id>

changePolicy <plan_id> <policy>

log

backup

restore

close

Command execution is synchronized to ensure correctness in a multithreaded environment.

6. Configuration File

The configuration file format remains unchanged and includes:

Settlements

settlement <name> <type>


Facility Types

facility <name> <category> <price> <life> <eco> <env>


Initial Plans

plan <settlement_name> <selection_policy>


The configuration file is parsed sequentially at startup.

7. Thread Lifecycle Management

Threads are created when plans are activated

Threads are joined or terminated cleanly when the simulation ends

No detached threads remain after program termination

Backup and restore operations are synchronized with running threads

8. Memory Management

The project follows RAII principles

Dynamic memory is managed safely

The Rule of Five is preserved where required

Backup and restore operations use deep copies

No memory leaks were detected (validated using Valgrind)

9. Build and Run Instructions
Prerequisites

Linux / Unix environment

C++11 compatible compiler

make

pthread support

Build
make

Run
bin/simulation <path_to_config_file>


Example:

bin/simulation config.txt

10. Project Structure
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

11. Testing and Validation

Tested with multiple configuration files

Stress-tested using multiple concurrent plans

Verified correct synchronization under load

Compiled with warnings enabled

Passed Valgrind memory checks

No race conditions observed

12. Notes

Skeleton method signatures were not modified

No forbidden libraries were used

Synchronization was added only where required

Logic from previous assignments was preserved

13. Conclusion

This project successfully extends the SPLand Reconstruction Simulation into a robust multithreaded system.
It demonstrates correct use of threads, mutexes, and synchronization, while maintaining clean object-oriented design and safe memory management.

The implementation is fully compliant with SPL Assignment 3 and ready for evaluation.