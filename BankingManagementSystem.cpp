#include <iostream>       
#include <iomanip>       
#include <fstream>       
#include <sstream>       
#include <string>        
#include <memory>        
#include <conio.h>      
#include <openssl/sha.h> // SHA-256 hashing
using namespace std;



string hashing_pin(string pin) //hashing function to securely store the PIN uses SHA-256 from Open-SSL
{   
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, pin.c_str(), pin.size());
    SHA256_Final(hash, &ctx);
    stringstream ss;
    for(auto i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    
    return ss.str();
}



class BankAccount //Bank Account Class for Account object holding : 1.Account Holder name 2.Account Number 3.PIN 4. Account Balance
{
    private:
        string accountHolder;
        int accountNumber;
        double balance;
        string pass;

    public:
        virtual void updateBalance(int AccNo, string& pass) = 0;
        static void loadAccCount();
        static void updateAccCount();
        static int accountCount;
        BankAccount(){};
        BankAccount(string name, double initialBalance, string pin)
            {   
                loadAccCount(); //loads the account Count for account number generation
                this ->accountHolder = name;
                this ->accountNumber = accountCount;
                this ->pass = hashing_pin(pin) ;
                this ->balance = initialBalance;
                ofstream file("Account_Details.csv", ios::app) ;
                file << accountHolder<<','<< accountNumber<<','  << this -> pass<<',' <<fixed << setprecision(2)<< balance << endl;
                file.close() ;
                cout << "Account Created Successfully !" << endl << "Your Account Number is : " << accountNumber<<endl;
                accountCount++;
                cout << "Account Balance : $"<< balance << endl;
                updateAccCount(); //updates the account count after generating account number for the account
            }
 
        void displayAccDetails(int accNo, string pass)
            { 
                ifstream inputFile("Account_Details.csv");
                if (!inputFile) 
                {
                    cerr << "Error: Cannot open account file!" << endl;
                    return;
                }
                string entered_pin = hashing_pin(pass);
                bool result = false;
                string line, name, pin, accountNoStr, fileBalance;
                int accountNo;
                while(getline(inputFile, line))
                    {          
                        stringstream ss(line);
                        if(! getline(ss, name, ',')) continue;
                        if(! getline(ss, accountNoStr, ',')) continue;
                        if(! getline(ss, pin, ',')) continue;
                        if(! getline(ss, fileBalance, ',')) continue;

                        accountNo = stoi(accountNoStr);
                        double balance = stod(fileBalance);

   

                        if(accountNo == accNo && entered_pin == pin)
                            {   
                                cout <<endl<<"Account Holder Name: "<<name << endl;
                                cout <<"Account Number: "<< accountNo << endl;
                                cout <<"Balance : $"<< balance << endl;
                                result = true;
                                break;
                            } 

 
                    }  
                if(!result)
                    {
                        cout<< "Account Not Found !! Invalid Account No or PIN"<<endl;
                    }
                inputFile.close();
            }
};

int BankAccount :: accountCount = 1000; // assigning default account number for the first account; can be changed accordingly

void BankAccount :: loadAccCount()
{
    ifstream file("Account_Count.csv");
    if(!file)
    {
        BankAccount :: accountCount = 1000; //default account number if no account exists
    }
    else
    {
        file >> BankAccount :: accountCount; //getting account count if accounts already exists
        file.close();
    }
    

}

void BankAccount :: updateAccCount()
{
    ofstream file("Account_Count.csv", ios::trunc);
    
    file << BankAccount :: accountCount; //updating account count after each successful creation of account
    file.close();

}

class Banking : public BankAccount //Banking class derived from Bank account class for Banking operations like 1.deposit 2.withdrawal
{   
    public:
        Banking(string name, double bal, string pass) : BankAccount(name , bal, pass){}//parameterised constructor
        Banking(){}; //default constructor

