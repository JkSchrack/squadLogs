//
// Created by Justin on 2020-07-30.
//
#include <iomanip>
#include <string>
#include <vector>
#include <iostream>
#include <fstream> // ofstream: Create/Write Files & ifstream: Read Files
#include <sstream>
#include <algorithm>
using namespace std;

string inputOperation() {
    int check = 0;
    string operation;
    cout << "Choose the Number of the Operation you would like to Perform!\n";
    cout << "1) Sort Log File into Multiple Files by their Initial LogMessage!\n";
    cout << "2) Server Statistics for a Single Log File!\n";
    cout << "3) Save Log File Server Statistics Information!\n";
    cout << "4) Analyze Saved Server Statistics!\n";
    cout << "5) Exit!\n";
    cout << "Enter the Number of the Desired Operation: ";
    cin >> operation;
    if (operation == "1" || operation == "2" || operation == "3" || operation == "4") {
        check = 1;
    } else if (operation == "5" ){
        exit(-1);
    } else {
        cout << "\n";
        cout << "Invalid Operation! Aborted!";
        exit(-1);
    }
    return operation;
}

string locateFile() {
    int check = 0;
    string fileName;
    cout << "\n";
    cout << "Enter the Absolute File Path of a File to Read; or Enter '5' to Exit!: ";
    cin >> fileName;

    if (fileName == "5") {
        exit(-1);
    }

    ifstream tempFile(fileName);
    if (tempFile){
        cout << "\n";
        cout << "File Located!\n";
        check = 1;
    }
    while (check == 0) {
        cout << "\n";
        cout << "Failed to find " << fileName << "\n";
        fileName = locateFile();
    }
    return fileName;
}

void dateTimeAssignment(string dateTime, string &date, string &beginTime){
    int timeStart = 0;
    int timeEnd = 0;
    for (int n=0;n<dateTime.length();n++) { // Assign Date & Begin Time
        if (dateTime[n] == '-') {
            timeEnd = n;
            for (int j=timeStart;j<timeEnd;j++) { // Date
                date += dateTime[j];
            }
            timeStart = timeEnd + 1;
        }
        if (dateTime[n] == ':') {
            timeEnd = n;
            for (int j=timeStart;j<timeEnd;j++) { // Begin
                beginTime += dateTime[j];
            }
        }
    }
}

