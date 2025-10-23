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
    double  AccountBalance;
    bool MarkForDelete = false;
};

struct sUser
{
    string UserName;
    string Password;
    short Permissions;
    bool MarkForDelete = false;

};

sUser CrruntUser;

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6,
    eManageUser = 7, eLogout = 8
};

enum class enManageUsersMenueOptions
{
    eListUsers = 1, eAddNewUsers = 2,
    eDeleteUsers = 3, eUpdateUsers = 4,
    eFindUsers = 5, eMainMenue = 6
};

enum enTransactionsMenueOptions
{
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenue = 4
};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();
void login();

vector <string> SplitString(string S1, string delim)
{
    vector <string> vString;

    short pos = 0;
    string sWord;

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);

        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Client;

    vector <string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> Clients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            Clients.push_back(Client);
        }

        MyFile.close();
    }

    return Clients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CrruntUser.Permissions == enMainMenuePermissions::eAll)
        return true;

    if ((CrruntUser.Permissions & Permission) == Permission)
        return true;
    else
        return false;
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }


    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

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

    for (sClient client : vClients)
    {
        PrintClientRecordLine(client);
        cout << endl;
    }
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
            Client = ConvertLineToRecord(Line);
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
    getline(cin >> ws, Client.AccountNumber); // Usage of std::ws will extract allthe whitespace character

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pincode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

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

void AddDataLineToFile(string FileName, string LineClientData)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << LineClientData << endl;

        MyFile.close();

    }
}

void AddNewClient()
{
    vector <sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    Client = ReadNewClient();

    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'y';

    do
    {
        cout << "Adding New Client:\n\n";

        AddNewClient();

        cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClient, sClient& Client)
{
    for (sClient& c : vClient)
    {
        if (c.AccountNumber == AccountNumber)
        {
            Client = c;
            return true;
        }
    }
    return false;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& c : vClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.MarkForDelete = true;
            return true;

        }
    }
    return false;
}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClient)
{
    fstream MyFile;
    string DataLine;

    MyFile.open(FileName, ios::out);//overwrite

    if (MyFile.is_open())
    {
        for (sClient& c : vClient)
        {
            if (c.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(c);
                MyFile << DataLine << endl;
            }

        }
        MyFile.close();
    }

    return vClient;


}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to delete this client ? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            //Refresh Clients
            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
        }
        return true;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter Pincode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
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
            for (sClient& c : vClients)
            {
                if (c.AccountNumber == AccountNumber)
                {
                    c = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
        }
        return true;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perform this transaction? y/n ?";
    cin >> Answer;

    if (Answer == 'Y' || Answer == 'y')
    {
        for (sClient& c : vClients)
        {
            if (c.AccountNumber == AccountNumber)
            {
                c.AccountBalance += Amount;

                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << c.AccountBalance;
                return true;
            }
        }
        return false;
    }
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\n\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void PrintClientRecordBalanceLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalances()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
        for (sClient c : vClients)
        {
            TotalBalances += c.AccountBalance;
            PrintClientRecordBalanceLine(c);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOptions)
{
    switch (TransactionsMenueOptions)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eMainMenue:
    {
        ShowMainMenue();
        break;
    }
    }
}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

sUser ConvertLineToRecord_User(string Line, string Seperator = "#//#")
{
    sUser User;

    vector <string> vUserData;
    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

vector <sUser> LoadUsersDataFromFile_User(string FileName)
{
    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToRecord_User(Line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }

    return vUsers;
}

bool _User_ExistsByUserName(string UserName, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineToRecord_User(Line);
            if (User.UserName == UserName)
            {
                MyFile.close();
                return true;
            }
        }

        MyFile.close();

    }
    return false;
}

int GetPermissions()
{
    char Answer = 'n';
    int UserPermissions = 0;

    cout << "Do you want to give full access? y/n? ";
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to :";

    cout << "\nShow Clients? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pListClients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        UserPermissions = UserPermissions | enMainMenuePermissions::pManageUsers;
    }

    return UserPermissions;

}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username? ";
    getline(cin >> ws, User.UserName); // Usage of std::ws will extract all the whitespace character

    while (_User_ExistsByUserName(User.UserName, UsersFileName))
    {
        cout << "\nUser with UserName [" << User.UserName << "] already exists, Enter another UserName? ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = GetPermissions();

    return User;
}

string ConvertRecordToLine_User(sUser User, string Seperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

void AddNewUser()
{
    sUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertRecordToLine_User(User));
}

void AddNewUsers()
{
    char AddMore = 'y';

    do
    {
        cout << "Adding New User:\n\n";

        AddNewUser();

        cout << "\nUser Added Successfully, do you want to add more Users ? Y / N ? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');
}

void ShowAddNewUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Users Screen";
    cout << "\n-----------------------------------\n";

    AddNewUsers();
}

void Print_User_RecordLine(sUser Users)
{
    cout << "| " << setw(18) << left << Users.UserName;
    cout << "| " << setw(10) << left << Users.Password;
    cout << "| " << setw(10) << left << Users.Permissions;
}

void ShowAllUsersScreen()
{
    vector <sUser> vUsers = LoadUsersDataFromFile_User(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(18) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(10) << "Permissions";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sUser Users : vUsers)
    {
        Print_User_RecordLine(Users);
        cout << endl;
    }
}

