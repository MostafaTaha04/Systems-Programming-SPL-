BGU Mart – Supermarket Management System
Python & SQLite Assignment

Student Information

Mostafa Taha – ID: 326524675 

1. General Description

This project implements BGU Mart, a supermarket chain management system.
The system supports managing employees, suppliers, products, branches, and all business activities including sales and supply deliveries.

The software maintains a centralized SQLite database and logs all activities for inventory management and tax reporting purposes.

The project is implemented using Python 3 and SQLite3, and follows the exact specifications provided in the assignment instructions.

2. System Architecture

The system consists of:

A single SQLite database: bgumart.db

Three mandatory Python modules:

initiate.py

action.py

printdb.py

Each module is executable as a standalone program and is invoked from the command line.

3. Database Design

The database contains five tables:

Employees

Stores employee details.

employees(id, name, salary, branche)

Suppliers

Stores supplier details.

suppliers(id, name, contact_information)

Products

Stores product inventory information.

products(id, description, price, quantity)

Branches

Stores branch information.

branches(id, location, number_of_employees)

Activities

Logs all sales and supply arrivals.

activities(product_id, quantity, activator_id, date)


Sales are represented by negative quantities

Supply arrivals are represented by positive quantities

4. Modules Description
4.1 initiate.py

This module initializes the database.

Responsibilities:

Deletes bgumart.db if it already exists

Creates all database tables

Parses the configuration file

Inserts initial data into the database

Usage:

python3 initiate.py config.txt


You may assume:

The configuration file exists

The data format is valid

4.2 action.py

This module processes sales and supply activities.

Responsibilities:

Reads an actions file line by line

Executes each activity in order

Updates product quantities

Inserts activity records into the database

Rules:

If a sale quantity exceeds available stock → do nothing

Partial sales are not allowed

No error messages should be printed

Usage:

python3 action.py actions.txt

4.3 printdb.py

This module prints the database contents and reports.

Responsibilities:

Print all database tables:

Activities (ordered by date)

Branches

Employees

Products

Suppliers

Print Employees Report:

Name Salary Location TotalSalesIncome


Ordered by employee name (ascending)

Print Activities Report:

date description quantity seller supplier


Ordered from oldest to newest

Seller is None for supplies

Supplier is None for sales

Implemented using SQL JOIN queries only

Usage:

python3 printdb.py

5. Configuration File Format

Each line represents a single record:

Branch:

B,id,location,number_of_employees


Employee:

E,id,name,salary,branch_id


Product:

P,id,description,price,quantity


Supplier:

S,id,name,contact_information


Notes:

Record type letter (B, E, P, S) is not inserted into the database

Employee IDs and Supplier IDs are unique across both tables

6. Action File Format

Each line represents a single activity:

product_id, quantity, activator_id, date


Rules:

quantity > 0 → Supply arrival

quantity < 0 → Sale

quantity = 0 → Illegal (ignored)

If stock is insufficient for a sale, the action is ignored.

7. Output Requirements

Output must use Python’s default print() behavior

No custom formatting

Database tables printed in ascending primary key order

Activities table ordered by date

Reports printed exactly as specified

8. Build & Run Instructions
Prerequisites

Python 3.9 or higher

SQLite3 (bundled with Python)

Run Order Example
python3 initiate.py config.txt
python3 printdb.py
python3 action.py actions.txt
python3 printdb.py

9. Development Notes

Implemented using Python standard libraries only

SQL queries use SELECT, JOIN, ORDER BY

No partial transactions

Logic strictly follows assignment rules

Designed to work correctly when executed from terminal

10. Conclusion

This project implements a complete supermarket management system using Python and SQLite.
It correctly manages inventory, employees, suppliers, branches, and activities while producing accurate reports and maintaining data integrity.

The implementation strictly follows the assignment specification and is ready for automatic testing and grading.