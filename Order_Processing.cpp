// I have used PF wali libraries, so code thora restricted but it is what it is, things that are yet to be changed:
// combining per se the programs (unmein compatibility add karna somehow), either removing the invoice file cause
// bruhhh phir usko bhi read aur write karna hoga mujhay nahi pata kaisay hoga, file mein jo tables save hotay
// wo thoray ajeeb hein, havent figured that part completely. so yeye have commented most parts gl

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class MenuItem
{
private:
  string name;
  double price;
  string description;
  bool availaiblity;
  string category;

public:
  MenuItem() : name(""), price(0.0), description(""), availaiblity(false), category("") {}

  void setName(string itemName)
  {
    name = itemName;
  }

  void setPrice(double itemprice)
  {
    price = itemprice;
  }

  void setDescription(string desc)
  {
    description = desc;
  }

  void setCategory(string cat)
  {
    category = cat;
  }

  void setAvailaibilty(bool aval)
  {
    availaiblity = aval;
  }

  string getName() const
  {
    return name;
  }

  string getCategory() const
  {
    return category;
  }

  double getPrice() const
  {
    return price;
  }

  bool getAvailaibilty() const
  {
    return availaiblity;
  }

  string getDescription() const
  {
    return description;
  }

  string serialize() const
  {
    return name + "*" + to_string(price) + "*" + description + "*" + (availaiblity ? "1" : "0") + "*" + category;
  }

  void deserialize(const string serializedString)
  {
    istringstream iss(serializedString);
    getline(iss, name, '*');
    iss >> price;
    iss.ignore();
    getline(iss, description, '*');

    string availabilityStr;
    getline(iss, availabilityStr, '*');
    availaiblity = (availabilityStr == "1");

    getline(iss, category, '*');
  }
};

struct Orders
{ // the 1st structure
    int orderId;
    string customer_name;
    string items_ordered;
    int quantity;
    int total_cost;
    int assignedTableId; // stores the ID of the table assigned to the order
};

vector<MenuItem> loadedMenu; // declaring a global array
vector<pair<int, int>> orderedItems;
const int max_orders = 20;   // max orders
const int max_tables = 10;   // max tables
const int max_items = 30;    // Adjust as needed

Orders orders[max_orders]; // declaring a new var with the properties of Orders structure
int order_nums = 0;        // number of orders

MenuItem stores[max_items]; // Array to store menu items
int itemCount = 0;          // Track item count

void CustomerSatisfaction(); // calling da function

bool isValidOrderId(int inputId, const vector<Orders>& orders) {
  return find_if(orders.begin(), orders.end(),
                 [inputId](const Orders& order) { return order.orderId == inputId; }) == orders.end();
}

