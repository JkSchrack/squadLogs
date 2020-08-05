//
// Created by Justin on 2020-07-30.
//

#include <iostream>
#include <fstream> // ofstream: Create/Write Files & ifstream: Read Files
using namespace std;

int main() {
    string text;
    ifstream MyReadFile("SquadGame-backup-2020.07.25-09.37.31.txt");
     while (getline (MyReadFile, text)) {
         cout << myText;
     }
     MyReadFile.Close();
}