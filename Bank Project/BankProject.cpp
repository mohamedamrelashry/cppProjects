
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
struct stUser {
    string username;
    string password;
    int permission; 
    bool MarkForDelete = false;
};
stUser CurrentUser;


enum enMainMenuePermissions{ 
    eAll = -1, pListClients = 1, 
    pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32,
    pManageUsers = 64 };
enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogOut = 8
};
enum enMangaeMenueOptions
{
    eListUsers = 1, eAddNewUser = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eMainMenue = 6
};
enum enTransactionManueScreen {
    eDeposit = 1, eWithDraw = 2,
    eTotalBalance = 3, MainMeneuScreen = 4
};


void ShowFindUserScreen();
void ManageUsersMenuScreen();
void ShowMainMenue();
void ShowAccessDeniedMessage();
void ShowAllClientsScreen();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void ShowUpdateUserScreen();
void Login();

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUserRecordLine(stUser User)
{
    cout << "| " << setw(15) << left << User.username;
    cout << "| " << setw(10) << left << User.password;
    cout << "| " << setw(40) << left << User.permission;
   
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients)) { ShowAccessDeniedMessage(); return; }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}
bool CheckAccessPermission(enMainMenuePermissions Permission) {
   if (CurrentUser.permission == enMainMenuePermissions::eAll)
       return true;
   if ((Permission & CurrentUser.permission) == Permission)
       return true;
   else
       return false; 
 }
void ShowAccessDeniedMessage() { 
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n"; }

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
        ////
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient)) { ShowAccessDeniedMessage(); return; }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients)) { ShowAccessDeniedMessage(); return; }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}


void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient)) { ShowAccessDeniedMessage(); return; }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient)) { ShowAccessDeniedMessage(); return; }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

bool DepositClientByAccountNumber(string AccountNumber, vector <sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        int AmountDeposit = 0;
        PrintClientCard(Client);
        cout << "please, enter deposit amount?";
        cin >> AmountDeposit;
        cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance += AmountDeposit;
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nTransaction Made Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}
void ShowDepositScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDepoist Screen\n";
    cout << "===========================================\n";
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DepositClientByAccountNumber(AccountNumber, vClients);
}
bool WithDrawClientByAccountNumber(string AccountNumber, vector <sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        int AmountDeposit = 0;
        PrintClientCard(Client);
        cout << "please, enter WithDraw amount?";
        cin >> AmountDeposit;
        cout << "\n\nAre you sure you want to perform this transaction? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    while (!(C.AccountBalance >= AmountDeposit)) {
                        cout << "You can not draw this amount of money" << endl;
                        cout << "enter amount less than " << C.AccountBalance << endl;
                        cin >> AmountDeposit;
                    }
                    C.AccountBalance -= AmountDeposit;
                    break;

                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nTransaction Made Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

void ShowWithDrawScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tWithDraw Screen\n";
    cout << "===========================================\n";
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    WithDrawClientByAccountNumber(AccountNumber, vClients);
}

short ReadTransactionMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void LineToOneRecored(sClient Client) {
    cout << "| " << setw(20) << left << Client.AccountNumber;
    cout << "| " << setw(60) << left << Client.Name;
    cout << "| " << setw(40) << left << Client.AccountBalance;
}
void ShowTotalBalance() {
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(20) << "Accout Number";
    cout << "| " << left << setw(60) << "Client Name";
    cout << "| " << left << setw(40) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    double TotoalBalance = 0;
    for (sClient Client : vClients)
    {
        LineToOneRecored(Client);
        TotoalBalance += (Client.AccountBalance);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "\t\t\t Total Balance is : " << TotoalBalance;


}


void GoBackToTransactionMenueOption()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransaction Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] eDeposit.\n";
    cout << "\t[2] eWithDraw.\n";
    cout << "\t[3] eTotalBalance.\n";
    cout << "\t[4] Main Meneu Screen.\n";
    cout << "===========================================\n";
    switch ((enTransactionManueScreen)ReadTransactionMenueOption())
    {
    case eDeposit: {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenueOption();
        break;

    }
    case eWithDraw: {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionMenueOption();
        break;

    }
    case eTotalBalance:
        system("cls");
        ShowMainMenue();
        GoBackToTransactionMenueOption();
        break;
    case MainMeneuScreen:
        system("cls");
        ShowMainMenue();
        break;
    default:
        break;
    }
}