void CreateOrder(vector<Orders> &orders, int &order_nums){
  bool foundDuplicate = false;

  int table_picked;
  const int max_tables = 20;
  bool tableAvailable[max_tables];

  for (int i = 0; i < max_tables; i++)
  {
    tableAvailable[i] = true;
  }

  ofstream outputFile("orders.txt", ios::app);
  if (!outputFile.is_open())
  {
    cout << "Error opening file." << endl;
    return;
  }

  if (order_nums >= max_orders)
  {
    cout << "Max Orders Exceeded" << endl;
    return;
  }

  Orders new_order; // creating a var with the properties of Orders structure
  new_order.orderId = order_nums + 1;
  order_nums++;

  bool dine_in = false;
  int nCount = 0; // Track consecutive n entries

  do
  {
    cout << "Do You Want To Dine in (y/n)?: ";
    char choice;
    cin >> choice;

    if (!cin)
    {
      cin.clear();
      cin.ignore();
      cout << "Invalid input. Please enter y or n." << endl;
      continue;
    }

    if (choice == 'n' || choice == 'N')
    {
      break;
    }

    dine_in = (choice == 'y' || choice == 'Y');
  } while (!dine_in && !cin.fail());

  if (dine_in)
  {
    cout << "Available Tables:" << endl;
    for (int i = 0; i < max_tables; i++)
    {
      if (tableAvailable[i])
      {
        cout << " - " << (i + 1) << endl;
      }
    }

    do
    {
      cout << "Choose a table number (or 0 to cancel): ";
      cin >> table_picked;

      if (table_picked == 0)
      {
        cout << "Order canceled." << endl;
        return;
      }

      if (table_picked < 1 || table_picked > max_tables)
      {
        cout << "Invalid table number. Please choose a valid number or 0 to cancel." << endl;
      }
      else if (!tableAvailable[table_picked - 1])
      {
        cout << "Table " << table_picked << " is unavailable. Please choose another." << endl;
      }
      else
      {
        tableAvailable[table_picked - 1] = false; // Mark the table as unavailable
        cout << "\nTable " << table_picked << " has been assigned to your order." << endl;
        break;
      }
    } while (true);
  }
  // ==================================================

  do {
    cout << "Enter Order ID: ";
    int inputId;
    while (!(cin >> inputId) || !isValidOrderId(inputId, orders)) {
      cout << "Invalid input. Please enter a unique ID: ";
      cin.clear();
      cin.ignore();
    }
    new_order.orderId = inputId;
  } while (false); // This loop exits after successfully getting a unique ID

  // Get customer name and validate
  getline(cin, new_order.customer_name);
  while (new_order.customer_name.empty()) {
    cout << "Please enter a name: ";
    getline(cin, new_order.customer_name);
  }

  vector<pair<int, int>> orderedItems;
  bool continueAddingItems = true;
  while (continueAddingItems) {
    cout << "Enter Item Number (1-20, 0 to Terminate): ";
    int itemIndex;
    while (!(cin >> itemIndex) || itemIndex < 0 || itemIndex > 20) {
      cout << "Invalid input. Please enter a number between 1 and 20: ";
      cin.clear();
      cin.ignore();
    }

    if (itemIndex == 0) {
      continueAddingItems = false;
      break; // Exit loop on zero
    }

    // Ask for quantity for this item
    cout << "Enter Quantity: ";
    int quantity;
    while (!(cin >> quantity) || quantity <= 0) {
      cout << "Invalid input. Please enter a positive number: ";
      cin.clear();
      cin.ignore();
    }

    orderedItems.emplace_back(itemIndex - 1, quantity);
  }

  double totalPrice = 0.0;
  for (const auto& [itemIndex, quantity] : orderedItems) {
    if (itemIndex < 0 || itemIndex >= loadedMenu.size()) {
      cout << "Error: Invalid item index." << endl;
      return; // Order creation failed
    }

    const MenuItem& item = loadedMenu[itemIndex];
    totalPrice += item.getPrice() * quantity;
  }

  new_order.total_cost = totalPrice;
  orders.push_back(new_order);
  order_nums++;

  cout << "-----------------------------------" << endl; // creates a bill once the order is placed :)
  cout << "Order ID: " << new_order.orderId << endl;
  cout << "Customer Name: " << new_order.customer_name << endl;
  cout << "-----------------------------------" << endl;

  cout << "Items Ordered: " << endl;
  for (const auto &item_info : orderedItems)
  {
    int item_index = item_info.first;
    int quantity = item_info.second;
    const MenuItem &item = loadedMenu[item_index];
    cout << " - " << item.getName() << " (" << quantity << ")" << endl;
  }

  cout << "-----------------------------------" << endl;
  cout << "Total Cost: Rs." << new_order.total_cost << endl;
  if (dine_in == 0)
  {
    cout << "Dine In Was Refused" << endl;
  }
  else
  {
    cout << "Table Picked: " << table_picked << endl;
  }
  cout << "-----------------------------------" << endl;

  outputFile << "Order ID: " << new_order.orderId << "\n"; // writing in da file
  outputFile << "Customer Name: " << new_order.customer_name << "\n";
  outputFile << "Items Ordered: ";
  for (const auto &item_info : orderedItems)
  {
    int item_index = item_info.first;
    const MenuItem &item = loadedMenu[item_index];
    outputFile << item.getName() << " (" << item_info.second << "), "; // Include quantity
  }
  outputFile.seekp(-2, std::ios::cur); // Move the cursor back to remove the trailing comma and space
  outputFile << "\n";
  outputFile << "Total Cost: " << new_order.total_cost << " Rs"
             << "\n\n";
  outputFile.close();

  cout << "Your Order Has Been Successfully Created" << endl;
  CustomerSatisfaction();
}

