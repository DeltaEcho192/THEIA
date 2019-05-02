//TODO Added FileName Storage once frames found

#include <string>
#include <mysql.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <ctime>
#include <thread>
#include <filesystem>

using namespace std;

//This function checks the difference in grey scale between two images using arrays to compare.
void diffCheck(const std::vector<int> frames0, const std::vector<int> frames1, bool &movement, int threshAmount, bool sizeCheck)
{
    using namespace std;
    int i = 0;
    int x = 0;
    int px1,px2;
    int pixelDiff;
    double threshHold = 0;
    int diffLen;
    double threshHoldPerc;
    vector<int> diff;
    int arraylen;
    if(sizeCheck == false)
    {
        arraylen = frames0.size();
    }
    else if(sizeCheck == true)
    {
        arraylen = frames1.size();
    }

    
    clock_t begin = clock();
    while(i < arraylen)
    {
        px1 = frames0[i];
        px2 = frames1[i];
        pixelDiff = px1 - px2;
        //Makes sure that the diff value is Positive
        if(pixelDiff < 0)
        {
            pixelDiff = pixelDiff * -1;
        }
        diff.push_back(pixelDiff);
        if(pixelDiff > 10)
        {
            threshHold = threshHold + 1;
        }

        i = i + 1; 
    }
    //cout << "Thresh Hold: " << threshHold << endl;
    diffLen = diff.size();
    //cout << "Diff Len: " << diffLen << endl;
    //Caluculates the percentage of pixels that were over the threshhold,
    threshHoldPerc = (threshHold / diffLen) * 100;
    //cout << "Thresh perc: " << threshHoldPerc << endl;

    //Decides if there was movement in the image.
    if(threshHoldPerc > threshAmount)
    {
        movement = true;
    }
    //cout << movement << endl;
    clock_t end = clock();
    //Calculates the time taken.
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Amount of time taken: " << elapsed_secs << "\n";

}

//Temp Function, is used to read in the text files with the Grey Scale values
vector<int> txtRead(string path)
{

    ifstream in1(path);
    vector<int> frames;
    string line;
    int output;
    string test;

    if(in1.is_open())
    {
        cout << "File is Open." << endl;
    }
    else
    {
        cout << "There has been a error the file could not be opened." << endl;
    }


    while ( getline(in1,line) )    
    // get next line in file
    {
        stringstream ss(line);

        ss >> output;
        //Adds greyscale value to array which is retuned back to main function
        frames.push_back(output);
        
    }
    cout << frames[0] << endl;
    return frames;

}

