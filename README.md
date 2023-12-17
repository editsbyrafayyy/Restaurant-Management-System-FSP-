Restaurant Management System Code (Manager's Usage)
This code is command-line interface tool for managing both employees and the menu of a restaurant. It uses text files for data storage and provides various functionalities through a menu-driven interface.

Employee Management (The First Part of The Program is To Be Used by The Manager. The Second Part
Of The Code Is For The Order Takers and Drive-Thru Employees To Use - Two Seperate Functioning Codes)

Data Storage: Employee information is stored in the "employee.txt" file, formatted with each line representing an employee's details separated by commas (name, position, contact details, salary, ratings).


Header and Includes:
    The code includes necessary header files (iostream, fstream, sstream, string, vector, ctime) and disables warnings using _CRT_SECURE_NO_WARNINGS.

Global Constants:
    Two global constants are defined to store the filenames for the menu (hardees.txt) and employee data (employee.txt).

Function Declarations:
    Three functions are declared: managemenu(), manageemployees(), and displayoptions(int a).

Employee Class:
    Defines a class named employee representing an employee in the restaurant.
    Contains private member variables for the employee's name, position, contact information, salary, total rating, and orders completed.
    Provides methods to get and set employee information, display employee details, and serialize/deserialize employee data for file I/O.

File I/O Functions for Employees:
    saveemployeestofile: Saves employee data to a file.
    loademployeesfromfile: Loads employee data from a file.

MenuItem Class:
    Defines a class named menuitem representing an item on the restaurant menu.
    Contains private member variables for the item's name, price, description, availability, and category.
    Provides methods to get and set menu item information, calculate a discount, and serialize/deserialize menu item data for file I/O.

File I/O Functions for Menu Items:
    savemenuitemstofile: Saves menu items to a file.
    loadmenuitemsfromfile: Loads menu items from a file.

Main Function:
    Declares two global vectors, loadedmenu and employees, to store menu items and employee data, respectively.
    Loads menu items and employee data from their respective files.
    Displays a welcome message and enters a loop for the main menu options.

Display Options Function:
    Displays menu options based on the input parameter a.

Manage Menu Function (managemenu()):
    Allows the user to view the current menu, add new items, remove items, modify item details, and exit the menu management.
    Handles file I/O for menu items.

Manage Employees Function (manageemployees()):
    Allows the user to list all employees, hire new employees, mark attendance, check attendance, fire employees, change employee information, and exit employee management.
    Handles file I/O for employee data.

Further Explanation:

Operations:

    Hiring: User enters new employee information, which gets appended to the "employee.txt" file.
    Attendance:
    Marking: User enters employee ID and date, marking the employee present for that day in the "attendance.txt" file.
    Checking: User enters date to view a list of present employees for that day, filtered from the "attendance.txt" file.
    Modifications: User enters employee ID and updates any detail, which modifies the corresponding line in "employee.txt".

Menu Management:

    Data Storage: Menu items are stored in the "hardees.txt" file, similar to employees, with each line representing an item's details separated by commas (name, price, description, availability, category).
    Operations:
    Adding: User enters new item details, which gets appended to the "hardees.txt" file.
    Removal: User enters item name, and the corresponding line gets deleted from "hardees.txt".
    Modification: User enters item name and updates any detail, modifying the corresponding line in "hardees.txt".
    Listing: User displays the entire menu by reading and formatting data from "hardees.txt".


===========================================================================================================================

Second Part (The Employee Part)

Functionality:
Order Creation:
    The program allows users to create a new order by choosing items from a menu.
    It prompts the user to enter customer details, choose items from the menu, and specifies if it's for dine-in (with a table assignment) or not.
    Order Cancellation:

    Users can cancel an existing order by providing the order ID.
    The canceled order is removed from the system, and the changes are updated in the "orders.txt" file.
    Order Search:

    Users can search for orders based on keywords such as customer name, items ordered, or total cost.

Display Menu:
    The program reads menu items from the "hardees.txt" file and displays them, showing details such as name, category, price, description, and availability.
    Feedback System:

    After order creation, the program prompts users to provide feedback on the waiter's name, speed of service, waiter's attitude, and restaurant cleanliness. This feedback is stored in "customer_feedback.txt".
    Data Storage:

    Order information is stored in the "orders.txt" file.
    Customer feedback is stored in the "customer_feedback.txt" file.

Libraries Used:

    iostream: Input and output operations.
    fstream: File stream operations for reading and writing to files.
    sstream: String stream for parsing and formatting strings.
    vector: Dynamic array for storing menu items and orders.
    algorithm: Used for searching and manipulating vectors.
    string: String manipulation operations.

How to Use

Menu Data:
    Menu items are loaded from the "hardees.txt" file using the loadMenuItemsFromFile function.

Order Data:
    Order information is read from and written to the "orders.txt" file.
    Order cancellation updates the "orders.txt" file.

Feedback Data:
    Customer feedback is stored in the "customer_feedback.txt" file.

Main Menu:
    The main menu is displayed, providing options to create an order, cancel an order, search orders, display the menu, and exit.

Order Creation:
    Users are prompted to enter order details, choose items, and provide feedback.

Order Cancellation:
    Users enter the order ID to cancel an order.

Order Search:
    Users provide a keyword, and the program searches and displays matching orders.

Display Menu:
    Menu items are displayed with details.

Exit Program:
    Option 5 exits the program.