void CustomerSatisfaction()
{ // for customer feedback, it is declared on top phir used in ordercreation
  string waiter, speed, staff, clean;

  bool valid_name = false;
  while (!valid_name)
  {
    cout << "What was the name of the waiter? (1. Ahmed Qureshi, 2. Soban Ali, 3. Abdullah Malik): ";
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
      waiter = "Ahmed Qureshi";
      valid_name = true;
      break;
    case 2:
      waiter = "Soban Ali";
      valid_name = true;
      break;
    case 3:
      waiter = "Abdullah Malik";
      valid_name = true;
      break;
    default:
      cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
      break;
    }
  }

  string validOptions[] = {"Good", "Avg", "Bad", "good", "avg", "bad"}; // validation to ensure that only these 6 options are choosen from

  bool valid_speed = false; // for speed
  while (!valid_speed)
  {
    cout << "Speed of Serving (good/avg/bad): ";
    getline(cin, speed);

    for (const string &option : validOptions)
    {
      if (speed == option)
      {
        valid_speed = true;
        break;
      }
    }

    if (!valid_speed)
    {
      cout << "Make Sure To Enter A Valid Option " << endl;
    }
  }

  bool valid_staff = false; // for staff
  while (!valid_staff)
  {
    cout << "Waiter's Attitude (good/avg/bad): ";
    getline(cin, staff);

    for (const string &option : validOptions)
    {
      if (staff == option)
      {
        valid_staff = true;
        break;
      }
    }

    if (!valid_staff)
    {
      cout << "Please enter a valid option: Good, Avg, or Bad." << endl;
    }
  }

  bool valid_clean = false; // for cleaning
  while (!valid_clean)
  {
    cout << "Restaurant's Cleanliness (good/avg/bad): ";
    getline(cin, clean);

    for (const string &option : validOptions)
    {
      if (clean == option)
      {
        valid_clean = true;
        break;
      }
    }

    if (!valid_clean)
    {
      cout << "Please enter a valid option: Good, Avg, or Bad." << endl;
    }
  }

  ofstream feedbackFile("customer_feedback.txt", ios::app); // adds all the data into the file
  if (!feedbackFile)
  {
    cout << "Error opening feedback file." << endl;
    return;
  }

  feedbackFile << "--------------------------------------" << endl;
  feedbackFile << "Waiter Name: " << waiter << endl;
  feedbackFile << "Speed of Service: " << speed << endl;
  feedbackFile << "Waiter's Attitude: " << staff << endl;
  feedbackFile << "Restaurant's Cleanliness: " << clean << endl;
  feedbackFile << "--------------------------------------" << endl;

  feedbackFile.close();

  cout << "Feedback Noted :)" << endl;
}

// works
void CancelOrder(int orderId, vector<Orders>& orders) {
  auto it = find_if(orders.begin(), orders.end(),
                    [orderId](const Orders& order) { return order.orderId == orderId; });

  if (it != orders.end()) {
    // Remove the order from the vector
    orders.erase(it);
    order_nums--;

    // Update the file
    ofstream outputFile("orders.txt");
    if (outputFile.is_open()) {
      for (const auto& order : orders) {
        outputFile << "Order ID: " << order.orderId << "\n";
        outputFile << "Customer Name: " << order.customer_name << "\n";
        outputFile << "Items Ordered: ";
        for (const auto& item_info : orderedItems) {
          int item_index = item_info.first;
          const MenuItem& item = loadedMenu[item_index];
          outputFile << item.getName() << " (" << item_info.second << "), ";
        }
        outputFile.seekp(-2, std::ios::cur);
        outputFile << "\n";
        outputFile << "Total Cost: " << order.total_cost << " Rs"
                   << "\n\n";
      }
      outputFile.close();

      cout << "Order " << orderId << " has been successfully canceled." << endl;
    } else {
      cout << "Error writing to file." << endl;
    }
  } else {
    cout << "Order " << orderId << " not found." << endl;
  }
}
void OutputOrders()
{ // this for testing purposes only

  ifstream inputFile;
  inputFile.open("orders.txt");

  if (inputFile.is_open())
  {
    string line;
    while (getline(inputFile, line))
    {
      cout << line << endl;
    }
    inputFile.close();
  }
  else
  {
    cout << "Failed to open the file." << endl;
  }

  cout << "These Are All The Orders" << endl;
}

// works
void SearchOrders()
{

  string keyword;
  cout << "Enter The Keyword That You Want To Find: "; // looks for a specific word and outputs the order accordingly
  cin >> keyword;

  bool found = false;
  for (int i = 0; i < order_nums; i++)
  {

    if (orders[i].customer_name.find(keyword) != string::npos || orders[i].items_ordered.find(keyword) != string::npos ||
        to_string(orders[i].total_cost).find(keyword) != string::npos)
    {               // checks the entire string to see if the val was not found
                    // npos ka matlab is no position so like agar val nahi found to return false
      found = true; // if found

      cout << "Order ID: " << orders[i].orderId << endl;
      cout << "Customer Name: " << orders[i].customer_name << endl;
      cout << "Items Ordered: " << orders[i].items_ordered << endl;
      cout << "Quantity: " << orders[i].quantity << endl;
      cout << "Total Cost: Rs" << orders[i].total_cost << endl;
    }
  }

  if (!found)
  {
    cout << "No orders found matching the keyword '" << keyword << "'." << endl;
    return;
  }
}

// works