void fileParser(ifstream& logFile, vector< vector<string> > &logMessageVector, vector< vector<string> > &gameVector,
                vector< vector<string> > &queueVector, vector< vector<string> > &queueVector2,
                vector< vector<string> > &registrationVector, string &operation ) {
    string line;
    int test = 0;
    int count = 0; // Line Number
    while (getline (logFile, line) ) { // Read a File Line by Line
        count++;
        if (line[0] == '[') {
            test++;
            string info[6]; // 0=Count, 1=Time, 2=LogNumber, 3=LogMessage, 4=LogValue
            info[0] = to_string(count); // Assign Line Number

            int begin = 1;
            int end = 0;

            int logTime = 0;
            int logNumber = 0;
            int logMessage = 0;
            int logValue = 0;
            int logGameMode = 0;
            int logOnlineSession = 0;
            int logEasyAntiCheatServer = 0;
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
                } else if ( operation != "1" && info[3] == "LogGameMode" && logGameMode == 0 && line[i] == ':' && logTime == 1 &&
                            logNumber == 1 && logMessage == 1 && logValue == 0) { // Read a LogGameMode Message

                    string mapTestCheck;
                    for (int n = end + 2; n < i; n++) { //
                        mapTestCheck += line[n];
                    }
                    int slashCount = 0;
                    int mapNameCheck = 0;
                    string mapName;
                    if (mapTestCheck == "ProcessServerTravel") {
                        //cout << "Map Change Detected \n";
                        int endOfMapName;
                        for (int n = line.length() - 1; n > end + 1; n--){ // Grabs Map Name in Reverse
                            if (line[n] == '/') {
                                endOfMapName = n;
                                n = 0;
                            }
                        }
                        for (int j = endOfMapName + 1; j < line.length() - 1; j++) { // Reorientate Map Name
                            mapName += line[j];
                        }
                        mapNameCheck = 1;

                        string dateTime = info[1];
                        string mapDate;
                        string mapBeginTime;
                        dateTimeAssignment(dateTime, mapDate, mapBeginTime);
                        string tempMap;

                        tempMap = mapName + " " + info[1];
                        vector<string> tempMapVector;
                        tempMapVector.push_back(tempMap);
                        tempMapVector.push_back(mapDate);
                        tempMapVector.push_back(mapBeginTime);
                        gameVector.push_back(tempMapVector);
                    }
                    logGameMode = 1;
                } else if ( operation != "1" && info[3] == "LogOnlineSession" && logOnlineSession == 0 && line[i] == ':' &&
                            logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) {
                    string queueSize;
                    string publicQueueCheck;
                    string reservedQueueCheck;
                    int valid = 0;
                    begin = end + 1;
                    end = line.length() - 1;
                    for (int n = begin; n < end; n++) {
                        if (line[n] == ',') {
                            for (int j=n+1;j<n+15;j++) {
                                publicQueueCheck += line[j];
                            }
                            if (publicQueueCheck == "PublicQueue_i:") {
                                queueSize = line[n + 15];
                                if (line [n + 16] != ' '){
                                    queueSize += line[n + 16];
                                }
                                valid = 1;
                            }
                            for (int j=n+1;j<n+17;j++) {
                                reservedQueueCheck += line[j];
                            }
                            if (reservedQueueCheck == "ReservedQueue_i:") {
                                queueSize = line[n + 17];
                                if (line [n + 18] != ' ') {
                                    queueSize += line[n + 18];
                                }
                                valid = 2;
                            }
                            n = end;
                        }
                    }
                    if (valid > 0 ){
                        string dateTime = info[1];
                        string queueDate;
                        string queueTime;
                        dateTimeAssignment(dateTime, queueDate, queueTime);
                        vector<string> tempQueueVector;
                        tempQueueVector.push_back(queueDate);
                        tempQueueVector.push_back(queueTime);
                        tempQueueVector.push_back(queueSize);
                        if (valid == 1){
                            queueVector.push_back(tempQueueVector);
                        }
                        else if ( valid == 2){
                            queueVector2.push_back(tempQueueVector);
                        }
                    }
                    logOnlineSession = 1;
                } else if ( operation != "1" && info[3] == "LogEasyAntiCheatServer" && logEasyAntiCheatServer == 0 && line[i] == ':' &&
                            logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) {
                    int bracketCount = 0;
                    int semicolonCount = 0;
                    string registrationCheck;
                    string registrationPlayerId;
                    string registrationName;
                    int valid = 0;
                    begin = end + 1;
                    end = line.length() - 1;
                    for (int n = begin; n < end; n++) {
                        if (line[n] == '[') {
                            bracketCount++;
                            if (bracketCount == 5) {
                                for (int j = n+1;j<n+15;j++) {
                                    registrationCheck += line[j];
                                }
                                if (registrationCheck == "RegisterClient") {
                                    valid = 1;
                                } else {
                                    registrationCheck = "";
                                    for (int k = n+1;k<n+17;k++) {
                                        registrationCheck += line[k];
                                    }
                                    if (registrationCheck == "UnregisterClient") {
                                        valid = 2;
                                    }
                                }
                                n = end;
                            }
                        }
                    }
                    if (valid > 0 ) {
                        if (valid == 1) {
                            end = line.length() - 1;
                            int begin2 = begin;
                            for (int n = end; n > begin; n--) {
                                if (line[n] == ':') {
                                    begin = n;
                                }
                            }
                            for (int n = begin + 2; n < end; n++) {
                                registrationName += line[n];
                            }

                            int semiCounter = 0;
                            for (int n = end; n > begin2; n--) {
                                if (line[n] == ':') {
                                    if( semiCounter == 1) {
                                        begin2 = n;
                                    }
                                    semiCounter++;
                                }
                            }
                            for (int n = begin2 + 2; n < begin2 + 19; n++) {
                                registrationPlayerId += line[n];
                            }
                        } else if (valid == 2) {
                            end = line.length() - 1;
                            for (int n = end; n > begin; n--) {
                                if (line[n] == ':') {
                                    begin = n;
                                }
                            }
                            for (int n = begin + 2; n < end; n++) {
                                registrationPlayerId += line[n];
                            }
                        }
                        string registrationDate;
                        string registrationTime;
                        string dateTime = info[1];
                        dateTimeAssignment(dateTime, registrationDate, registrationTime);
                        vector<string> tempRegistrationVector;
                        tempRegistrationVector.push_back(registrationDate);
                        tempRegistrationVector.push_back(registrationTime);
                        tempRegistrationVector.push_back(registrationCheck);
                        tempRegistrationVector.push_back(registrationPlayerId);
                        if (valid == 1){
                            tempRegistrationVector.push_back(registrationName);
                        }
                        registrationVector.push_back(tempRegistrationVector);
                    }
                    logEasyAntiCheatServer = 1;

                } else if ( line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1
                            && logValue == 0) { //LogValue
                    begin = end + 1;
                    end = line.length() - 1;
                    for (int n = begin; n < end; n++) {
                        info[4] += line[n];
                    }
                    logValue = 1;
                }

            }
            info[5] = line;

            if (operation == "1"){ // Create Log Vector
                int check = 0;
                if (!logMessageVector.empty()) { // Add Log Message to logMessageVector
                    for (int i = 0; i < logMessageVector.size(); i++) {
                        if (logMessageVector[i][0] == info[3]) {
                            logMessageVector[i].push_back(info[5]);
                            check = 1;
                        }
                    }
                }
                if (check == 0) { // Add a new logMessage to logMessageVector
                    vector<string> tempVector;
                    tempVector.push_back(info[3]);
                    tempVector.push_back(info[5]);
                    logMessageVector.push_back(tempVector);
                }
            }
        }
    }
}