        void updateBalance (int accNo, string &pass) override 
        {
            string entered_pin = hashing_pin(pass);
            ifstream file("Account_Details.csv");
            if (!file) 
                {
                    cerr << "Error: Cannot open account file!" << endl;
                    return;
                }
            ofstream tempFile("temporary_file.csv", ios::trunc);
            string line, name, accountNostr, pin, balanceStr;
            double newBalance, amount;
            bool Accfound = false, status = false;
            cout<<"Choose Preferred Service"<<endl
                << "1. Deposit" <<endl
                << "2. Withdrawal " <<endl;
            
            int choice;
            cin >> choice;
            cin.ignore(); //flushing cin before using getline()
        
                cout << "Enter Amount : $";
                cin>>amount;
                while (getline(file, line))
                {
                    stringstream ss(line);
                    if (!getline(ss, name, ',')) continue;
                    if (!getline(ss, accountNostr, ',')) continue;
                    if (!getline(ss, pin, ',')) continue;
                    if (!getline(ss, balanceStr, ',')) continue;

                    int accountNo = stoi(accountNostr);
                    double balance = stod(balanceStr);

                    if (!Accfound && accountNo == accNo && entered_pin == pin) 
                    {   
                        // Deposit
                        if (choice == 1) 
                        {   
                            if (amount <= 0) 
                            {   
                                cout << "Invalid Input!" << endl;
                                status = false;
                            }
                            else
                            {
                                newBalance = balance + amount;
                                status = true;
                            }
                        }
                        // Withdrawal
                        else if (choice == 2)
                        {   
                            if (amount <= balance) 
                            {
                                newBalance = balance - amount;
                                status = true;
                            }
                            else 
                            {
                                cout << "Insufficient Fund!!" << endl;
                                newBalance = balance;
                                status = false;
                            }
                        }
                        else 
                        {   
                            newBalance = balance;
                            cout << "Invalid Response!!" << endl;
                            status = false;
                        }

                        Accfound = true;
                        tempFile << name << "," << accountNo << "," << pin << "," 
                                << fixed << setprecision(2) << newBalance << endl;
                    }
                    else
                    {
                        tempFile << name << "," << accountNo << "," << pin << "," 
                                << fixed << setprecision(2) << balance << endl;
                    }
                }

                       file.close();
                       tempFile.close();
                if(!status)
                {
                    cerr << "Transaction Failed !"<<endl;
                }
                if(!Accfound)
                {
                    cerr<<"Invalid Account Number or PIN" << endl;
                }
            
                else
                {
                    cout << "Transaction Completed Successfully!!"<<endl;
                    remove("Account_Details.csv");
                    rename("temporary_file.csv", "Account_Details.csv");
                }
            
            }
    };


string pin_updation(int length = 6) //for getting PIN as INPUT; Masks the PIN While Entering for Security
{
    string pin = "";
    char ch;
    cout<<"Enter 6 Digit Pin :"<<endl;
    while(pin.length() < length)
    {
        ch = _getch();
        if (isdigit(ch)){
            cout<<"*";
            pin += ch ;
        }
        
    };
     return pin;

}

int main()
{
    while (true) // Menu loop
    {
        cout << "\n======= Banking System Menu =======\n";
        cout << "1. Account Creation\n";
        cout << "2. Banking (Deposit / Withdrawal)\n";
        cout << "3. Show Account Balance\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // clear newline from buffer

        string name, pass;
        double bal;
        int accNo;

        if (choice == 1)
        {
            cout << "Enter your Name: ";
            getline(cin, name);
            cout << "\nCreate PIN for your Account\n";
            pass = pin_updation();
            cout << "\nEnter the Initial Deposit amount: ";
            cin >> bal;
            unique_ptr<BankAccount> b = make_unique<Banking>(name, bal, pass);
        }
        else if (choice == 2)
        {
            unique_ptr<Banking> b = make_unique<Banking>();
            cout << "Enter Account Number: ";
            cin >> accNo;
            cin.ignore();
            pass = pin_updation();
            b->updateBalance(accNo, pass);
        }
        else if (choice == 3)
        {
            unique_ptr<Banking> b = make_unique<Banking>();
            cout << "Enter Account Number: ";
            cin >> accNo;
            cin.ignore();
            pass = pin_updation();
            b->displayAccDetails(accNo, pass);
        }
        else if (choice == 4)
        {
            cout << "Thank you for using our Banking System. Goodbye!\n";
            break; // Exit loop
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}