void WriteDataToFiles(const string &filePath, const Orders orders[], int order_nums)
{
  ofstream file(filePath);

  if (!file.is_open())
  {
    cerr << "Error opening file: " << filePath << endl;
    return;
  }

  // Write all orders
  for (int i = 0; i < order_nums; i++)
  {
    file << "Order ID: " << orders[i].orderId << endl;
    file << "Customer Name: " << orders[i].customer_name << endl;
    file << "Items Ordered: " << orders[i].items_ordered << endl;
    file << "Total Cost: " << orders[i].total_cost << endl;
    file << endl;
  }

  file.close();
}

// works
bool readDataFromFile(const string &filePath, vector<Orders> &orders)
{
  ifstream file(filePath);

  if (!file.is_open())
  {
    cerr << "Error opening file: " << filePath << endl;
    return false;
  }

  string line;
  int currentOrderId = 0;
  order_nums = 0;

  while (getline(file, line))
  {
    if (line.empty() || line.rfind("//", 0) == 0)
    { // checks if the line is empty or a comment
      continue;
    }

    if (line.find("Order ID:") != string::npos)
    {
      currentOrderId = stoi(line.substr(line.find(":") + 2));

      orders[order_nums] = Orders();
      orders[order_nums].orderId = currentOrderId;
      order_nums++;
    }
    else
    { // string manipulation to save each variable properly
      if (line.find("Customer Name:") != string::npos)
      {
        orders[order_nums - 1].customer_name = line.substr(line.find(":") + 2);
      }
      else if (line.find("Items Ordered:") != string::npos)
      {
        orders[order_nums - 1].items_ordered = line.substr(line.find(":") + 2);
      }
      else if (line.find("Quantity:") != string::npos)
      {
        // Update quantity in the existing order
        orders[order_nums - 1].quantity = stoi(line.substr(line.find(":") + 2));
      }
      else if (line.find("Total Cost:") != string::npos)
      {
        orders[order_nums - 1].total_cost = stoi(line.substr(line.find(":") + 2));
      }
    }
  }

  file.close();
  return true;
}

// works
void ExitProgram()
{
  cout << "Exiting Program..." << endl;
  exit(0);
}

vector<MenuItem> loadMenuItemsFromFile(const string &filename)
{
    vector<MenuItem> items;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Unable to open file: " << filename << endl;
        return items;
    }

    string line;
    while (getline(file, line))
    {
        MenuItem item;
        if (!line.empty())
        {
            item.deserialize(line);
            items.push_back(item);
        }
    }

    file.close();
    return items;
}
// works
void DisplayMenu()
{

  string filename = "hardees.txt";
  loadedMenu = loadMenuItemsFromFile(filename); // loads the info from the file and calls the methods/objects
  for (const auto &item : loadedMenu)
  {
    cout << "Item Name: " << item.getName() << endl;
    cout << "Category: " << item.getCategory() << endl;
    cout << "Price: PKR " << item.getPrice() << endl;
    cout << "Description: " << item.getDescription() << endl;
    cout << "Availability: " << (item.getAvailaibilty() ? "Available" : "Not Available") << endl;
    cout << "-----------------\n";
  }
}

int main()
{
  int option;
  loadedMenu = loadMenuItemsFromFile("hardees.txt");

  vector<Orders> orders;
  readDataFromFile("orders.txt", orders); // before exexuting the program reads the file

  do
  { // this was kafi uneccesary but looks cool (totally didnt spend way too much time aligning everything)
    cout << "\t-┌───────────────────────────────────────┐-" << endl;
    cout << "-----= \t│      Restaurant Management System       │  =-----" << endl;
    cout << "\t-└───────────────────────────────────────┘-" << endl;

    cout << "\t    ┌───────────────────────────────┐" << endl;
    cout << "\t    │ 1. Create a New Order         │" << endl;
    cout << "\t    │ 2. Cancel an Order            │" << endl;
    cout << "\t    │ 3. Search for an Order        │" << endl;
    cout << "\t    │ 4. Display The Menu           │" << endl;
    cout << "\t    │ 5. Exit                       │" << endl;
    cout << "\t    └───────────────────────────────┘" << endl;

    cout << endl;
    cout << "Enter your option: ";
    cin >> option;

    switch (option)
    {
    case 1:
      CreateOrder(orders, order_nums);
      // WriteDataToFiles("orders.txt", orders, order_nums);
      break;
    case 2:
      {
          int orderIdToCancel;
          cout << "Enter the Order ID to cancel: ";
          cin >> orderIdToCancel;
          CancelOrder(orderIdToCancel, orders); 
          break;
      }
    case 3:
      SearchOrders();
      break;
    case 4:
      DisplayMenu();
      break;
    case 5:
      ExitProgram();
      break;
    default:
      cout << "Invalid option. Please try again." << endl;
      break;
    }

    cout << "\n";
  } while (option != 5);

  return 0;
}