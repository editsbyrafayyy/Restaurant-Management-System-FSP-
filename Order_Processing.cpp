// I have used PF wali libraries, so code thora restricted but it is what it is, things that are yet to be changed:
// combining per se the programs (unmein compatibility add karna somehow), either removing the invoice file cause 
// bruhhh phir usko bhi read aur write karna hoga mujhay nahi pata kaisay hoga, file mein jo tables save hotay
// wo thoray ajeeb hein, havent figured that part completely. so yeye have commented most parts gl

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
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

	void setName(string itemName) {
		name = itemName;
	}

	void setPrice(double itemprice) {
		price = itemprice;
	}

	void setDescription(string desc) {
		description = desc;
	}

	void setCategory(string cat) {
		category = cat;
	}

	void setAvailaibilty(bool aval) {
		availaiblity = aval;
	}

	string getName() const {
		return name;
	}

	string getCategory() const {
		return category;
	}

	double getPrice() const {
		return price;
	}

	bool getAvailaibilty() const {
		return availaiblity;
	}

	string getDescription() const {
		return description;
	}

	string serialize() const {
		return name + "*" + to_string(price) + "*" + description + "*" + (availaiblity ? "1" : "0") + "*" + category;
	}

	void deserialize(const string serializedString) {
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

vector<MenuItem> loadedMenu; //declaring a global array
const int max_orders = 20; // max orders
const int max_tables = 10; // max tables
const int max_items = 30; // Adjust as needed

struct Orders { 
  int orderId;
  string customer_name;
  string items_ordered;
  int quantity;
  int price;
  int total_cost;
  int assignedTableId; 
};

Orders orders[max_orders]; //declaring a new var with the properties of Orders structure
int order_nums = 0; // number of orders

MenuItem stores[max_items]; // Array to store menu items
int itemCount = 0; // Track item count

void CustomerSatisfaction(); // calling da function

void CreateOrder_TableManagement() {
  bool foundDuplicate = false;

  int table_picked;
  const int max_tables = 20; 
  bool tableAvailable[max_tables];

  for (int i = 0; i < max_tables; i++) {
    tableAvailable[i] = true;
  }

  ofstream outputFile("orders.txt", ios::app);
  if (!outputFile.is_open()) {
    cout << "Error opening file." << endl;
    return;
  }

  if (order_nums >= max_orders) {
    cout << "Max Orders Exceeded" << endl;
    return;
  }

  Orders new_order; // creating a var with the properties of Orders structure
  new_order.orderId = order_nums + 1;
  order_nums++;

bool dine_in = false;

do {
  cout << "Do You Want To Dine in (y/n)?: ";
  char choice;
  cin >> choice;

  if (!cin) {
    cin.clear(); // clears the error state 
    cin.ignore(); // clears the input (takay new input can be taken)
    cout << "Invalid input. Please enter y or n." << endl;
    continue;
  }

  if (choice == 'n' || choice == 'N') {
      break;
    }

  dine_in = (choice == 'y' || choice == 'Y');
} while (!dine_in && !cin.fail()); // if the input is a num for not (so error handling lol)

  if (dine_in) {
    cout << "Available Tables:" << endl;
    for (int i = 0; i < max_tables; i++) {
      if (tableAvailable[i]) {
        cout << " - " << (i + 1) << endl;
      }
    }

    do {
      cout << "Choose a table number (or 0 to cancel): ";
      cin >> table_picked;

      if (table_picked < 0 || table_picked > max_tables) { // to make sure the number if valid (error handling)
        cout << "Invalid table number. Please choose a valid number or 0 to cancel." << endl;
      } 
      else if (!tableAvailable[table_picked - 1]) { 
        cout << "Table " << table_picked << " is unavailable. Please choose another." << endl;
      } 
      else {
        tableAvailable[table_picked - 1] = false; // that table is marked as unavailableee
        break; 
      }
    } while (true);

    if (dine_in) {
      cout << "\nTable " << table_picked << " has been assigned to your order." << endl; // displays the table assigned
    }
  }

  do {
  int input_id; 
  cout << "Enter Order ID: ";
  cin >> input_id;

  if (cin.fail()) { // so .fail checks if the input is a num for not (so error handling lol)
    cout << "Invalid input. Please enter a number: ";
    cin.clear(); // clears the error state 
    cin.ignore(); // clears the input (takay new input can be taken)
  } else {

    for (const Orders& order : orders) {
      if (order.orderId == input_id) { // compares to see if ID exists
        foundDuplicate = true;
        break;
      }
    }

    if (foundDuplicate) {
      cout << "The Order ID " << input_id << " already exists,  Please enter a different ID: ";
    } else {
      new_order.orderId = input_id; // if it doesnt exist tou saves it here
      break;
    }
  }
} while (foundDuplicate);

  getline(cin, new_order.customer_name); // getline ensures that spaces are also taken into account for strings
  while (new_order.customer_name.empty()) { // makes sure empty nahi ha input
    cout << "Please enter the customer's name: ";
    getline(cin, new_order.customer_name);
  }

  vector<int> orderedItems;
  new_order.quantity = 1; 

  do {
  cout << "Enter Item Number for The Item The User Wants: (1-20, 0 to Terminate The Program): ";
  int item_index;
  while (!(cin >> item_index) || item_index < 0 || item_index > 20) { // ensures the value input for the item selected
    cout << "Invalid input. Please enter a number between 1 and 20: ";
    cin.clear();
    cin.ignore();
  }

  if (item_index == 0) break; 

  cout << "Enter The Quantity: ";
  while (!(cin >> new_order.quantity) || new_order.quantity <= 0) {
    cout << "Invalid input. Please enter a positive number: ";
    cin.clear();
    cin.ignore();
  }

  orderedItems.push_back(item_index - 1);
} while (true);

  double totalPrice = 0.0;
  for (int item_index : orderedItems) {

    if (item_index < 0 || item_index >= loadedMenu.size()) {
      cout << "Error: Invalid item index." << endl;
      outputFile.close();
      return;
    }

    totalPrice += loadedMenu[item_index].getPrice() * new_order.quantity;
  }
  new_order.total_cost = totalPrice;

  orders[order_nums++] = new_order;

    cout << "-----------------------------------" << endl; // creates a bill once the order is placed :)
  cout << "Order ID: " << new_order.orderId << endl;
  cout << "Customer Name: " << new_order.customer_name << endl;
  cout << "-----------------------------------" << endl;

  cout << "Items Ordered: " << endl;
  for (int item_index : orderedItems) {
    const MenuItem& item = loadedMenu[item_index];
    cout << " - " << item.getName() << " (" << new_order.quantity << ")" << endl;
  }

  cout << "-----------------------------------" << endl;
  cout << "Quantity: " << new_order.quantity << endl;
  cout << "Total Cost: Rs." << new_order.total_cost << endl;
    if(dine_in == 0){
      cout << "Dine In Was Refused" << endl;
  }
  else{
  cout << "Table Picked: " << table_picked << endl;
  }
  cout << "-----------------------------------" << endl;

  outputFile << "Order ID: " << new_order.orderId << "\n"; // writing in da file
  outputFile << "Customer Name: " << new_order.customer_name << "\n";
  outputFile << "Items Ordered: ";
  for (int item_index : orderedItems) {
    const MenuItem& item = loadedMenu[item_index]; 
    outputFile << item.getName();
  }
  outputFile.seekp(-2, ios::cur);
  outputFile << "\n";
  outputFile << "Quantity: " << new_order.quantity << "\n";
  outputFile << "Total Cost: " << new_order.total_cost << " Rs" << "\n\n";
  outputFile.close();

  cout << "Your Order Has Been Successfully Created"<<endl;
  CustomerSatisfaction();
}

void CustomerSatisfaction() { // for customer feedback, it is declared on top phir used in ordercreation
  string waiter, speed, staff, clean;

  bool valid_name = false;
  while (!valid_name) {
    cout << "What was the name of the waiter? (1. Ahmed Qureshi, 2. Soban Ali, 3. Abdullah Malik): ";
    int choice;
    cin >> choice;

    switch (choice) {
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
  while (!valid_speed) {
    cout << "Speed of Serving (good/avg/bad): ";
    getline(cin, speed);

    for (const string& option : validOptions) {
      if (speed == option) {
        valid_speed = true;
        break;
      }
    }

    if (!valid_speed) {
      cout << "Make Sure To Enter A Valid Option " << endl;
    }
  }

    bool valid_staff = false; // for staff
  while (!valid_staff) {
    cout << "Waiter's Attitude (good/avg/bad): ";
    getline(cin, staff);

    for (const string& option : validOptions) {
      if (staff == option) {
        valid_staff = true;
        break;
      }
    }

    if (!valid_staff) {
      cout << "Please enter a valid option: Good, Avg, or Bad." << endl;
    }
  }

  bool valid_clean = false; // for cleaning
    while (!valid_clean) {
      cout << "Restaurant's Cleanliness (good/avg/bad): ";
      getline(cin, clean);

      for (const string& option : validOptions) {
        if (clean == option) {
          valid_clean = true;
          break;
        }
      }

      if (!valid_clean) {
        cout << "Please enter a valid option: Good, Avg, or Bad." << endl;
      }
    }

  ofstream feedbackFile("customer_feedback.txt", ios::app); // adds all the data into the file 
  if (!feedbackFile) {
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

//works
void CancelOrder() {

  int ID;
  cout << "Enter order ID to cancel: ";
  cin >> ID; 

  bool found = false;
  int index = -1;
  for (int i = 0; i < order_nums; i++) {
    if (orders[i].orderId == ID) { // searches for the ID
      found = true;
      index = i; // brings it to that index where it is located
      break; 
    }
  }

  if (!found) {
    cout << "The Order Does Not Exist" << endl;
    return;
  }

  if (index < order_nums - 1) { // this shifts the orders from the point where the order was removed
    orders[index] = orders[order_nums - 1];
  }

  order_nums--;

  ofstream outputFile("orders.txt", ios::out | ios::trunc); // entering all the data into the file backkk
  if (!outputFile.is_open()) {
    cout << "Error opening file." << endl;
    return;
  }

  for (int i = 0; i < order_nums; i++) {
    outputFile << "Order ID: " << orders[i].orderId << "\n";
    outputFile << "Customer Name: " << orders[i].customer_name << "\n";
    outputFile << "Items Ordered: " << orders[i].items_ordered << "\n";
    outputFile << "Quantity: " << orders[i].quantity << "\n";
    outputFile << "Total Price in Rs: " << orders[i].total_cost << "\n";
  }

  outputFile.close();

  cout << "The Order Has Been Successfully Canceled." << endl;
}

void OutputOrders() { // this for testing purposes only

    ifstream inputFile;
    inputFile.open("orders.txt");

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
                cout << line << endl;
        }
        inputFile.close();
    } else {
        cout << "Failed to open the file." << endl;
    }

    cout << "These Are All The Orders" << endl;
}

//works

bool containsKeyword(const Orders& order, const string& keyword, const vector<MenuItem>& menu) {
    if (order.customer_name.find(keyword) != string::npos ||
        to_string(order.total_cost).find(keyword) != string::npos) {
        return true;
    }

    stringstream ss(order.items_ordered);
    string itemID;
    while (getline(ss, itemID, ',')) {
        if (itemID.empty() || !isdigit(itemID[0])) {
            cerr << "Error: Invalid item ID format in order. Skipping this item." << endl;
            continue;
        }

        int item_index = stoi(itemID);
        if (item_index >= 0 && item_index < menu.size()) {
            const MenuItem& item = menu[item_index];
            if (item.getName().find(keyword) != string::npos) {
                return true;
            }
        } else {
            cerr << "Error: Invalid item index in order. Skipping this item." << endl;
        }
    }

    return false;
}

void displayOrderDetails(const Orders& order, const vector<MenuItem>& menu) {
    cout << "--------------------------------------" << endl;
    cout << "Order ID: " << order.orderId << endl;
    cout << "Customer Name: " << order.customer_name << endl;
    cout << "Items Ordered: " << endl;

    stringstream ss(order.items_ordered);
    string itemID;
    while (getline(ss, itemID, ',')) {
        if (itemID.empty() || !isdigit(itemID[0])) {
            cerr << "Error: Invalid item ID format in order. Skipping this item." << endl;
            continue;
        }

        int item_index = stoi(itemID);
        if (item_index >= 0 && item_index < menu.size()) {
            const MenuItem& item = menu[item_index];
            cout << " - " << item.getName() << " (" << order.quantity << ")" << endl;
        } else {
            cerr << "Error: Invalid item index in order. Skipping this item." << endl;
        }
    }

    cout << "Total Cost: Rs" << order.total_cost << endl;
}

void SearchOrders(const vector<MenuItem>& menu) {
    string keyword;
    cout << "Enter The Keyword That You Want To Find: ";
    cin >> keyword;

    bool found = false;
    for (int i = 0; i < order_nums; i++) {
        if (containsKeyword(orders[i], keyword, menu)) {
            displayOrderDetails(orders[i], menu);
            found = true;
        }
    }

    if (!found) {
        cout << "No orders found matching the keyword '" << keyword << "'." << endl;
    }
}



//works
void WriteDataToFiles(const string& filePath, const Orders orders[], int order_nums) {
    ofstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    for (int i = 0; i < order_nums; i++) {
        file << "Order ID: " << orders[i].orderId << "\n";
        file << "Customer Name: " << orders[i].customer_name << "\n";
        file << "Items Ordered: " << orders[i].items_ordered << "\n";
        file << "Quantity: " << orders[i].quantity << "\n";
        file << "Total Cost: " << orders[i].total_cost << " Rs" << "\n\n";
    }

    file.close();
}

//works
bool readDataFromFile(const string& filePath, Orders orders[], int& order_nums) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return false;
    }

    string line;
    int currentOrderId = 0;
    order_nums = 0;

    while (getline(file, line)) {
        if (line.empty() || line.rfind("//", 0) == 0) {
            continue;
        }

        if (line.find("Order ID:") != string::npos) {
            currentOrderId = stoi(line.substr(line.find(":") + 2));
            orders[order_nums] = Orders();
            orders[order_nums].orderId = currentOrderId;
            order_nums++;
        } else {
            istringstream iss(line);
            string key, value;
            getline(iss, key, ':');
            getline(iss, value);

            // Process different keys and assign values accordingly
            if (key == "Customer Name") {
                orders[order_nums - 1].customer_name = value;
            } else if (key == "Items Ordered") {
                orders[order_nums - 1].items_ordered = value;
            } else if (key == "Quantity") {
                orders[order_nums - 1].quantity = stoi(value);
            } else if (key == "Total Cost") {
                orders[order_nums - 1].total_cost = stoi(value);
            }
        }
    }

    file.close();
    return true;
}

//works
void ExitProgram() {
  ofstream outputFile("orders.txt", ios::out | ios::trunc); // exits and updates everything 

  // if (!outputFile.is_open()) {
  //   cerr << "Error opening file." << endl;
  //   return;
  // }

  // for (int i = 0; i < order_nums; i++) { // writing all the data back into the file (in case, read karnay mein problem araha tha)
  //   outputFile << "Order ID: " << orders[i].orderId << endl;
  //   outputFile << "Customer Name: " << orders[i].customer_name << endl;
  //   outputFile << "Items Ordered: " << orders[i].items_ordered << endl;
  //   outputFile << "Quantity: " << orders[i].quantity << endl;
  //   outputFile << "Total Cost (Rs): " << orders[i].total_cost << endl;
  // }

  // outputFile.close();

  cout << "Exiting Program..." << endl;
  exit(0);
}

vector<MenuItem> loadMenuItemsFromFile(const string& filename) {
	vector<MenuItem> items;
	ifstream file(filename);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			MenuItem item;
			item.deserialize(line);
			items.push_back(item);
		}
		file.close();
	}
	else {
		cout << "Error: Unable to open file: " << filename << endl;
	}
	return items;
}

//works
void DisplayMenu() {

  string filename = "hardees.txt";
		loadedMenu = loadMenuItemsFromFile(filename); // loads the info from the file and calls the methods/objects
			for (const auto& item : loadedMenu) {
				cout << "Item Name: " << item.getName() << endl;
				cout << "Category: " << item.getCategory() << endl;
				cout << "Price: PKR " << item.getPrice() << endl;
				cout << "Description: " << item.getDescription() << endl;
				cout << "Availability: " << (item.getAvailaibilty() ? "Available" : "Not Available") << endl;
				cout << "-----------------\n";
			}
}

int main() {
  int option;
  loadedMenu = loadMenuItemsFromFile("hardees.txt");

  readDataFromFile("orders.txt", orders, order_nums); // before exexuting the program reads the file

  do { // this was kafi uneccesary but looks cool (totally didnt spend way too much time aligning everything)
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

    cout<<endl;
    cout << "Enter your option: ";
    cin >> option;

    switch (option) {
      case 1:
        CreateOrder_TableManagement();
        break;
      case 2:
        CancelOrder();
        WriteDataToFiles("orders.txt", orders, order_nums);
        break;
      case 3:
        SearchOrders(loadedMenu);
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