void logMessageFileWriter(vector< vector<string> > &logMessageVector) { // Sort Log Messages into Seperate Files
    string filename; // "c:/Users/Justin/Downloads/Squad/SquadLogs/SQUAD_" + logMessageVector[i][0] + ".txt";
    cout << "\n";
    cout << "Enter Absolute File Path of a Folder to Save the Server Messages to: ";
    cin >> filename;

    int logMessageSize = logMessageVector.size();
    for (int i=0;i<logMessageSize;i++) {
        string tempFileName;
        tempFileName = filename + "/SQUAD_" + logMessageVector[i][0] + ".txt";
        ofstream eachFile(tempFileName);
        for (int n = 0; n < logMessageVector[i].size(); n++) {
            eachFile << logMessageVector[i][n] << "\n";
        }
    }
}

void gameEndTimer(vector< vector<string> > &gameVector) { //Assign EndTime to Games
    int tempSize3 = gameVector.size();
    for (int i=0;i<tempSize3;i++) { //Assign End of Game Time
        if (i == tempSize3 - 1) {
            gameVector.pop_back(); // Remove Last Game of Log File since it may not be complete
        }
        else {
            string endTime = gameVector[i+1][2];
            gameVector[i].push_back(endTime);
        }
    }
}

void printGames(vector< vector<string> > &gameVector){
    int tempSize2 = gameVector.size();
    for (int i=0; i<tempSize2;i++) {
        cout << "Game: " << setw(50) << left << gameVector[i][0] << "  ";
        cout << "Date: " << gameVector[i][1] << "  ";
        cout << "Begin Time: " << gameVector[i][2] << "  ";
        cout << "End Time: " << gameVector[i][3] << "\n";
    }
}

