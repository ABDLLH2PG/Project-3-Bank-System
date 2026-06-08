//Project #3, Bank System (V3);
//Due: In 2026/6/8

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

const string UsersFileName = "Users.txt";

enum enMainMenueOptions { enListClients = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enShowTransactionsMenue = 6, enShowManageUsersMenue = 7, enLogout = 8 };

enum enTransactionsMenueOptions { enDeposit = 1, enWithdraw = 2, TotalBalances = 3, MainMenue = 4 };

enum enManageUsersMenueOptions { enListUsers = 1, enAddNewUser = 2, enDeleteUser = 3, enUpdateUser = 4, enFindUser = 5, enMainMenue = 6 };

enum enMainMenuePermissions { pFullAccess = -1, pListClients = 1, pAddNewClinets = 2, pDeleteClients = 4, pUpdateClients = 8, pFindClinets = 16, pTranactions = 32, pManageUsers = 64 };

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;
};

struct stUser
{
	string UserName = "";
	string Password = "";
	int Permissions = 0;
	bool MarkForDelete = false;
};

stUser CurrentUser;

void Login();

void ShowMainMenue();

void ShowTransactionsMenue();

void ShowManageUsersMenue();

void GoBackToMainMenue();

void GoBackToManageUsersMenue();

string PrintPunctuations(char Punct, short NumberOfPunct)
{
	string sPunct = "";

	for (short i = 0; i < NumberOfPunct; i++)
	{
		sPunct += Punct;
	}

	return sPunct;
}

short ReadMainMenueOption()
{
	short Choose = 0;
	do
	{
		cout << "Choose What do you want to do? [1 to 8]? ";
		cin >> Choose;
	} while (!(Choose >= 1 && Choose <= 8));

	return Choose;
}

short ReadTransactionMenueOption()
{
	short Choose = 0;
	do
	{
		cout << "Choose what do you want to do? [1 to 4]? ";
		cin >> Choose;
	} while (!(Choose >= 1 && Choose <= 4));

	return Choose;
}

short ReadManageUsersMenueOption()
{
	short Choose = 0;
	do
	{
		cout << "Choose what do you want to do? [1 to 6]? ";
		cin >> Choose;
	} while (!(Choose >= 1 && Choose <= 6));

	return Choose;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;

}

string ReadUserName()
{
	string UserName = "";

	cout << "\nPlease enter Username? ";
	cin >> UserName;

	return UserName;
}

vector <string> SplitString(string S1, string Delim = " ")
{
	vector <string> vString;

	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	stClient Client;

	vector <string> vClientData;
	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]); //cast string to double

	return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";

	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
	stUser User;

	vector <string> vUserData;
	vUserData = SplitString(Line, Seperator);

	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	User.Permissions = stoi(vUserData[2]); // cast string to int

	return User;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
	string stUserRecord = "";

	stUserRecord += User.UserName + Seperator;
	stUserRecord += User.Password + Seperator;
	stUserRecord += to_string(User.Permissions);

	return stUserRecord;
}

vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in); //read Mode

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

vector <stClient> SaveClientsDataToFile(string FileName, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); //overWrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				// we only write records that are not marked for delete.
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vClients;
}

vector <stUser> LoadUsersDateFromFile(string FileName)
{
	vector <stUser> vUsers;

	fstream MyFile;
	MyFile.open(FileName, ios::in); //readMode

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			vUsers.push_back(User);
		}

		MyFile.close();
	}

	return vUsers;
}

vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser>& vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); //overWrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (stUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertUserRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vUsers;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

bool FindClientByAccountNumber(string AccountNumber, stClient& Client)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User)
{
	vector <stUser> vUsers = LoadUsersDateFromFile(UsersFileName);

	for (stUser U : vUsers)
	{
		if (U.UserName == Username && U.Password == Password)
		{
			User = U;
			return true;
		}
	}

	return false;
}

bool FindUserByUsername(string Username, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in); //read Mode

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);

			if (User.UserName == Username)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}

	return false;
}

