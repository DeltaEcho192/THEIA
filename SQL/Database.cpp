// Database.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include "pch.h"
#include <iostream>
#include <mysql.h>
#include <vector>
#include <filesystem>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <ctime>
#include <thread>
#include <filesystem>
#include <iostream> 
#include <string.h> 

using namespace std;
int qstate;

vector<string> oldRecords(string query, const char * address, const char * username, const char * password, const char * database,int port)
{
	vector<string> dir_list;

	MYSQL * conn;
	MYSQL_ROW row;
	MYSQL_RES *res;
	conn = mysql_init(0);


	conn = mysql_real_connect(conn, address, username, password, database, port, NULL, 0);


	if (conn)
	{
		puts("Successful connection to database!");


		const char* q = query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				dir_list.push_back(row[0]);
			}
		}
		else
		{
			cout << "Query failed - 001: " << mysql_error(conn) << endl;
		}
	}
	else
	{
		puts("Connection to database had failed! - 002");
	}

	return dir_list;
}

vector<string> curFiles(string path)
{
	vector<string> output;
	string fullPath;
	
	namespace fs = std::filesystem;
	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
		std::filesystem::path workingPath = entry.path();
		fullPath = workingPath.string();
		int size = fullPath.length();
		char tab2[1024];
		strcpy_s(tab2, fullPath.c_str());
		int test = strlen(tab2);
		int counter = 0;
		cout << fullPath << endl;
		//TODO Find size of char array or try find amount of char in string
		for (int i = size; i > 0; i--)
		{
			if(tab2[i] == '\\')
			{
				break;
			}
			else
			{
				counter++;
			}
		}
		int cutSize = size - (counter-1);
		string fileName = fullPath.substr(cutSize);

		counter = 0;
		test = 0;
		size = 0;

		//TODO Get only the first / from back
		output.push_back(fileName);
	}
	return output;

}

void insert(vector<string> newFiles, const char * address,const char * username, const char * password, const char * database,int port)
{
	//TODO Create Insert to sql for new files
	MYSQL * conn;
	conn = mysql_init(0);


	conn = mysql_real_connect(conn, address, username, password, database, port, NULL, 0);


	if (conn)
	{
		puts("Successful connection to database!");

		for(int i = 0; i < newFiles.size(); i++)
		{
			string query = "INSERT INTO dir_list_test (name) values ('" + newFiles[i] + "');";
			cout << query << endl;
			const char* q = query.c_str();
			qstate = mysql_query(conn, q);
			cout << qstate << endl;
			if(qstate == 0)
			{
				cout << "Insertion was successful" << endl;
			}
			else
			{
				cout << "There was a failure - 003" << mysql_error(conn) << endl;
			}
		}
	}
	else
	{
		puts("Connection to database had failed! - 002");
	}
}

void delete1(vector<string> nukeFiles, const char * address, const char * username, const char * password, const char * database,int port)
{
	MYSQL * conn;
	conn = mysql_init(0);


	conn = mysql_real_connect(conn, address, username, password, database, port, NULL, 0);


	if (conn)
	{
		puts("Successful connection to database!");

		for(int i = 0; i < nukeFiles.size(); i++)
		{
			string query = "DELETE FROM dir_list_test WHERE name = '" + nukeFiles[i] + "';";
			cout << query << endl;
			const char* q = query.c_str();
			qstate = mysql_query(conn, q);
			if(qstate == 0)
			{
				cout << "Deletion was successful" << endl;
			}
			else
			{
				cout << "There was a failure -004 " << mysql_error(conn) << endl;
			}
		}
	}
	else
	{
		puts("Connection to database had failed! - 002");
	}

}