void printQueues(vector< vector<string> > &queueVector){
    int tempSize1 = queueVector.size();
    for (int i=0; i<tempSize1;i++) {
        cout << "Date: " << queueVector[i][0] << "  ";
        cout << "Time: " << queueVector[i][1] << "  ";
        cout << "QueueSize: " << queueVector[i][2] << "\n";
    }
}

void combineRegistrations(vector< vector<string> > &registrationVector){
    int registrationSize1 = registrationVector.size();
    for (int i = 0; i<registrationSize1;i++){
        if (registrationVector[i][2] == "RegisterClient") {
            for (int n = 0; n<registrationSize1; n++) {
                if (registrationVector[n][2] == "UnregisterClient") {
                    if (registrationVector[n][3] == registrationVector[i][3]) {
                        string temp = registrationVector[i][4];
                        registrationVector[n].push_back(temp);
                    }
                }
            }
        } else if (registrationVector[i][2] == "UnregisterClient") {
            //leaveCount++;
        }
    }
}

void printRegistrations(vector< vector<string> > &registrationVector){
    int registrationSize = registrationVector.size();
    for (int i=0; i<registrationSize;i++) {
        cout << "Date: " << registrationVector[i][0] << "  ";
        cout << "Time: " << registrationVector[i][1] << "  ";
        cout << "RegistrationMessage: " << registrationVector[i][2] << "  ";
        cout << "RegistrationPlayerID: " << registrationVector[i][3] << "  ";
        cout << "RegistrationName: " << registrationVector[i][4] << "\n";
    }
}

void combineQueues(vector< vector<string> > &queueVector, vector< vector<string> > &queueVector2){ // Combine Public & Reserve Queue
    int combineQueue1 = queueVector.size();
    for(int i = 0; i<combineQueue1;i++) {
        int temp1 = stoi(queueVector[i][2]);
        int temp2 = stoi(queueVector2[i][2]);
        int temp3 = temp1 + temp2;
        queueVector[i].at(2) = to_string(temp3);
    }
}