bool FindUserByUsername(string UserName, vector <stUser>& vUsers, stUser& User)
{
	for (stUser& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in); //read Mode

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

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

void PrintClientRecord(stClient& Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void PrintClientRecordBalanceLine(stClient& Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

void PrintUserRecordLine(stUser& User)
{
	cout << "| " << left << setw(15) << User.UserName;
	cout << "| " << left << setw(12) << User.Password;
	cout << "| " << left << setw(15) << User.Permissions;
}

void PrintAllClientsData(vector <stClient>& vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";

	cout << "\n________________________________________________________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << PrintPunctuations('\t', 4) << "No Clients Available In the System!";
	}
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientRecord(Client);
			cout << endl;
		}
	}

	cout << "\n________________________________________________________________________________________________\n" << endl;
}

void PrintAllClientsBalances(vector <stClient>& vClients)
{
	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";

	cout << "\n________________________________________________________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n________________________________________________________________________________________________\n" << endl;

	double TotalBalances = 0.0;

	if (vClients.size() == 0)
	{
		cout << PrintPunctuations('\t', 4) << "No Clients Available In the System!";
	}
	else
	{
		for (stClient& Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;

			cout << endl;
		}
	}

	cout << "\n________________________________________________________________________________________________\n" << endl;

	cout << PrintPunctuations('\t', 5) << "    Total Balances = " << TotalBalances;
}

void PrintAllUsersData(vector <stUser>& vUsers)
{
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";

	cout << "\n________________________________________________________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(12) << "Password";
	cout << "| " << left << setw(15) << "Permissions";
	cout << "\n________________________________________________________________________________________________\n" << endl;

	for (stUser& User : vUsers)
	{
		PrintUserRecordLine(User);

		cout << endl;
	}

	cout << "\n________________________________________________________________________________________________\n" << endl;
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the Client details:\n";
	cout << PrintPunctuations('-', 35);
	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code      : " << Client.PinCode;
	cout << "\nName          : " << Client.Name;
	cout << "\nPhone         : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
}

void PrintUserCard(stUser User)
{
	cout << "\nThe following are the User details:\n";
	cout << PrintPunctuations('-', 35);
	cout << "\nUsername    : " << User.UserName;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
}

int ReadPermissionToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		return enMainMenuePermissions::pFullAccess;
	}

	cout << "\nDo you want to give access to :\n";

	cout << "\nshow Client List? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pListClients;
	}

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pAddNewClinets;
	}

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pDeleteClients;
	}

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pFindClinets;
	}

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pTranactions;
	}

	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions = Permissions | enMainMenuePermissions::pManageUsers;
	}

	return Permissions;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permissions == enMainMenuePermissions::pFullAccess)
		return true;

	if ((CurrentUser.Permissions & Permission) == Permission)
		return true;
	else
		return false;
}

