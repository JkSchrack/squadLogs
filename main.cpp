//
// Created by Justin on 2020-07-30.
//
#include <string>
#include <vector>
#include <iostream>
#include <fstream> // ofstream: Create/Write Files & ifstream: Read Files
using namespace std;

int main() {
    vector< vector<string> > logMessageVector;
    string line;
    ifstream file("c:/Users/Justin/Downloads/Squad/SquadGame-backup-2020.07.25-09.37.31.log");

    if (!file){
        cout << "Failed to find File!\n";
    }
    int test = 0;
    int count = 0;
    while (getline (file, line) ) {
        count++;
        if (line[0] == '[') {
            test++;
            string info[6]; // 0=Count, 1=Time, 2=LogNumber, 3=LogMessage, 4=LogValue
            info[0] = to_string(count);

            int begin = 1;
            int end = 0;

            int logTime = 0;
            int logNumber = 0;
            int logMessage = 0;
            int logValue = 0;
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ']' && logTime == 0) { //logTime
                    end = i;
                    for (int n = begin; n < end; n++) {
                        info[1] += line[n];
                    }
                    logTime = 1;
                } else if (line[i] == ']' && logTime == 1 && logNumber == 0) { //LogNumber
                    begin = end + 2;
                    end = i;
                    for (int n = begin; n < end; n++) {
                        info[2] += line[n];
                    }
                    logNumber = 1;
                } else if (line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 0) { //LogMessage
                    begin = end + 1;
                    end = i;
                    for (int n = begin; n < end; n++) {
                        info[3] += line[n];
                    }
                    logMessage = 1;
                } else if (line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1 &&
                           logValue == 0) { //LogValue
                    begin = end + 1;
                    end = line.length() - 1;
                    for (int n = begin; n < end; n++) {
                        info[4] += line[n];
                    }
                    logValue = 1;
                }
            }
            info[5] = line;

            int check = 0;
            if (!logMessageVector.empty()) {
                for (int i = 0; i < logMessageVector.size(); i++) {
                    if (logMessageVector[i][0] == info[3]) {
                        logMessageVector[i].push_back(info[5]);
                        check = 1;
                    }
                }
            }
            if (check == 0) {
                vector<string> tempVector;
                tempVector.push_back(info[3]);
                tempVector.push_back(info[5]);
                logMessageVector.push_back(tempVector);
            }

            /*
            if (stoi(info[0]) > 400 && stoi(info[0]) < 450) {
                cout << info[5] << "\n";
                cout << "Line: " << info[0] << " ";
                cout << "logTime: " << info[1] << " ";
                cout << "logNumber: " << info[2] << " ";
                cout << "logMessage: " << info[3] << " ";
                cout << "logValue: " << info[4] << " ";
                cout << "\n";
            }
             */
        }

        /*
        for (int i=0;i<10;i++) {
            vector<string> vector;
            for (int n=0;n<5;n++) {
                vector.push_back(to_string(n));
            }
            logMessageVector.push_back(vector);
        }

        cout << to_string(logMessageVector.size());
        cout << "\n";
         */

    }

    for (int i=0;i<logMessageVector.size();i++) {
        string filename = "SQUAD_" + logMessageVector[i][0] + ".txt";
        ofstream eachFile(filename);
        for (int n=0;n<logMessageVector[i].size();n++) {
            eachFile << logMessageVector[i][n] << "\n";
            //cout << logMessageVector[i][n] << "\n";
        }
    }

    file.close();
}