void dailyServerStatistics(vector< vector<string> > &gameVector, vector< vector<string> > &queueVector,
                           vector< vector<string> > &registrationVector){
    int numberOfGames = gameVector.size();
    int numberOfQueues = queueVector.size();
    int numberOfRegistrations = registrationVector.size();
    for (int i=0; i<numberOfGames;i++){
        string gameDate = gameVector[i][1];
        string gameBeginTime = gameVector[i][2];
        string gameEndTime = gameVector[i][3];

        string hoursB = gameBeginTime.substr(0,2);
        string minutesB = gameBeginTime.substr(3,2);
        string secondsB = gameBeginTime.substr(6,2);
        int beginTimeSeconds = (stoi(hoursB) * 60 * 60)  + (stoi(minutesB) * 60) + stoi(secondsB);

        string hoursE = gameEndTime.substr(0,2);
        string minutesE = gameEndTime.substr(3,2);
        string secondsE = gameEndTime.substr(6,2);
        int endTimeSeconds = (stoi(hoursE) * 60 * 60)  + (stoi(minutesE) * 60) + stoi(secondsE);

        //Average Queue
        int midnightClause = 0;
        string gameDate2;
        if (gameBeginTime > gameEndTime){ // Incase game proceeds over midnight into the next day
            midnightClause = 1;
            if (i < numberOfGames - 1) {
                gameDate2 = gameVector[i+1][1];
            }
        }

        int totalTimeSeconds;
        if (midnightClause == 1) {
            totalTimeSeconds = 86400 - beginTimeSeconds;
            totalTimeSeconds += endTimeSeconds;
        } else {
            totalTimeSeconds = endTimeSeconds - beginTimeSeconds;
        }
        int previousQueueSize = 0;
        int previousTimeSeconds = beginTimeSeconds;
        int currentQueueSize;
        int queueAverage = 0;
        int queueCount = 0;
        int queuePeak = 0;
        int crossOver = 0;
        for (int j=0; j<numberOfQueues;j++) {
            string queueDate = queueVector[j][0];
            string queueTime = queueVector[j][1];
            string queueSize = queueVector[j][2];

            string hoursQ = queueTime.substr(0,2);
            string minutesQ = queueTime.substr(3,2);
            string secondsQ = queueTime.substr(6,2);
            int queueTimeSeconds = (stoi(hoursQ) * 60 * 60)  + (stoi(minutesQ) * 60) + stoi(secondsQ);
            int queueTimeElapsed = queueTimeSeconds - previousTimeSeconds;

            if (gameDate == queueDate) {
                string tempGameEndTime = gameEndTime;
                if (midnightClause == 1) {
                    tempGameEndTime = "23.59.59";
                }

                if (queueTime > gameBeginTime && queueTime < tempGameEndTime ){
                    if (stoi(queueSize) > queuePeak) {
                        queuePeak = stoi(queueSize);
                    }
                    queueAverage += previousQueueSize * queueTimeElapsed;
                    previousQueueSize = stoi(queueSize);
                    previousTimeSeconds = queueTimeSeconds;
                }
            }
            else if (midnightClause == 1) {
                if (gameDate2 == queueDate) {
                    if (queueTime < gameEndTime) {
                        if (stoi(queueSize) > queuePeak) {
                            queuePeak = stoi(queueSize);
                        }
                        if (crossOver == 0){
                            queueTimeElapsed = 86400 - previousTimeSeconds;
                            queueTimeElapsed += queueTimeSeconds;
                            crossOver = 1;
                        }
                        queueAverage += previousQueueSize * queueTimeElapsed;
                        previousQueueSize = stoi(queueSize);
                        previousTimeSeconds = queueTimeSeconds;
                    }
                }
            }

        }
        float queueResult = 0;
        if (queueAverage > 0) {
            queueResult = (float)queueAverage / (float)totalTimeSeconds;
        }
        string tempQueueResult = to_string(queueResult);
        string tempQueueResult2;
        for (int j=0;j<4;j++){
            tempQueueResult2 += tempQueueResult[j];
        }
        gameVector[i].push_back(tempQueueResult2);
        gameVector[i].push_back(to_string(queuePeak));


        //Average Player Count
        int previousTimeSecondsR = beginTimeSeconds;
        int crossOverR = 0;
        int registrationAverage = 0;
        int playerCount = 0;
        for (int j=0; j<numberOfRegistrations;j++) {
            string registrationDate = registrationVector[j][0];
            string registrationTime = registrationVector[j][1];
            string registrationMessage = registrationVector[j][2];

            string hoursR = registrationTime.substr(0,2);
            string minutesR = registrationTime.substr(3,2);
            string secondsR = registrationTime.substr(6,2);
            int registrationTimeSeconds = (stoi(hoursR) * 60 * 60)  + (stoi(minutesR) * 60) + stoi(secondsR);
            int registrationTimeElapsed = registrationTimeSeconds - previousTimeSecondsR;

            if (gameDate == registrationDate) {
                string tempGameEndTime = gameEndTime;
                if (midnightClause == 1) {
                    tempGameEndTime = "23.59.59";
                }
                if (registrationTime > gameBeginTime && registrationTime < tempGameEndTime ){
                    registrationAverage += playerCount * registrationTimeElapsed;
                    previousTimeSecondsR = registrationTimeSeconds;
                    if (registrationMessage == "RegisterClient"){
                        playerCount++;
                    }
                    else if (registrationMessage == "UnregisterClient") {
                        if (playerCount > 0) {
                            playerCount--;
                        }
                    }
                }
            }
            else if (midnightClause == 1) {
                if (gameDate2 == registrationDate) {
                    if (registrationTime < gameEndTime) {
                        if (crossOverR == 0){
                            registrationTimeElapsed = 86400 - previousTimeSecondsR;
                            registrationTimeElapsed += registrationTimeSeconds;
                            crossOverR = 1;
                        }
                        registrationAverage += playerCount * registrationTimeElapsed;
                        previousTimeSecondsR = registrationTimeSeconds;
                        if (registrationMessage == "RegisterClient"){
                            playerCount++;
                        }
                        else if (registrationMessage == "UnregisterClient") {
                            if (playerCount > 0){
                                playerCount--;
                            }
                        }
                    }
                }
            }
        }

        float registrationResult = 0;
        if (registrationAverage > 0) {
            registrationResult = (float)registrationAverage / (float)totalTimeSeconds;
        }
        string tempRegistrationResult = to_string(registrationResult);
        string tempRegistrationResult2;
        for (int j=0;j<4;j++){
            tempRegistrationResult2 += tempRegistrationResult[j];
        }
        gameVector[i].push_back(tempRegistrationResult2);
        int gameMinutes = totalTimeSeconds / 60;
        gameVector[i].push_back(to_string(gameMinutes));
    }
}