stClient ReadNewClient()
{
	stClient Client;

	cout << "Enter Account Number? ";
	// Usage of std::we will extract all the witespace character
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter Another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Phone? ";
	getline(cin >> ws, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddNewClient()
{
	stClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddClients()
{
	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "Adding New Client:\n\n";

		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more Client? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Phone? ";
	getline(cin >> ws, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

stUser ReadNewUser()
{
	stUser User;

	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);

	while (FindUserByUsername(User.UserName, UsersFileName))
	{
		cout << "\User with [" << User.UserName << "] already exists, Enter Another UserName? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter Password? ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

void AddNewUser()
{
	stUser User;
	User = ReadNewUser();

	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding New User:\n\n";

		AddNewUser();
		cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

stUser ChangeUserRecord(string Username)
{
	stUser User;

	User.UserName = Username;

	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionToSet();

	return User;
}

bool MarkUserForDeleteByUserName(string UserName, vector <stUser>& vUsers)
{
	for (stUser& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteUserByUserName(string Username, vector <stUser>& vUsers)
{
	if (Username == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;
	}

	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(Username, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want delete this User? y/n ? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			MarkUserForDeleteByUserName(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);

			//Refresh Users
			vUsers = LoadUsersDateFromFile(UsersFileName);

			cout << "\n\nUser Deleted Successfully.";
			return true;
		}
		return false;
	}
	else
	{
		cout << "\nUser with Username (" << Username << ") is Not Found!";
		return false;
	}
}

bool UpdateUserByUserName(string UserName, vector <stUser>& vUsers)
{
	if (UserName == "Admin")
	{
		cout << "\n\nYou cannot Update This User.";
		return false;
	}

	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want update this User? y/n ? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			for (stUser& U : vUsers)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);

			cout << "\n\nUser Update Successfully.";
			return true;
		}
		return false;
	}
	else
	{
		cout << "\User with Username (" << UserName << ") is Not Found!";
		return false;
	}
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			//Refresh Clients
			vClients = LoadClientsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;

		if (tolower(Answer) == 'y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(ClientsFileName, vClients);

			cout << "\n\nClient Update Successfully.";
			return true;
		}
		return false;
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perform this transaction? y/n? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\nDone Successfully New Balance is: " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}
	return false;
}

void ShowAccessDeniedMessage()
{
	cout << "\n";
	cout << PrintPunctuations('-', 36) << "\n";
	cout << "Access Denied,\n";
	cout << "You dont Have Permission To Do this,\n";
	cout << "Please Conact Your Admin.\n";
	cout << PrintPunctuations('-', 36) << "\n";
}

void ShowClientListScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintAllClientsData(vClients);
}

void ShowAddNewClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClinets))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Add New Clients Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	AddClients();
}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Delete Client Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Update Client Info Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClinets))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Find Client Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	string AccountNumber = ReadClientAccountNumber();

	stClient Client;
	if (FindClientByAccountNumber(AccountNumber, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
	}
}

void ShowEndScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Program Ends :-)\n";
	cout << PrintPunctuations('-', 35) << "\n";
	system("pause>0");
}

void DepositScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Deposit Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	stClient Client;
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void WithdrawScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Withdraw Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	stClient Client;
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;

	//Validate that the amount does not exceeds the balance
	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance;
		cout << "\nPlease enter another amount? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}

void ShowBalancesListScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintAllClientsBalances(vClients);
}

void ShowListUsersScreen()
{
	vector <stUser> vUsers = LoadUsersDateFromFile(UsersFileName);

	PrintAllUsersData(vUsers);
}

void ShowAddNewUserScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Add New User Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	AddNewUsers();
}

void ShowDeleteUserScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Delete User Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stUser> vUser = LoadUsersDateFromFile(UsersFileName);
	string UserName = ReadUserName();

	DeleteUserByUserName(UserName, vUser);
}

void ShowUpdateUserScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Update Users Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stUser> vUser = LoadUsersDateFromFile(UsersFileName);
	string UserName = ReadUserName();

	UpdateUserByUserName(UserName, vUser);
}

void ShowFindUserScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Find User Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stUser> vUsers = LoadUsersDateFromFile(UsersFileName);

	string UserName = ReadUserName();

	stUser User;
	if (FindUserByUsername(UserName, vUsers, User))
	{
		PrintUserCard(User);
	}
	else
	{
		cout << "\nUser with Username (" << UserName << ") is Not Found!";
	}
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

void GoBackToManageUsersMenue()
{
	cout << "\n\nPress any key to go back to Manage Users Menue...";

	system("pause>0");
	ShowManageUsersMenue();
}

void PerformMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enMainMenueOptions::enListClients:
	{
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enAddNewClient:
	{
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enDeleteClient:
	{
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enUpdateClient:
	{
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enFindClient:
	{
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enShowTransactionsMenue:
	{
		system("cls");
		ShowTransactionsMenue();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enShowManageUsersMenue:
	{
		system("cls");
		ShowManageUsersMenue();
		GoBackToMainMenue();
		break;
	}

	case enMainMenueOptions::enLogout:
	{
		system("cls");
		//ShowEndScreen();
		Login();
		break;
	}
	}
}

void PerformTransactionMenueOption(enTransactionsMenueOptions enOptions)
{
	switch (enOptions)
	{
	case enTransactionsMenueOptions::enDeposit:
	{
		system("cls");
		DepositScreen();
		GoBackToTransactionsMenue();
		break;
	}

	case enTransactionsMenueOptions::enWithdraw:
	{
		system("cls");
		WithdrawScreen();
		GoBackToTransactionsMenue();
		break;
	}

	case enTransactionsMenueOptions::TotalBalances:
	{
		system("cls");
		ShowBalancesListScreen();
		GoBackToTransactionsMenue();
		break;
	}

	case enTransactionsMenueOptions::MainMenue:
	{
		ShowMainMenue();
		break;
	}
	}
}

void PerformManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption)
{
	switch (ManageUsersMenueOption)
	{
	case enManageUsersMenueOptions::enListUsers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUsersMenue();
		break;
	}

	case enManageUsersMenueOptions::enAddNewUser:
	{
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUsersMenue();
		break;
	}

	case enManageUsersMenueOptions::enDeleteUser:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenue();
		break;
	}

	case enManageUsersMenueOptions::enUpdateUser:
	{
		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageUsersMenue();
		break;
	}

	case enManageUsersMenueOptions::enFindUser:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenue();
		break;
	}

	case enManageUsersMenueOptions::enMainMenue:
	{
		system("cls");
		ShowMainMenue();
		break;
	}
	}
}

void ShowManageUsersMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Manage Users Menue Screen" << "\n";
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 1) << "[1] List Users." << "\n";
	cout << PrintPunctuations('\t', 1) << "[2] Add New User." << "\n";
	cout << PrintPunctuations('\t', 1) << "[3] Delete User." << "\n";
	cout << PrintPunctuations('\t', 1) << "[4] Update User." << "\n";
	cout << PrintPunctuations('\t', 1) << "[5] Find User." << "\n";
	cout << PrintPunctuations('\t', 1) << "[6] Main Menue." << "\n";
	cout << PrintPunctuations('=', 45) << "\n";

	PerformManageUsersMenueOption(enManageUsersMenueOptions(ReadManageUsersMenueOption()));
}

void ShowTransactionsMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Transactions Menue Screen" << "\n";
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 1) << "[1] Deposit." << "\n";
	cout << PrintPunctuations('\t', 1) << "[2] Withdraw." << "\n";
	cout << PrintPunctuations('\t', 1) << "[3] Total Balances." << "\n";
	cout << PrintPunctuations('\t', 1) << "[4] Main Menue." << "\n";
	cout << PrintPunctuations('=', 45) << "\n";

	PerformTransactionMenueOption(enTransactionsMenueOptions(ReadTransactionMenueOption()));
}

void ShowMainMenue()
{
	system("cls");
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 2) << "Main Menue Screen" << "\n";
	cout << PrintPunctuations('=', 45) << "\n";
	cout << PrintPunctuations('\t', 1) << "[1] Show Client List." << "\n";
	cout << PrintPunctuations('\t', 1) << "[2] Add New Client." << "\n";
	cout << PrintPunctuations('\t', 1) << "[3] Delete Client." << "\n";
	cout << PrintPunctuations('\t', 1) << "[4] Update Client Info." << "\n";
	cout << PrintPunctuations('\t', 1) << "[5] Find Client." << "\n";
	cout << PrintPunctuations('\t', 1) << "[6] Transactions." << "\n";
	cout << PrintPunctuations('\t', 1) << "[7] Manage Users." << "\n";
	cout << PrintPunctuations('\t', 1) << "[8] Logout." << "\n";
	cout << PrintPunctuations('=', 45) << "\n";

	PerformMainMenueOption(enMainMenueOptions(ReadMainMenueOption()));
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string Username, Password;

	do
	{
		system("cls");
		cout << PrintPunctuations('-', 32) << "\n";
		cout << PrintPunctuations('\t', 1) << "Login Screen" << "\n";
		cout << PrintPunctuations('-', 32) << "\n";

		if (LoginFaild)
		{
			cout << "Invalaid Username/Password!\n";
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
	Login();


	system("pause>0");
	return 0;
}