vector<string> config(string configPath)
{
    string file = "database_config.txt";
    
    ifstream config(configPath);
    vector<string> settings;
    string line;
    string output;

    if(config.is_open())
    {
        cout << "Config File Was Opened." << endl;
    }
    else
    {
        cout << "There was a error opening the config file. -005" << endl;

    }

    while ( getline(config,line) )    
    // get next line in file
    {
        stringstream ss(line);

        ss >> output;
        settings.push_back(output);
        
    }
    return settings;
}


int main()
{

	//TODO Check why running so slow
	using namespace std;
	namespace fs = std::filesystem;

	vector<string> oldRecords1;
	vector<string> curRecords;
	vector<string> newFiles;
	vector<string> configArr;
	int portName;
	string workPath;
	string configPath;

	fs::path currentPath = fs::current_path();
    //cout << currentPath.parent_path() << endl;
    string configFileName = "\\database_config.txt";
    std::ostringstream configStr;
    configStr << currentPath.parent_path();
    workPath = configStr.str();
    workPath.erase(0,1);
    int PathLen = workPath.size() - 1;
    cout << "Path Length " << PathLen << endl;
    workPath.erase(PathLen,1);
    configPath = workPath + configFileName; 
	cout << configPath << " :This is the Config Path" << endl;


	configArr = config(configPath);

	string addressS = configArr[1];
	const char *address = addressS.c_str();
	string UserNameS = configArr[2];
	const char * UserName = UserNameS.c_str();
	string passwordS = configArr[3];
	const char *password = passwordS.c_str();
	string databaseNameS = configArr[4];
	const char *databaseName = databaseNameS.c_str();
	string portNameS = configArr[5];

	stringstream portConvert(portNameS);
	portConvert >> portName;

	string query = "SELECT * FROM dir_list_test";
	oldRecords1 = oldRecords(query,address,UserName,password,databaseName,portName);

	for(int q = 0;  q < oldRecords1.size(); q++)
	{
		cout << oldRecords1[q] << endl;
	}

	string pathFiles = "C:\\Users\\admin\\Development\\Python\\Recon\\Dir_list\\testing_enviro";
	curRecords = curFiles(pathFiles);

	for(int b = 0; b < curRecords.size(); b++)
	{
		cout << curRecords[b] << endl;
	}


	//TODO Do searching of files
	int z = 0;
	int statCounter = 0;
	bool add = true;
	int sizeCur = curRecords.size();
	int sizeOld = oldRecords1.size();
	if(sizeOld < sizeCur)
	{
		while(z < curRecords.size())
		{
			bool add = true;
			string workvar1 = curRecords[z];
			for(int i = 0; i < oldRecords1.size(); i++ )
			{
				if(workvar1 == oldRecords1[i])
				{
					add = false;
					cout << "Match Found!" << endl;
					break;
					
				}
			}
			if(add == true)
			{
				newFiles.push_back(workvar1);
				statCounter = statCounter + 1;
			}
			z = z + 1;
		}
		cout << statCounter << " :Amount of New Files" << endl;
		insert(newFiles,address,UserName,password,databaseName,portName);
	}

	//TODO Add deletion of files.

	//Onlyfiles = curRecords
	//dir_list = OldRecords1


	if(sizeCur < sizeOld)
	{
		vector<string> oldRecords_CP;
		oldRecords_CP = oldRecords1;
		int n = 0;

		while(n < curRecords.size())
		{

			string workvar1 = curRecords[n];
			for(int i = 0; i < oldRecords1.size(); i++ )
			{
				if(workvar1 == oldRecords1[i])
				{
					auto itr = find(oldRecords_CP.begin(), oldRecords_CP.end(),workvar1);
					if(itr != oldRecords_CP.end()) oldRecords_CP.erase(itr);
					cout << workvar1 << " :Still Exists" << endl;
					break;
				}
			}
			n = n + 1;
		}

	delete1(oldRecords_CP,address, UserName, password, databaseName, portName);

	}

	if (sizeCur == sizeOld)
	{
		cout << "There are no new files" << endl;
		exit(EXIT_FAILURE);
	}
	

	return 0;
}