void printDailyStatistics(vector< vector<string> > &gameVector){
    int gameTestSize = gameVector.size();
    for (int i=0; i<gameTestSize;i++) {
        cout << "Game: " << setw(55) << left << gameVector[i][0] << "  ";
        cout << "Date: " << gameVector[i][1] << "  ";
        cout << "Begin Time: " << gameVector[i][2] << "  ";
        cout << "End Time: " << gameVector[i][3] << "  ";
        cout << "Average Queue: " << gameVector[i][4] << "  ";
        cout << "Queue Peak: " << setw(3) << left << gameVector[i][5] << "  ";
        cout << "Average Player Count: " << gameVector[i][6] << "  ";
        cout << "Game Time (Minutes): " << gameVector[i][7] << "\n";
    }
}

void saveServerStatistics(vector< vector<string> > &gameVector){
    string filename;
    cout << "\n";
    cout << "Enter the File Path of a File to Save Server Statistics to: ";
    cin >> filename; // c:/Users/Justin/Downloads/Squad/SQUAD_DerivedData.txt

    ifstream tempFile(filename);
    if (tempFile) {
        string proceed;
        cout << "\n";
        cout << "File Exists! \n";
        cout << "Enter '1' to Append to File or '2' to Abort: ";
        cin >> proceed;
        if (proceed == "1"){
            cout << "\n";
            cout << "Proceeding to Append to " << filename << "!\n";
        } else {
            cout << "Aborting!\n";
            exit(-1);
        }
        tempFile.close();
    } else {
        cout << "Proceeding to Create & Write to " << filename << "!\n";
        ofstream  createFile(filename);
    }

    ofstream derivedFile(filename, derivedFile.out | derivedFile.app);
    int derivedSize = gameVector.size();
    for (int i = 0; i < derivedSize; i++) {
        string line;
        line += "Game: " + gameVector[i][0] + " ";
        line += "Date: " + gameVector[i][1] + " ";
        line += "BeginTime: " + gameVector[i][2] + " ";
        line += "EndTime: " + gameVector[i][3] + " ";
        line += "AverageQueue: " + gameVector[i][4] + " ";
        line += "QueuePeak: " + gameVector[i][5] + " ";
        line += "AveragePlayerCount: " + gameVector[i][6] + " ";
        line += "GameTimeMinutes: " + gameVector[i][7] + "\n";
        derivedFile << line;
    }

    derivedFile.close();
}

void readSavedStatistics(ifstream& logFile, vector< vector<string> > &savedStatisticsVector) {
    string line;
    int test = 0;
    int count = 0; // Line Number
    while (getline (logFile, line) ) {
        istringstream words(line);
        string word;
        int count = 0;

        vector<string> tempVector;
        // 2 = map, 5 = date, 11 = AverageQueue, 13 = Queue Peak, 15 = AveragePlayerCount, 17 = GameTimeMinutes
        while (words >> word) {
            count++;
            // cout << word << "\n";
            if (count == 2 || count == 3 || count == 11 || count == 13 || count == 15 || count == 17 ) {
                tempVector.push_back(word);
            }
        }
        savedStatisticsVector.push_back(tempVector);
    }
}

