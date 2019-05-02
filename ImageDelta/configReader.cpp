//
// Created by admin on 2019/04/10.
//
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <filesystem>

using namespace std;

int main(){

    string path = "";
    ifstream config(path);
    vector<int> settings;
    string line;
    int output;
    string test;

    if(config.is_open())
    {
        cout << "File is Open." << endl;
    }
    else
    {
        cout << "There has been a error the file could not be opened." << endl;
    }

    while ( getline(config,line) )    
    // get next line in file
    {
        stringstream ss(line);

        ss >> output;
        settings.push_back(output);
        
    }
    cout << settings[0] << endl;
    return 0;
}
