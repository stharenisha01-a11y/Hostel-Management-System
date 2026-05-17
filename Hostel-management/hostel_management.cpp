#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <cctype>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

using namespace std;

// ============================================================================
// CONSTANTS AND GLOBAL VARIABLES
// ============================================================================
const int singleBeds = 10;
const int doubleBeds = 30;
const int tripleBeds = 24;

// Occupancy arrays: 0 = empty, 1 = occupied
int singleRoomOccupancy[singleBeds] = {0};
int doubleRoomOccupancy[doubleBeds] = {0};
int tripleRoomOccupancy[tripleBeds] = {0};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Cross-platform password masking function (Windows, Mac, Linux)
string getPasswordInput() 
{
    string password = "";
    
    #ifdef _WIN32
        // Windows implementation
        char ch;
        while(true)
         {
            ch = _getch();  
            
            if(ch == 13)
             {  
                cout << endl;
                break;
            }
            else if(ch == 8) 
            {  
                if(password.length() > 0)
                 {
                    password.pop_back();
                    cout << "\b \b";  
                }
            }
            else 
            {
                password += ch;
                cout << "*"; 
            }
        }
    #else
        // Unix/Linux/Mac implementation
        struct termios oldt, newt;
        char ch;
        
        // Get current terminal settings
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        
        // Disable echo
        newt.c_lflag &= ~(ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        while(true) {
            ch = getchar();
            
            if(ch == '\n')
             {  
             cout << endl;
                break;
            }
            else if(ch == 127 || ch == 8) 
            {  
                if(password.length() > 0)
                 {
                    password.pop_back();
                    cout << "\b \b";  
                }
            }
            else {
                password += ch;
                cout << "*";  
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif
    
    return password;
}
bool isValidContact(string number)
{
    if(number.length() != 10)
    {
        return false;
    }

    if(number.substr(0,2) != "98" &&
       number.substr(0,2) != "97")
    {
        return false;
    }

    for(int i = 0; i < number.length(); i++)
    {
        if(!isdigit(number[i]))
        {
            return false;
        }
    }

    return true;
}

// ============================================================================
// HELPER FUNCTION DECLARATIONS & IMPLEMENTATIONS
// ============================================================================

// Helper: Find index of first valid contact number in token list
int findFirstContact(const vector<string> &toks, int startIdx) {
    for(size_t i = startIdx; i < toks.size(); ++i) {
        if(isValidContact(toks[i])) return (int)i;
    }
    return -1;
}

void parseStudentRecord(const string &line, string &id, string &name, string &contact,
                        string &fname, string &fcontact, string &rtype, int &bed, string &foodType);
string makeStudentRecord(const string &id, const string &name, const string &contact,
                         const string &fname, const string &fcontact, const string &rtype,
                         int bed, const string &foodType);

// ============================================================================
// HOSTEL CLASS
// ============================================================================

class hostel {
private:
    string student_name, student_contact, family_name, family_contact, roomtype;
    string generateStudentID();
    int totalclothes;
    double ratePerCloth;
    int bedNumber;
    string loggedInUser="";
    string loggedInRoom="";
    string loggedInID = "";

public:
    hostel() { totalclothes = 0; ratePerCloth = 10; }
    
    // ---- Authentication ----
    void adminLogin();
    void studentLogin();
    
    // ---- Student Features ----
    void studentMenu();
    void feeManagement();
    void laundaryStudent();
    void addlaundary();
    void viewlaundaryStudent();
    
    // ---- Admin Menu & Registration ----
    void screen();
    void reserve();
    void add();
    void update();
    void remove();
    void viewStudentDetails();
    
    // ---- Admin: Fees & Payments ----
    void viewAllStudentFees();
    void recordPayment();
    
    // ---- Admin: Laundry Management ----
    void viewlaundaryAdmin();
    void updatelaundary();
    
    // ---- Shared: Food & Laundry (accessible to both) ----
    void food();
    void laundary();
    
    // ---- Room Management ----
    void roomSystem();
    void showRoomAvailability();
    void roomTypeDetails();
    void viewRooms();
    void viewRoomsStudentSimple();
    void loadRoomOccupancy();
};

void hostel::studentMenu() 
{
    int choice;
    do {
        cout << "\n================================";
        cout << "\n      STUDENT DASHBOARD";
        cout << "\n================================";
        cout << "\n1. Fee Management (View Dues)";
        cout << "\n2. Food Schedule";
        cout << "\n3. Laundry System";
        cout << "\n4. View Room Occupancy";
        cout << "\n5. Logout";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
         {
            case 1: 
            {
                feeManagement();
                break;
            }
            case 2: 
            {
                food(); 
                break;
            }
            case 3: 
            {
                laundaryStudent(); 
                break;
            }
            case 4: 
            {
                viewRoomsStudentSimple(); 
                break;
            }
            case 5: 
            {
                cout << "Logging out...\n"; 
                break;
            }
            default: 
            {
                cout << "Invalid choice!\n";
            }
        }
    } 
    while(choice != 5);
}
void hostel::studentLogin() 
{
    string user, pass;
    bool found = false;

    cout << "\n--- STUDENT LOGIN (DEMO) ---";
    cout << "\nEnter student ID: "; 
    cin >> user;

    cout << "Password: "; 
    pass = getPasswordInput();

    ifstream infile("students.txt");


string line;
while(getline(infile, line))
{
    if(line.empty()) continue;

    string id, name, contact, fname, fcontact, rtype, foodType;
    int bed;
    parseStudentRecord(line, id, name, contact, fname, fcontact, rtype, bed, foodType);
    if(id == user)
    {
        found = true;
        loggedInUser = name;
        loggedInRoom = rtype;
        loggedInID = id;
        break;
    }
}

infile.close();

    if(found && pass == "123")
{
    cout << "\nLogin Successful! Welcome, " << loggedInUser << ".\n";
    cout << "DEBUG -> " << loggedInID << " " << loggedInRoom << endl;
    studentMenu();
}
    else
    {
        cout << "\nInvalid Student Credentials or Student not found in records.\n";
    }
}
void hostel::feeManagement() 
{
    system("cls");
    if(loggedInID == "")
{
    cout << "Error: No student logged in properly!\n";
    return;
}
    int roomPrice = 0;
    totalclothes = 0;
    int foodFee = 0;

    ifstream file("laundry.txt");
    string id;
    int clothes;
    while(file >> id >> clothes)
    {
        if(id == loggedInID)
        {
            totalclothes += clothes;
        }
    }
    file.close();
   ifstream foodfile("food.txt");
   string fid, foodtype;
while(foodfile >> fid >> foodtype)
{
    if(fid == loggedInID)
    {
        if(foodtype == "Veg")
        {
            foodFee = 2500;
        }
        else if(foodtype == "NonVeg")
        {
            foodFee = 3500;
        }
        else
        {
            foodFee = 0;
        }

        break;
    }
}

foodfile.close();
    
   if (loggedInRoom == "Single" || loggedInRoom == "single")
    {
        roomPrice = 6000;
    } 
    else if (loggedInRoom == "Double" || loggedInRoom == "double") 
    {
        roomPrice = 5000;
    } 
    else if (loggedInRoom == "Triple" || loggedInRoom == "triple") 
    {
        roomPrice = 4000;
    }
    cout << "\n================================";
    cout << "\n       FEE MANAGEMENT";
    cout << "\n================================";
    cout << "\nStudent Name : " << loggedInUser;
    cout << "\nRoom Type    : " << (loggedInRoom == "" ? "Not Assigned" : loggedInRoom);
    cout << "\nMonthly Fee  : Rs. " << roomPrice;
    cout << "\nLaundry      : Rs. " << (totalclothes * ratePerCloth);
    cout << "\nFood Fee     : Rs. " << foodFee;
    cout << "\n--------------------------------";
    cout << "\nTotal Due    : Rs. " << (roomPrice + (totalclothes * ratePerCloth) + foodFee);    
    cout << "\n================================\n";
    
    if(roomPrice == 0)
     {
        cout << "Note: Please contact Admin to assign a valid room.\n";
    }
}

void hostel::adminLogin() 
{
    string user, pass;
    cout << "\n--- ADMIN AUTHENTICATION ---";
    cout << "\nUsername: "; 
    cin >> user;
    cout << "Password: "; 
    pass = getPasswordInput();

    if(user=="admin" && pass=="1234") 
    {
        cout << "\nAccess Granted!";
        screen(); 
    } 
    else 
    {
        cout << "\nInvalid Credentials.";
    }
}

void hostel::screen() 
{
    int s;
    do {
        cout << "\n\t** WELCOME TO OUR HOSTEL (ADMIN) **" << endl;
        cout << "\t\t1. STUDENT REGISTRATION " << endl;
        cout << "\t\t2. FOOD SCHEDULE" << endl;
        cout << "\t\t3. LAUNDRY SERVICE" << endl;
        cout << "\t\t4. ROOM MANAGEMENT" << endl;
        cout << "\t\t5. VIEW STUDENT FEES & DUES" << endl;
        cout << "\t\t6. RECORD FEE PAYMENT" << endl;
        cout << "\t\t7. EXIT" << endl;
        cout << "\t\tEnter your Choice: ";
        cin >> s;

        switch(s) {
            case 1: reserve(); break;
            case 2: food(); break;
            case 3: laundary(); break;
            case 4: roomSystem(); break;
            case 5: viewAllStudentFees(); break;
            case 6: recordPayment(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(s != 7);
}

void hostel::viewStudentDetails() 
{
    string search_id;
    bool found = false;
    cout << "\n--- SEARCH STUDENT DETAILS ---";
    cout << "\nEnter Student ID to Search: ";
    cin >> search_id;
    ifstream infile("students.txt");
    string id, s_name, s_contact, f_name, f_contact, rtype, foodType;
    int bed;
    string line;
    while(getline(infile, line))
    {
        if(line.empty()) continue;
        parseStudentRecord(line, id, s_name, s_contact, f_name, f_contact, rtype, bed, foodType);
        if(id == search_id) 
        {
            found = true;
            cout << "\n================================";
            cout << "\n      STUDENT RECORD FOUND      ";
            cout << "\n================================";
            cout << "\nName:            " << s_name;
            cout << "\nContact:         " << s_contact;
            cout << "\nFamily Name:     " << f_name;
            cout << "\nFamily Contact:  " << f_contact;
            cout << "\nRoom Type:       " << rtype;
            cout << "\nBed Number:      " << bed;
            cout << "\nFood Preference: " << foodType;
            cout << "\n================================\n";
            break; 
        }
    }
    infile.close();
    if(!found) 
    {
        cout << "\nError: No student found.\n";
    }
}

void hostel::reserve() 
{
    system("cls");
    int r;
    do 
    {
        cout << "\n\t** RESERVATION & REGISTRATION MENU **\n";
        cout << "\t\t1. ADD NEW STUDENT\n\t\t2. UPDATE STUDENT RECORD\n\t\t3. REMOVE STUDENT RECORD\n\t\t4. SEARCH STUDENT DETAILS\n\t\t5. EXIT TO MAIN MENU\nEnter Choice: ";
        cin >> r;
        switch(r) 
        {
            case 1: 
            {
                add(); 
                break;
            }
            case 2: 
            {
                update(); 
                break;
            }
            case 3: 
            {
                remove(); 
                break;
            }
            case 4: 
            {
                viewStudentDetails(); 
                break;
            }
            case 5: break;
        }
    } 
    while(r != 5);
}

void hostel::add() 
{
    
    string id = generateStudentID();
    cout << "Generated Student ID: " << id << endl;
    ifstream checkFile("students.txt");
    string existingLine;
    string existingID;
    while(getline(checkFile, existingLine)) {
        if(existingLine.empty()) continue;
        string _name, _contact, _fname, _fcontact, _rtype, _food;
        int _bed;
        parseStudentRecord(existingLine, existingID, _name, _contact, _fname, _fcontact, _rtype, _bed, _food);
        if(existingID == id) {
            cout << "Error: ID already exists!\n";
            checkFile.close();
            return;
        }
    }
    checkFile.close();
    
ofstream file("students.txt", ios::app);

if(!file)
{
    cout << "ERROR: Cannot create students.txt\n";
    return;
}   cin.ignore();

cout << "Enter student name: ";
getline(cin, student_name);
  do
{
    cout << "Enter contact: ";
    cin >> student_contact;

    if(!isValidContact(student_contact))
    {
        cout << "Invalid number! Enter valid Nepal number.\n";
    }

}
while(!isValidContact(student_contact));
cin.ignore(1000, '\n');

cout << "Enter family name: ";
getline(cin, family_name);
do
{
    cout << "Enter family contact: ";
    cin >> family_contact;

    if(!isValidContact(family_contact))
    {
        cout << "Invalid number! Enter valid Nepal number.\n";
    }

}
while(!isValidContact(family_contact));
    cout << "Enter Room Type (Single/Double/Triple): ";
    cin >> roomtype;

    int chosenBed;
    if(roomtype=="Single") 
    {
        cout << "Choose bed number (1-" << singleBeds << "): ";
        cin >> chosenBed;
        if(chosenBed<1 || chosenBed>singleBeds || singleRoomOccupancy[chosenBed-1]==1) 
        {
            cout << "Invalid or occupied bed!\n"; return;
        }
        singleRoomOccupancy[chosenBed-1]=1; bedNumber=chosenBed;
    }
    else if(roomtype=="Double") {
        cout << "Choose bed number (1-" << doubleBeds << "): ";
        cin >> chosenBed;
        if(chosenBed<1 || chosenBed>doubleBeds || doubleRoomOccupancy[chosenBed-1]==1) 
        {
            cout << "Invalid or occupied bed!\n"; return;
        }
        doubleRoomOccupancy[chosenBed-1]=1; bedNumber=chosenBed;
    }
    else if(roomtype=="Triple") 
    {
        cout << "Choose bed number (1-" << tripleBeds << "): ";
        cin >> chosenBed;
        if(chosenBed<1 || chosenBed>tripleBeds || tripleRoomOccupancy[chosenBed-1]==1) 
        {
            cout << "Invalid or occupied bed!\n"; return;
        }
        tripleRoomOccupancy[chosenBed-1]=1; bedNumber=chosenBed;
    } 
    
    else 
    { 
        cout << "Invalid room type!\n"; return; 
    }
    int foodChoice;
string foodType;

cout << "\nFood Preference:\n";
cout << "1. Veg\n";
cout << "2. Non-Veg\n";
cout << "3. Outside Hostel\n";
cout << "Enter choice: ";
cin >> foodChoice;

if(foodChoice == 1)
{
    foodType = "Veg";
}
else if(foodChoice == 2)
{
    foodType = "NonVeg";
}
else
{
    foodType = "Outside";
}

     file << makeStudentRecord(id, student_name, student_contact, family_name, family_contact, roomtype, bedNumber, foodType) << endl;
    file.close();

ofstream foodfile("food.txt", ios::app);

if(!foodfile)
{
    cout << "ERROR: Cannot create food.txt\n";
    return;
}
foodfile << id << " " << foodType << endl;

foodfile.close();

    cout << "Student ID: " << id << endl;
    cout << "Student added successfully!\n";
    cout << "Room: " << roomtype << ", Bed: " << bedNumber << endl;
}

void hostel::viewRoomsStudentSimple() 
{
    ifstream file("students.txt");
    cout << "\n--- ROOM OCCUPANCY (STUDENT VIEW) ---\n";
    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;
        string id, name, contact, fname, fcontact, rtype, foodType;
        int bed;
        parseStudentRecord(line, id, name, contact, fname, fcontact, rtype, bed, foodType);
        cout << "\nStudent: " << name;
        cout << "\nRoom Type: " << rtype;
        cout << "\nBed Number: " << bed;
        cout << "\n------------------------";
    }
    file.close();
}

void hostel::viewRooms() 
{
    ifstream file("students.txt");
    cout << "\n--- ROOM OCCUPANCY (ADMIN VIEW) ---\n";
    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;
        string id, name, contact, fname, fcontact, rtype, foodType;
        int bed;
        parseStudentRecord(line, id, name, contact, fname, fcontact, rtype, bed, foodType);
        cout << "Student ID: " << id;
        cout << "\nStudent: " << name;
        cout << "\nRoom Type: " << rtype;
        cout << "\nBed Number: " << bed;
        cout << "\n-----------------------";
    }
    file.close();
}

void hostel::roomSystem() 
{
    system("cls");
    int choice;
    do 
    {
        cout << "\n** ROOM MANAGEMENT **\n1. View Availability\n2. Room Details\n3. View Room Occupancy\n4. Back\nEnter choice: ";
        cin >> choice;
        if(choice==1) showRoomAvailability();
        else if(choice==2) roomTypeDetails();
        else if(choice==3) viewRooms(); // admin view
    } while(choice!=4);
}

void hostel::update() 
{

    string search_id;
    bool found = false;

    cout << "Enter student ID to update: ";
    cin >> search_id;

    ifstream infile("students.txt");
    ofstream tempfile("temp.txt");
    string line;
    while(getline(infile, line)) {
        if(line.empty()) continue;
        string id, s_name, s_contact, f_name, f_contact, rtype, foodType;
        int bed;
        parseStudentRecord(line, id, s_name, s_contact, f_name, f_contact, rtype, bed, foodType);
        if (id == search_id) {
            found = true;
            do {
                cout << "Enter new contact: ";
                cin >> student_contact;
                if(!isValidContact(student_contact))
                    cout << "Invalid number! Enter valid Nepal number.\n";
            } while(!isValidContact(student_contact));

            cin.ignore(1000,'\n');
            cout << "Enter new family name: ";
            getline(cin, family_name);

            do {
                cout << "Enter new family contact: ";
                cin >> family_contact;
                if(!isValidContact(family_contact))
                    cout << "Invalid number! Enter valid Nepal number.\n";
            } while(!isValidContact(family_contact));

            tempfile << makeStudentRecord(id, s_name, student_contact, family_name, family_contact, rtype, bed, foodType) << "\n";
        } else {
            tempfile << line << "\n";
        }
    }

    infile.close();
    tempfile.close();

    std::remove("students.txt");
    rename("temp.txt", "students.txt");

    if(found)
        cout << "Student record updated successfully!\n";
    else
        cout << "Student not found!\n";
}

void hostel::remove() 
{
    string search_id;
    bool found = false;

    cout << "Enter Student ID to delete: ";
    cin >> search_id;

    ifstream infile("students.txt");
    ofstream tempfile("temp.txt");
    string line;
    while(getline(infile, line)) {
        if(line.empty()) continue;
        string id, s_name, s_contact, f_name, f_contact, rtype, foodType;
        int bed;
        parseStudentRecord(line, id, s_name, s_contact, f_name, f_contact, rtype, bed, foodType);
        if (id == search_id) {
            found = true;
            if (rtype == "Single")
                singleRoomOccupancy[bed-1] = 0;
            else if (rtype == "Double")
                doubleRoomOccupancy[bed-1] = 0;
            else if (rtype == "Triple")
                tripleRoomOccupancy[bed-1] = 0;
            // skip writing this record (deleting)
        } else {
            tempfile << line << "\n";
        }
    }

    infile.close();
    tempfile.close();

    std::remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        cout << "Student deleted successfully!\n";
    else
        cout << "Student not found!\n";
}

void hostel::food()
{
    system("cls");
    int type, m;

    cout << "\n\t*** HOSTEL WEEKLY MENU ***\n";
    cout << "1. Veg Menu\n";
    cout << "2. Non-Veg Menu\n";
    cout << "3. Exit\n";
    cout << "\nEnter your choice: ";
    cin >> type;

    if(type == 3)
    {
        cout << "Exiting program. Have a nice day!\n";
        return;
    }
    cout << "\nSelect Day:\n";
    cout << "1. Sunday\n";
    cout << "2. Monday\n";
    cout << "3. Tuesday\n";
    cout << "4. Wednesday\n";
    cout << "5. Thursday\n";
    cout << "6. Friday\n";
    cout << "7. Saturday\n";

    cout << "\nEnter the day number (1-7): ";
    cin >> m;

    if(type == 1)   
    {
        switch(m)
        {
            case 1:
                cout << "Sunday Veg Menu:\n";
                cout << "Breakfast: Paratha, Pickle, Tea\n";
                cout << "Lunch: Veg Biryani, Salad\n";
                cout << "Dinner: Chapati, Mixed Vegetable\n";
                break;

            case 2:
                cout << "Monday Veg Menu:\n";
                cout << "Breakfast: Oatmeal, Tea\n";
                cout << "Lunch: Rice, Dal, Vegetable Curry\n";
                cout << "Dinner: Chapati, Paneer Butter Masala\n";
                break;

            case 3:
                cout << "Tuesday Veg Menu:\n";
                cout << "Breakfast: Poha, Coffee\n";
                cout << "Lunch: Rice, Sambar, Curd\n";
                cout << "Dinner: Paratha, Mixed Vegetable\n";
                break;

            case 4:
                cout << "Wednesday Veg Menu:\n";
                cout << "Breakfast: Bread, Jam, Milk\n";
                cout << "Lunch: Rice, Rajma, Salad\n";
                cout << "Dinner: Chapati, Chole\n";
                break;

            case 5:
                cout << "Thursday Veg Menu:\n";
                cout << "Breakfast: Upma, Tea\n";
                cout << "Lunch: Rice, Vegetable Curry\n";
                cout << "Dinner: Paratha, Yogurt\n";
                break;

            case 6:
                cout << "Friday Veg Menu:\n";
                cout << "Breakfast: Idli, Sambar\n";
                cout << "Lunch: Rice, Veg Curry, Salad\n";
                cout << "Dinner: Chapati, Dal Makhani\n";
                break;

            case 7:
                cout << "Saturday Veg Menu:\n";
                cout << "Breakfast: Pancakes, Juice\n";
                cout << "Lunch: Rice, Vegetable Curry, Raita\n";
                cout << "Dinner: Noodles, Soup\n";
                break;

            default:
                cout << "Invalid day selection.\n";
        }
    }
    else if(type == 2)   
    {
        switch(m)
        {
            case 1:
                cout << "Sunday Non-Veg Menu:\n";
                cout << "Breakfast: Paratha, Pickle, Tea\n";
                cout << "Lunch: Chicken Biryani\n";
                cout << "Dinner: Egg Curry\n";
                break;

            case 2:
                cout << "Monday Non-Veg Menu:\n";
                cout << "Breakfast: Oatmeal, Tea\n";
                cout << "Lunch: Chicken Curry\n";
                cout << "Dinner: Omelette\n";
                break;

            case 3:
                cout << "Tuesday Non-Veg Menu:\n";
                 cout << "Breakfast: Poha, Coffee\n";
                cout << "Lunch: Fish Curry\n";
                cout << "Dinner: Egg Bhurji\n";
                break;

            case 4:
                cout << "Wednesday Non-Veg Menu:\n";
                cout << "Breakfast: Bread, Jam, Milk\n";
                cout << "Lunch: Chicken Roast\n";
                cout << "Dinner: Egg Curry\n";
                break;

            case 5:
                cout << "Thursday Non-Veg Menu:\n";
                 cout << "Breakfast: Upma, Tea\n";
                cout << "Lunch: Fish Fry\n";
                cout << "Dinner: Chicken Curry\n";
                break;

            case 6:
                cout << "Friday Non-Veg Menu:\n";
                cout << "Breakfast: Idli, Sambar\n";
                cout << "Lunch: Chicken Curry\n";
                cout << "Dinner: Egg Fried Rice\n";
                break;

            case 7:
                cout << "Saturday Non-Veg Menu:\n";
                cout << "Breakfast: Pancakes, Juice\n";
                cout << "Lunch: Mutton Curry\n";
                cout << "Dinner: Chicken Noodles\n";
                break;

            default:
                cout << "Invalid day selection.\n";
        }
    }
    else
    {
        cout << "Invalid menu type selection.\n";
    }
}
void hostel::laundary() 
{
    system("cls");
    int l;
    do 
    {
     cout<<"\n\t** LAUNDRY MENU **\n";
     cout<<"\t\t1. VIEW LAUNDRY\n\t\t2. UPDATE RATE\n\t\t3. EXIT\nEnter Choice: ";
     cin>>l;
     switch(l) 
     {
        case 1: viewlaundaryAdmin(); break;
        case 2: updatelaundary(); break;
     }
    } 
    while(l!=3);
}

void hostel::laundaryStudent() 
{
    system("cls");
    int l;
    do 
    {
        cout<<"\n\t** STUDENT LAUNDRY MENU **\n";
        cout<<"\t\t1. ADD LAUNDRY\n";
        cout<<"\t\t2. VIEW LAUNDRY\n";
        cout<<"\t\t3. EXIT\n";
        cout<<"Enter Choice: ";
        cin>>l;

        switch(l) 
        {
            case 1: addlaundary(); break;
            case 2: viewlaundaryStudent(); break;
        }

    } 
    while(l!=3);
}

void hostel::addlaundary() 
{
    int clothes;

    if(loggedInUser == "")
    {
        cout << "Error: No student logged in!\n";
        return;
    }
    cout << "\nEnter number of Clothes: ";
    cin >> clothes;

ofstream file("laundry.txt", ios::app);

if(!file)
{
    cout << "ERROR: Cannot create laundry.txt\n";
    return;
}
    file << loggedInID << " " << clothes << endl;

    file.close();

    cout << "Laundry added successfully!\n";
}

void hostel::viewlaundaryStudent()
{
    ifstream file("laundry.txt");
    
    if(!file)
    {
        cout << "No laundry records found.\n";
        return;
    }

    string id;
    int clothes;
    bool found = false;

    cout << "\n--- YOUR LAUNDRY RECORDS ---\n";

    while(file >> id >> clothes)
    {
        if(id == loggedInID)
        {
            cout << "Clothes: " << clothes << endl;
            found = true;
        }
    }

    if(!found)
    {
        cout << "No laundry records found for you.\n";
    }

    file.close();
}
void hostel::viewlaundaryAdmin()
{
    ifstream file("laundry.txt");

    if(!file)
    {
        cout << "No laundry records found.\n";
        return;
    }
    string id;
    int clothes;
    cout << "\n--- ALL LAUNDRY RECORDS (ADMIN) ---\n";

    while(file >> id >> clothes)
    {
        cout << "Student ID: " << id << " | Clothes: " << clothes << endl;
    }
    file.close();
}
void hostel::updatelaundary()
{
    cout << "\nEnter new rate: "; cin >> ratePerCloth;
}

void hostel::viewAllStudentFees()
{
    system("cls");

    ifstream studentFile("students.txt");

    if(!studentFile)
    {
        cout << "Cannot open students.txt\n";
        return;
    }

    string id, name, contact, fname, fcontact, rtype, foodType;
    int bed;

    cout << "\n============================================";
    cout << "\n        ALL STUDENT FEES & DUES";
    cout << "\n============================================\n";

    string line;
    while(getline(studentFile, line))
    {
        if(line.empty()) continue;
        parseStudentRecord(line, id, name, contact, fname, fcontact, rtype, bed, foodType);
        int roomPrice = 0;
        int foodFee = 0;
        int laundryTotal = 0;

        if(rtype == "Single")
            roomPrice = 6000;
        else if(rtype == "Double")
            roomPrice = 5000;
        else if(rtype == "Triple")
            roomPrice = 4000;

        if(foodType == "Veg")
            foodFee = 2500;
        else if(foodType == "NonVeg")
            foodFee = 3500;
        else
            foodFee = 0;

        ifstream laundryFile("laundry.txt");
        string lid;
        int clothes;
        while(laundryFile >> lid >> clothes)
        {
            if(lid == id)
                laundryTotal += clothes;
        }
        laundryFile.close();

        int laundryFee = laundryTotal * ratePerCloth;

        int totalDue = roomPrice + foodFee + laundryFee;

        // sum payments
        int paid = 0;
        ifstream payFile("payments.txt");
        if(payFile)
        {
            int amt; string pid;
            while(payFile >> pid >> amt)
            {
                if(pid == id) paid += amt;
            }
            payFile.close();
        }

        int netDue = totalDue - paid;
        if(netDue < 0) netDue = 0;

        cout << "Student ID : " << id << endl;
        cout << "Name       : " << name << endl;
        cout << "Room Type  : " << rtype << endl;
        cout << "Room Fee   : Rs. " << roomPrice << endl;
        cout << "Food Fee   : Rs. " << foodFee << endl;
        cout << "Laundry    : Rs. " << laundryFee << endl;
        cout << "Paid       : Rs. " << paid << endl;
        cout << "TOTAL DUE  : Rs. " << netDue << endl;
        cout << "--------------------------------------\n";
    }

    studentFile.close();
}

void hostel::recordPayment()
{
    string id;
    int amount;
    cout << "\n--- RECORD STUDENT PAYMENT ---\n";
    cout << "Enter Student ID: ";
    cin >> id;
    cout << "Enter payment amount: Rs. ";
    cin >> amount;

    ofstream payFile("payments.txt", ios::app);
    if(!payFile)
    {
        cout << "Error: Cannot open payments.txt for writing.\n";
        return;
    }
    payFile << id << " " << amount << endl;
    payFile.close();
    cout << "Payment recorded successfully.\n";
}

void hostel::showRoomAvailability()
{
    int singleAvailable = 0;
    int doubleAvailable = 0;
    int tripleAvailable = 0;

    for(int i = 0; i < singleBeds; i++)
    {
        if(singleRoomOccupancy[i] == 0)
            singleAvailable++;
    }
    for(int i = 0; i < doubleBeds; i++)
    {
        if(doubleRoomOccupancy[i] == 0)
            doubleAvailable++;
    }
    for(int i = 0; i < tripleBeds; i++)
    {
        if(tripleRoomOccupancy[i] == 0)
            tripleAvailable++;
    }
    cout << "\n--- ROOM AVAILABILITY ---\n";
    cout << "Single Beds Available : " << singleAvailable << " / " << singleBeds << endl;
    cout << "Double Beds Available : " << doubleAvailable << " / " << doubleBeds << endl;
    cout << "Triple Beds Available : " << tripleAvailable << " / " << tripleBeds << endl;
}

void hostel::loadRoomOccupancy()
{
    ifstream file("students.txt");
    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;
        string id, name, contact, fname, fcontact, rtype, foodType;
        int bed;
        parseStudentRecord(line, id, name, contact, fname, fcontact, rtype, bed, foodType);
        if(rtype == "Single")
            singleRoomOccupancy[bed-1] = 1;
        else if(rtype == "Double")
            doubleRoomOccupancy[bed-1] = 1;
        else if(rtype == "Triple")
            tripleRoomOccupancy[bed-1] = 1;
    }
    file.close();
}

void hostel::roomTypeDetails() 
{
    cout << "\nSingle: Rs.6000\nDouble: Rs.5000\nTriple: Rs.4000\n";
}

string hostel::generateStudentID()
{
    ifstream fin("id_counter.txt");
    int last = 1000;

    if(fin)
    {
        fin >> last;
    }
    fin.close();
    int newID = last + 1;
    ofstream fout("id_counter.txt", ios::trunc);
    fout << newID;
    fout.close();
    return "STU" + to_string(newID);
}

int main() 
{
    hostel h;
    h.loadRoomOccupancy();
    int choice;
    do 
    {
        cout << "\n1. Admin Login";
        cout << "\n2. Student Login";
        cout << "\n3. Exit";
        cout << "\nEnter Choice: ";
        cin >> choice;
        if (choice == 1)
            h.adminLogin();
        else if (choice == 2)
            h.studentLogin();
    } 
    while(choice != 3);
    return 0;
}
// Helper to parse a pipe-delimited student record:
// id|name|contact|family_name|family_contact|roomtype|bed|foodType
void parseStudentRecord(const string &line, string &id, string &name, string &contact,
                        string &fname, string &fcontact, string &rtype, int &bed, string &foodType)
{
    // Pipe-delimited format (canonical): id|name|contact|fname|fcontact|rtype|bed|foodType
    if(line.find('|') != string::npos) {
        string tmp;
        stringstream ss(line);
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, contact, '|');
        getline(ss, fname, '|');
        getline(ss, fcontact, '|');
        getline(ss, rtype, '|');
        getline(ss, tmp, '|');
        try { bed = stoi(tmp); } catch(...) { bed = 0; }
        getline(ss, foodType, '|');
        return;
    }

    // Legacy space-separated format: handles names with spaces by finding contacts
    vector<string> toks;
    string t; stringstream ss2(line);
    while(ss2 >> t) toks.push_back(t);
    
    if(toks.size() < 8) {
        id = name = contact = fname = fcontact = rtype = foodType = ""; bed = 0; return;
    }
    
    id = toks[0];
    int i_contact = findFirstContact(toks, 1);  // first contact (student)
    if(i_contact == -1) { 
        id = line; name = contact = fname = fcontact = rtype = foodType = ""; bed = 0; return;
    }
    
    // Assemble student name from tokens before first contact
    name = "";
    for(int k = 1; k < i_contact; ++k) { 
        if(!name.empty()) name += ' '; 
        name += toks[k]; 
    }
    contact = toks[i_contact];
    
    int j_contact = findFirstContact(toks, i_contact + 1);  // second contact (family)
    if(j_contact == -1) { 
        fname = fcontact = rtype = foodType = ""; bed = 0; return; 
    }
    
    // Assemble family name from tokens between contacts
    fname = "";
    for(int k = i_contact + 1; k < j_contact; ++k) { 
        if(!fname.empty()) fname += ' '; 
        fname += toks[k]; 
    }
    fcontact = toks[j_contact];
    
    // Parse remaining fields: roomtype, bed, foodType
    if(j_contact + 3 <= (int)toks.size() - 1) {
        rtype = toks[j_contact + 1];
        try { bed = stoi(toks[j_contact + 2]); } catch(...) { bed = 0; }
        foodType = toks[j_contact + 3];
    } else {
        rtype = ""; bed = 0; foodType = "";
    }
}

string makeStudentRecord(const string &id, const string &name, const string &contact,
                         const string &fname, const string &fcontact, const string &rtype,
                         int bed, const string &foodType)
{
    stringstream ss;
    ss << id << '|' << name << '|' << contact << '|' << fname << '|' << fcontact << '|' << rtype << '|' << bed << '|' << foodType;
    return ss.str();
}