void printSavedStatistics(vector< vector<string> > &savedStatisticsVector) {
    int size = savedStatisticsVector.size();
    for (int i=0; i<size;i++) {
    cout << "Map: " << setw(30) << left << savedStatisticsVector[i][0] << "  ";
    cout << "Date: " << savedStatisticsVector[i][1] << "  ";
    cout << "Average Queue: " <<  setw(5) << left << savedStatisticsVector[i][2] << "  ";
    cout << "Queue Peak: " << setw(5) << left << savedStatisticsVector[i][3] << "  ";
    cout << "Average Player Count: " << savedStatisticsVector[i][4] << "  ";
    cout << "Game Time (Minutes): " << savedStatisticsVector[i][5] << "\n";
    }
}

void analyzeServerStatistics(vector< vector<string> > &savedStatisticsVector,
                             vector< vector<string> > &analyzedStatisticsVector) {
    vector<string> uniqueMaps;
    for (int i = 0; i < savedStatisticsVector.size(); i++) {
        string mapName = savedStatisticsVector[i][0];
        int unique = 0;
        for (int n = 0; n < uniqueMaps.size(); n++) {
            if (mapName == uniqueMaps[n]) {
                unique = 1;
            }
        }
        if (unique == 0 ) {
            uniqueMaps.push_back(mapName);
        }
    }

    sort(uniqueMaps.begin(), uniqueMaps.end());
    for (int i = 0; i < uniqueMaps.size(); i++) {
        string playTimes = " ";
        int count = 0;
        float averageQueue = 0;
        int averageQueuePeak = 0;
        float averagePlayerCount = 0;
        int averageGameTime = 0;
        for (int n = 0; n < savedStatisticsVector.size(); n++) {
            if (savedStatisticsVector[n][0] == uniqueMaps[i]) {
                count++;
                playTimes += "  " + savedStatisticsVector[n][1];
                averageQueue += stof(savedStatisticsVector[n][2]);
                averageQueuePeak += stoi(savedStatisticsVector[n][3]);
                averagePlayerCount += stof(savedStatisticsVector[n][4]);
                averageGameTime += stoi(savedStatisticsVector[n][5]);
            }
        }
        averageQueue = averageQueue / count;
        averageQueuePeak = averageQueuePeak / count;
        averagePlayerCount = averagePlayerCount / count;
        averageGameTime = averageGameTime / count;
        vector<string> tempVector;
        tempVector.push_back(uniqueMaps[i]);
        tempVector.push_back(to_string(averageQueue));
        tempVector.push_back(to_string(averageQueuePeak));
        tempVector.push_back(to_string(averagePlayerCount));
        tempVector.push_back(to_string(averageGameTime));
        tempVector.push_back(to_string(count));
        tempVector.push_back(playTimes);
        analyzedStatisticsVector.push_back(tempVector);
    }
}

void printAnalyzedServerStatistics(vector< vector<string> > &analyzedStatisticsVector) {
    int size = analyzedStatisticsVector.size();
    for (int i=0; i<size;i++) {
        cout << "Map: " << setw(30) << left << analyzedStatisticsVector[i][0] << "  ";
        cout << "Average Queue: " <<  setw(10) << left << analyzedStatisticsVector[i][1] << "  ";
        cout << "Average QueuePeak: " << setw(10) << left << analyzedStatisticsVector[i][2] << "  ";
        cout << "Average PlayerCount: " << setw(10) << left << analyzedStatisticsVector[i][3] << "  ";
        cout << "Average GameTime (Minutes): " << setw(10) << left << analyzedStatisticsVector[i][4] << "  ";
        cout << "Times Played: " << analyzedStatisticsVector[i][5] << "\n";
    }
}

