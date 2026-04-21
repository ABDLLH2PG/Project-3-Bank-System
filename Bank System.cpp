//Project #3, Bank 1 (V2);
//Due: In 2026/4/20

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions { enShowClientList = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClientInfo = 4, enFindClient = 5, enTransactions = 6, enExit = 7 };

enum enTransactionsMenueOptions { enDeposit = 1, enWithdraw = 2, TotalBalances = 3, MainMenue = 4 };

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;
};

void ShowMainMenue();

void ShowTransactionsMenue();

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
		cout << "Choose What do you want to do? [1 to 7]? ";
		cin >> Choose;
	} while (!(Choose >= 1 && Choose <= 7));

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

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;

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

void ShowClientListScreen()
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintAllClientsData(vClients);
}

void AddNewClientScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Add New Clients Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	AddClients();
}

void DeleteClientScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Delete Client Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void UpdateClientInfoScreen()
{
	cout << "\n";
	cout << PrintPunctuations('-', 35) << "\n";
	cout << PrintPunctuations('\t', 1) << "Update Client Info Screen\n";
	cout << PrintPunctuations('-', 35) << "\n";

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void FindClientScreen()
{
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

void PerformMainMenueOption(enMainMenueOptions enOptions)
{
	switch (enOptions)
	{
	case enMainMenueOptions::enShowClientList:
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::enAddNewClient:
		system("cls");
		AddNewClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::enDeleteClient:
		system("cls");
		DeleteClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::enUpdateClientInfo:
		system("cls");
		UpdateClientInfoScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::enFindClient:
		system("cls");
		FindClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::enTransactions:
		system("cls");
		ShowTransactionsMenue();
		break;

	case enMainMenueOptions::enExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void PerformTransactionMenueOption(enTransactionsMenueOptions enOptions)
{
	switch (enOptions)
	{
	case enTransactionsMenueOptions::enDeposit:
		system("cls");
		DepositScreen();
		GoBackToTransactionsMenue();
		break;

	case enTransactionsMenueOptions::enWithdraw:
		system("cls");
		WithdrawScreen();
		GoBackToTransactionsMenue();
		break;

	case enTransactionsMenueOptions::TotalBalances:
		system("cls");
		ShowBalancesListScreen();
		GoBackToTransactionsMenue();
		break;

	case enTransactionsMenueOptions::MainMenue:
		ShowMainMenue();
		break;
	}
}

void ShowTransactionsMenue()
{
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
	cout << PrintPunctuations('\t', 1) << "[7] Exit." << "\n";
	cout << PrintPunctuations('=', 45) << "\n";
	PerformMainMenueOption(enMainMenueOptions(ReadMainMenueOption()));
}


int main()
{
	ShowMainMenue();


	return 0;
}