bool FindUserByUserName(string UserName, vector <sUser> vUsers, sUser& User)
{
    for (sUser& u : vUsers)
    {
        if (u.UserName == UserName)
        {
            User = u;
            return true;
        }
    }
    return false;
}

void Print_User_Card(sUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "\n-----------------------------------\n";
    cout << "\nUserName: " << User.UserName;
    cout << "\nPassword : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";


}

bool Mark_User_ForDeleteByUserName(string UserName, vector <sUser>& vUser)
{
    for (sUser& u : vUser)
    {
        if (u.UserName == UserName)
        {
            u.MarkForDelete = true;
            return true;

        }
    }
    return false;
}

vector <sUser> SaveClientsDataToFile(string FileName, vector <sUser> vUser)
{
    fstream MyFile;
    string DataLine;

    MyFile.open(FileName, ios::out);//overwrite

    if (MyFile.is_open())
    {
        for (sUser& u : vUser)
        {
            if (u.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine_User(u);
                MyFile << DataLine << endl;
            }

        }
        MyFile.close();
    }

    return vUser;


}

bool DeleteClientByAccountNumber(string UserName, vector <sUser>& vUser)
{

    if (UserName == "Admin")
    {
        cout << "You can Not delete this user";

        return false;
    }


    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User))
    {
        Print_User_Card(User);

        cout << "\n\nAre you sure you want to delete this user ? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            Mark_User_ForDeleteByUserName(UserName, vUser);
            SaveClientsDataToFile(UsersFileName, vUser);

            //Refresh Clients
            vUser = LoadUsersDataFromFile_User(UsersFileName);

            cout << "\n\nUser Deleted Successfully.";
        }
        return true;
    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
        return false;
    }
}

string ReadUserName()
{
    string AccountNumber = "";
    cout << "\nPlease enter User Name? ";
    cin >> AccountNumber;
    return AccountNumber;
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUsersDataFromFile_User(UsersFileName);
    string UserName = ReadUserName();

    DeleteClientByAccountNumber(UserName, vUser);
}

sUser ChangeClientRecord_User(string UserName)
{
    sUser User;

    User.UserName = UserName;

    cout << "Enter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = GetPermissions();


    return User;
}

bool UpdateClientByAccountNumber(string UserName, vector <sUser>& vUser)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUser, User))
    {
        Print_User_Card(User);

        cout << "\n\nAre you sure you want update this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sUser& u : vUser)
            {
                if (u.UserName == UserName)
                {
                    u = ChangeClientRecord_User(UserName);
                    break;
                }
            }

            SaveClientsDataToFile(UsersFileName, vUser);

            cout << "\n\nClient Updated Successfully.";
        }
        return true;
    }
    else
    {
        cout << "\nUser with UserName (" << UserName << ") is Not Found!";
        return false;
    }
}

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUsersDataFromFile_User(UsersFileName);
    string UserName = ReadUserName();

    UpdateClientByAccountNumber(UserName, vUser);

}

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    vector <sUser> vUser = LoadUsersDataFromFile_User(UsersFileName);
    sUser User;
    string UserName = ReadUserName();


    if (FindUserByUserName(UserName, vUser, User))
    {
        Print_User_Card(User);
    }
    else
    {
        cout << "\nUser with Username[" << UserName << "] is not found!";

    }
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenue();
}

short ReadManageUsersMenueOption()
{
    short choise = 0;
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> choise;

    return choise;
}

void PerfromManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOptions)
{
    switch (ManageUsersMenueOptions)
    {
    case enManageUsersMenueOptions::eListUsers:
    {
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eAddNewUsers:
    {
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eDeleteUsers:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eUpdateUsers:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eFindUsers:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    }

    case enManageUsersMenueOptions::eMainMenue:
    {
        ShowMainMenue();
        break;
    }
    }
}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Users List.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}

short ReadMainMenueOption()
{
    short choise = 0;
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> choise;

    return choise;
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
    {
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eTransactions:
    {
        system("cls");
        ShowTransactionsMenue();
        break;
    }

    case enMainMenueOptions::eManageUser:
    {
        system("cls");
        ShowManageUsersMenue();
        break;
    }

    case enMainMenueOptions::eLogout:
    {
        system("cls");
        login();
        break;
    }
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
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

sUser ReadUser()
{
    sUser User;

    cout << "\n\nEnter Username? ";
    getline(cin, User.UserName);

    cout << "Enter Password? ";
    getline(cin, User.Password);


    return User;
}

bool FindUserByUsername(string UserName, string Password, sUser& User, vector <sUser> vUsers)
{
    for (sUser u : vUsers)
    {

        if (u.UserName == UserName && u.Password == Password)
        {
            User = u;
            return true;
        }

    }

    return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser& User)
{
    vector <sUser> vUsers = LoadUsersDataFromFile_User(UsersFileName);

    for (sUser u : vUsers)
    {

        if (u.UserName == Username && u.Password == Password)
        {
            User = u;
            return true;
        }

    }
    return false;
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CrruntUser))
        return true;
    else
        return false;
}

void login()
{
    bool LoginFaild = false;

    string Username, Password;

    do
    {
        system("cls");

        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;


        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    login();

    return 0;
}