void saveAnalyzation(vector< vector<string> > &analyzedStatisticsVector){
    string filename;
    cout << "\n";
    cout << "Enter File Path to Save Analyzed Statistics or '2' to Exit: ";
    cin >> filename; // C:\Users\Justin\Downloads\Squad\SQUAD_Analyzed.txt

    if (filename == "2"){
        exit(-1);
    }

    ifstream tempFile(filename);
    if (tempFile) {
        cout << "\n";
        cout << "File Exists! Aborting!";
        exit(-1);
    }
    tempFile.close();

    ofstream analyzationFile(filename);
    for (int i = 0; i < analyzedStatisticsVector.size(); i++) {
        string map = "Map: " + analyzedStatisticsVector[i][0] + " ";
        analyzationFile << "Map: " << setw(30) << left << analyzedStatisticsVector[i][0] << "  ";
        analyzationFile << "Average Queue: " <<  setw(10) << left << analyzedStatisticsVector[i][1] << "  ";
        analyzationFile << "Average QueuePeak: " << setw(10) << left << analyzedStatisticsVector[i][2] << "  ";
        analyzationFile << "Average PlayerCount: " << setw(10) << left << analyzedStatisticsVector[i][3] << "  ";
        analyzationFile << "Average GameTime (Minutes): " << setw(5) << left << analyzedStatisticsVector[i][4] << "  ";
        analyzationFile << "Times Played: " << setw(5) << left << analyzedStatisticsVector[i][5] << "  ";
        analyzationFile << "Play Dates: " << analyzedStatisticsVector[i][6] << "\n";
    }
    analyzationFile.close();


    int size = analyzedStatisticsVector.size();
    for (int i=0; i<size;i++) {
        cout << "Map: " << setw(30) << left << analyzedStatisticsVector[i][0] << "  ";
        cout << "Average Queue: " <<  setw(10) << left << analyzedStatisticsVector[i][1] << "  ";
        cout << "Average QueuePeak: " << setw(10) << left << analyzedStatisticsVector[i][2] << "  ";
        cout << "Average PlayerCount: " << setw(10) << left << analyzedStatisticsVector[i][3] << "  ";
        cout << "Average GameTime (Minutes): " << setw(10) << left << analyzedStatisticsVector[i][4] << "  ";
        cout << "Times Played: " << analyzedStatisticsVector[i][5] << "\n";
    }
}

int main() {
    vector< vector<string> > logMessageVector;
    vector< vector<string> > gameVector;
    vector< vector<string> > queueVector;
    vector< vector<string> > queueVector2;
    vector< vector<string> > registrationVector;
    vector< vector<string> > savedStatisticsVector;
    vector< vector<string> > analyzedStatisticsVector;
    string line;

    string operation = inputOperation();
    ifstream logFile(locateFile());

    if (operation == "1" || operation == "2" ||  operation == "3") {
        fileParser(logFile, logMessageVector, gameVector, queueVector, queueVector2, registrationVector, operation);
    }

    if (operation == "1") { // Sort Log Information into Seperate Files by their LogMessage
        logMessageFileWriter(logMessageVector);
    }

    if (operation == "2" || operation == "3") { // Server Statistics
        gameEndTimer(gameVector);

        combineRegistrations(registrationVector);

        combineQueues(queueVector, queueVector2);

        dailyServerStatistics(gameVector, queueVector, registrationVector);

        printDailyStatistics(gameVector);
    }

    if (operation == "3") { // Save Server Statistics
        saveServerStatistics(gameVector);
    }

    if (operation == "4") {
        readSavedStatistics(logFile, savedStatisticsVector);

        // printSavedStatistics(savedStatisticsVector);

        analyzeServerStatistics(savedStatisticsVector, analyzedStatisticsVector);

        printAnalyzedServerStatistics(analyzedStatisticsVector);

        saveAnalyzation(analyzedStatisticsVector);
    }

    logFile.close();
}