void PerfromTransactionMenueOption(enTransactionManueScreen TransactionOption) {
    switch (TransactionOption)
    {
    case eDeposit: {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenueOption();
        break;

    }
    case eWithDraw: {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionMenueOption();
        break;

    }
    case eTotalBalance:
        system("cls");
        ShowTotalBalance();
        GoBackToTransactionMenueOption();
        break;
    case MainMeneuScreen:
        system("cls");
        ShowMainMenue();
        break;
    default:
        break;
    }
}
void ShowTransactionMenueScreen() {
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions)) { ShowAccessDeniedMessage(); return; }
    cout << "===========================================\n";
    cout << "\t\tTransaction Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] eDeposit.\n";
    cout << "\t[2] eWithDraw.\n";
    cout << "\t[3] eTotalBalance.\n";
    cout << "\t[4] Main Meneu Screen.\n";
    cout << "===========================================\n";
    PerfromTransactionMenueOption((enTransactionManueScreen)ReadTransactionMenueOption());

}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
void GoBackToManageMenue()
{
    cout << "\n\nPress any key to go back to Manage Menue...";
    system("pause>0");
    ManageUsersMenuScreen();

}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadManageMenueOption()
{
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransactionMenueScreen();
        break;
    case enMainMenueOptions::eManageUsers:
        system("cls");
        ManageUsersMenuScreen();
        break;
    case enMainMenueOptions::eLogOut:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transaction processes \n";
    cout << "\t[7] Manage Users \n";
    cout << "\t[8] LogOut.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void LoginScreen() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tlogin screen\n";
    cout << "===========================================\n";
}

stUser AskUserPassword() {
    stUser user; 
    
    cout << "Enter your UserName:\n";
    cin >> user.username; 
    cout << "Enter you Password:\n";
    cin >> user.password;
    return user; 

}

stUser convertLinetoRecord(string Line, string Seperator = "#//#")
{
    stUser user;
    vector<string> vUserData;
    vUserData= SplitString(Line, Seperator);

    user.username = vUserData[0];
    user.password = vUserData[1];
    user.permission = stoi(vUserData[2]);
    return user;
}

string ConvertRecordToLine(stUser user, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += user.username + Seperator;
    stClientRecord += user.password + Seperator;
    stClientRecord += to_string(user.permission);
    return stClientRecord;
}

vector <stUser> LoadDataFromFile(string FileName)
{
    vector <stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {
            User = convertLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

bool IsUserFound(stUser& user , vector <stUser> vUsers) {
    for (stUser U : vUsers)
    { 

        if (   U.username == user.username 
            && U.password == user.password)
        {
            user = U;
            return true;
        }

    }
    return false;

}

void Login() {
    bool found = false;
    LoginScreen();
    while (!found) {
        stUser TempUser = AskUserPassword();
        found = IsUserFound(TempUser, LoadDataFromFile(UsersFileName));
        if (found) {
            CurrentUser = TempUser;
        }
        else {
            cout << "Invalid username/Password" << endl;
        }
    }
    ShowMainMenue();

}

void ShowAllUsersScreen()
{
    vector <stUser> vUser = LoadDataFromFile(UsersFileName);
    system("cls");
    cout << "\n\t\t\t\t\tClient List (" << vUser.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permission";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vUser.size() == 0)
        cout << "\t\t\t\tNo USERS  Available In the System!";
    else

        for (stUser user : vUser)
        {
            PrintUserRecordLine(user);
            cout << endl;
        }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}


bool UserExistByUsername(stUser user,vector <stUser> vUsers) {
    for (stUser U : vUsers)
    { 

        if (   U.username == user.username )
        {
            return true;
        }

    }
    return false;
}


int ReadUserPermission() {
    int Permissions = 0;
    char Answer = 'n';
    enMainMenuePermissions p; 
    char ans; 
    cout << "you want to give full access? y/n? n"; 
    cin >> ans; 
    if (ans == 'y') {
        return -1;
    }
    cout << "\nDo you want to give access to : \n ";
    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pListClients;
    }
    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') 
    { Permissions += enMainMenuePermissions::pAddNewClient;
    }     
    cout << "\nDelete Client? y/n? ";
    cin >> Answer; 
    if (Answer == 'y' || Answer == 'Y') 
    { Permissions += enMainMenuePermissions::pDeleteClient; 
    }
    cout << "\nUpdate Client? y/n? ";
    cin >> Answer; 
    if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pUpdateClients; 
    }
    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') { 
        Permissions += enMainMenuePermissions::pFindClient; 
    }     cout << "\nTransactions? y/n? ";
    cin >> Answer; 
    if (Answer == 'y' || Answer == 'Y') 
    {
        Permissions += enMainMenuePermissions::pTranactions;
    }
    cout << "\nManage Users? y/n? ";
    cin >> Answer; if (Answer == 'y' || Answer == 'Y')
    {
        Permissions += enMainMenuePermissions::pManageUsers;
    }
    return Permissions;
}