//Function which extracts config data from file and returns array with config varibles
vector<string> config(string configPath)
{
    string file = "config.txt";
    
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
        cout << "There was a error opening the config file." << endl;
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


//
//Code Incase diff files are put into database and then can be read out and input into the OPENCV Read.
//

/*
vector<string> newQuery(string query, const char * address, const char * username, const char * password, const char * database,int port)
{
    vector<string> FileToDo;

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
				FileToDo.push_back(row[0]);
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

    return FileToDo;

}
*/

//
//
//


void motionInsert(vector<string> newFiles, const char * address,const char * username, const char * password, const char * database,int port)
{
    MYSQL * conn;
	conn = mysql_init(0);


	conn = mysql_real_connect(conn, address, username, password, database, port, NULL, 0);


	if (conn)
	{
		puts("Successful connection to database!");

		for(int i = 0; i < newFiles.size(); i++)
		{
			string query = "INSERT INTO motionframes_testing (Frame) values ('" + newFiles[i] + "');";
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

int main()
{
    using namespace std;
    namespace fs = std::filesystem;
    string configPath;
    string workPath;
    int threshAmount;
    int threadAmt;

    //Gets Path for config File so no Changes need to be made inside the code on deployment.
    fs::path currentPath = fs::current_path();
    //cout << currentPath.parent_path() << endl;
    string configFileName = "\\config.txt";
    std::ostringstream configStr;
    configStr << currentPath.parent_path();
    workPath = configStr.str();
    workPath.erase(0,1);
    int PathLen = workPath.size() - 1;
    cout << "Path Length " << PathLen << endl;
    workPath.erase(PathLen,1);
    configPath = workPath + configFileName; 


    cout << "Cut Working Path " << configPath << endl;

    //configPath = "C:\\Users\\admin\\Development\\C++\\ImageDelta\\config.txt";

    cout << "Working Directory" << configPath << endl;

    vector<string> settings = config(configPath);

    //Retreving config Settings
    string fileName = settings[0];
    string threshAmountS = settings[1];
    //Converts strint to int
    stringstream threshConvert(threshAmountS);
    threshConvert >> threshAmount;
    
    string framePath = settings[3];

    string addressS = settings[5];
	const char *address = addressS.c_str();
	string UserNameS = settings[6];
	const char * UserName = UserNameS.c_str();
	string passwordS = settings[7];
	const char *password = passwordS.c_str();
	string databaseNameS = settings[8];
	const char *databaseName = databaseNameS.c_str();
	string portNameS = settings[9];

	stringstream portConvert(portNameS);
	portConvert >> portName;
    
    //
    //Currently a useless varible as const cant be declered with a varible
    //Long Complicated way of fiquring out amount useing switch case
    string threadCountS = settings[4];
    stringstream threadCon(threadCountS);
    threadCon >> threadAmt;
    if(threadAmt <= 0 & threadAmt > 64)
    {
        cout << "The Amount of threads is invalid Please Fix - code: 1" << endl;
        exit (EXIT_FAILURE);
    }
    //
    //
    //
    


    //Counts how many files in a given directory.
    int counter = 0;
    for (const auto & entry : fs::directory_iterator(framePath))
    {
        counter = counter + 1;
    }


    int b = 0;
    int threadCounter = 0;


    //Loop to work through all the files in the file directory
    //!!! Remove while loop arg before Final.
    while(b < 1)
    {
    


        //Curently not used code
        /*
        std::ostringstream oss;
        oss << "C:/Users/admin/Development/OpenCV/ImageChange/Frames/frame" << q << ".jpg";
        std::string path1 = oss.str();

        std::ostringstream oss;
        oss << "C:/Users/admin/Development/OpenCV/ImageChange/Frames/frame" << q + 1 << ".jpg";
        std::string path2 = oss.str();
        */
        //
        //
        //

        //TODO How to check movement from frames and get name to store in database

        string line, field;
        int i = 0;
        int x = 0;
        bool movement1 = false;
        bool movement2 = false;
        vector<int> frames0;
        vector<int> frames1;
        bool sizeCheck = false;
        //Remove Before Code Run
        static const int num_threads = 5;

        thread check[num_threads];

        for(int q = 0; q < num_threads; q++)
        {
            if(threadCounter > counter)
            {
                break;
            }
            else
            {
            std::ostringstream oss;
            oss << framePath << fileName << q << ".txt";
            std::string path1 = oss.str();
            cout << "This is the First Path: " << path1 << endl;
        
            std::ostringstream oss1;
            oss1 << framePath << fileName << (q + 1) << ".txt";
            std::string path2 = oss1.str();
            cout << "This is the second path: " << path2 << endl;
            
            frames0 = txtRead(path1);
            frames1 = txtRead(path2);

            if(frames0.size()< frames1.size())
            {
                sizeCheck = false;
            }
            else if (frames1.size() < frames0.size())
            {
                sizeCheck = true;
            }


            check[q] = std::thread(diffCheck,std::ref(frames0) ,std::ref(frames1),std::ref(movement1),threshAmount,sizeCheck);
            q = q + 1;
            threadCounter = threadCounter + 1;
            } 

        }
        for(int z = 0; z < num_threads; z++)
        {
            check[z].join();
            z = z + 1;
        }
        
        //Checking for movement
        if(movement1 == 1)
        {
            cout << "ThreshHold was Passed" << endl;

        }
        else
        {
            cout << "Threre was no Movement" << endl;
        }

        if(movement2 == 1)
        {
            cout << "ThreshHold was Passed" << endl;

        }
        else
        {
            cout << "Threre was no Movement" << endl;
        }

        b = b + 1;
        }
    
    return 0;
}