stUser ReadNewUser() {
    stUser user; 
    cout << "Enter your user Name: ";
    getline(cin >> ws, user.username);
    while (UserExistByUsername(user, LoadDataFromFile(UsersFileName))) {
        cout << "\nUser with ['I " << user.username << "] already,exist";
            getline(cin >> ws, user.username);

    }
    cout << "Enter Password? ";
    getline(cin, user.password);
  
    user.permission= ReadUserPermission();
    return user;

}

void AddNewUser()
{
    stUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine(User));
}


void PrintUserCard(stUser Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nusername: " << Client.username<<endl;
    cout<< "\nPassword:" << Client.password<<endl;
    cout << "\npermission:" << Client.permission<<endl;

}

bool MarkUserForDeleteByAccountNumber(stUser User, vector <stUser>& vUsers)
{

    for (stUser& C : vUsers)
    {

        if (C.username == User.username)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUser C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

bool DeleteUserByUserName(stUser User, vector <stUser>& vUsers)
{
    
    char Answer = 'n';

    if (UserExistByUsername(User, vUsers))
    {
        
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByAccountNumber(User, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadDataFromFile(UsersFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << User.username << ") is Not Found!";
        return false;
    }
}

void ShowDeleteUsertScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = LoadDataFromFile(UsersFileName);
    string UserName = ReadClientAccountNumber();
    stUser user;
    user.username = UserName;
    DeleteUserByUserName(user, vUsers);
}

stUser ChangeUserRecord(stUser User) {
     
    cout << "\n\nEnter Password? "; 
    getline(cin >> ws, User.password);
    User.permission = ReadUserPermission();
    return User;
}


bool UpdateClientByAccountNumber(stUser user, vector <stUser>& vUser)
{

    char Answer = 'n';

    if (UserExistByUsername(user,vUser))
    {

        PrintUserCard(user);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUser& C : vUser)
            {
                if (C.username == user.username)
                {
                    C = ChangeUserRecord(C);
                    break;
                }
            }

            SaveUsersDataToFile(UsersFileName, vUser);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << user.username << ") is Not Found!";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    stUser user;
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Info Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vClients = LoadDataFromFile(UsersFileName);
    string AccountNumber = ReadClientAccountNumber();
    user.username = AccountNumber;
    UpdateClientByAccountNumber(user, vClients);
}


void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUser = LoadDataFromFile(UsersFileName);
    stUser user;
    string AccountNumber = ReadClientAccountNumber();
    user.username = AccountNumber;

    if (UserExistByUsername(user, vUser))
  
        PrintUserCard(user);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}
void PerfromManageMenueOption(enMangaeMenueOptions options){
    switch (options)
    {
    case eListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageMenue();
        break;
    case eAddNewUser:
        system("cls");
        AddNewUser();
        GoBackToManageMenue();
        break;
    case eDeleteUser:
        system("cls");
        ShowDeleteUsertScreen();
        GoBackToManageMenue();
        break;
    case eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageMenue();
        break;
    case eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageMenue();

        break;
    case eMainMenue:
        
        ShowMainMenue();
        break;
    default:
        GoBackToManageMenue();
        break;
    }

}

void ManageUsersMenuScreen() {
    system("cls");
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers)) { ShowAccessDeniedMessage(); return; }
    cout << "===========================================\n";
    cout << "\t\tMain Manage Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show users List.\n";
    cout << "\t[2] Add New user.\n";
    cout << "\t[3] Delete user.\n";
    cout << "\t[4] Update user Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Main Menue. \n";
    
    cout << "===========================================\n";
    PerfromManageMenueOption(enMangaeMenueOptions(ReadManageMenueOption()));
}

int main()

{
    Login();
    system("pause>0");
    return 0;
}
