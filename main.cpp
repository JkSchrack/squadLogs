//
// Created by Justin on 2020-07-30.
//
#include <iomanip>
#include <string>
#include <vector>
#include <iostream>
#include <fstream> // ofstream: Create/Write Files & ifstream: Read Files
using namespace std;

string locateFile() {
    string fileName;
    cout << "Enter a File to Read: ";
    cin >> fileName;

    ifstream tempFile(fileName);
    if (!tempFile){
        cout << "Failed to find " << fileName << "\n";
        exit(-1);
    }
    cout << "File Located!\n";
    return fileName;
}

int logMessageExtractor() {
    return 0;
}

int main() {
    vector< vector<string> > logMessageVector;
    vector< vector<string> > gameVector;
    vector< vector<string> > queueVector;
    vector< vector<string> > queueVector2;
    vector< vector<string> > registrationVector;
    string line;
    /*
    string logFilename = "c:/Users/Justin/Downloads/Squad/SquadGame-backup-2020.08.10-23.10.21.log";
    ifstream logFile(logFilename);
    if (!logFile){
        cout << "Failed to find " << logFilename << "\n";
    }
     */

    ifstream logFile(locateFile());


    int test = 0;
    int count = 0;
    while (getline (logFile, line) ) {
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
                } else if ( info[3] == "LogGameMode" && logGameMode == 0 && line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) { // Read a LogGameMode Message
                    string mapTestCheck;
                    for (int n = end + 2; n < i; n++) {
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

                        string mapDate;
                        string mapBeginTime;
                        string dateTime = info[1];
                        int timeStart = 0;
                        int timeEnd = 0;
                        for (int n=0;n<dateTime.length();n++) { // Assign Date & Begin Time
                            if (dateTime[n] == '-') {
                                timeEnd = n;
                                for (int j=timeStart;j<timeEnd;j++) { // Date
                                    mapDate += dateTime[j];
                                }
                                timeStart = timeEnd + 1;
                            }
                            if (dateTime[n] == ':') {
                                timeEnd = n;
                                for (int j=timeStart;j<timeEnd;j++) { // Begin
                                    mapBeginTime += dateTime[j];
                                }
                            }
                        }
                        string tempMap;
                        tempMap = mapName + " " + info[1];
                        vector<string> tempMapVector;
                        tempMapVector.push_back(tempMap);
                        tempMapVector.push_back(mapDate);
                        tempMapVector.push_back(mapBeginTime);
                        gameVector.push_back(tempMapVector);
                    }
                    logGameMode = 1;
                } else if ( info[3] == "LogOnlineSession" && logOnlineSession == 0 && line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) {
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
                        string queueDate;
                        string queueTime;
                        string dateTime = info[1];
                        int timeStart = 0;
                        int timeEnd = 0;
                        for (int n=0;n<dateTime.length();n++) { // Assign Date & Begin Time
                            if (dateTime[n] == '-') {
                                timeEnd = n;
                                for (int j=timeStart;j<timeEnd;j++) { // Date
                                    queueDate += dateTime[j];
                                }
                                timeStart = timeEnd + 1;
                            }
                            if (dateTime[n] == ':') {
                                timeEnd = n;
                                for (int j=timeStart;j<timeEnd;j++) { // Begin
                                    queueTime += dateTime[j];
                                }
                            }
                        }
                        if (valid == 1){
                            vector<string> tempQueueVector;
                            tempQueueVector.push_back(queueDate);
                            tempQueueVector.push_back(queueTime);
                            tempQueueVector.push_back(queueSize);
                            queueVector.push_back(tempQueueVector);
                        }
                        else if ( valid == 2){
                            vector<string> tempQueueVector2;
                            tempQueueVector2.push_back(queueDate);
                            tempQueueVector2.push_back(queueTime);
                            tempQueueVector2.push_back(queueSize);
                            queueVector2.push_back(tempQueueVector2);
                        }
                    }
                    logOnlineSession = 1;
                } else if ( info[3] == "LogEasyAntiCheatServer" && logEasyAntiCheatServer == 0 && line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) {
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
                        int timeStart = 0;
                        int timeEnd = 0;
                        for (int n = 0; n < dateTime.length(); n++) { // Assign Date & Begin Time
                            if (dateTime[n] == '-') {
                                timeEnd = n;
                                for (int j = timeStart; j < timeEnd; j++) { // Date
                                    registrationDate += dateTime[j];
                                }
                                timeStart = timeEnd + 1;
                            }
                            if (dateTime[n] == ':') {
                                timeEnd = n;
                                for (int j = timeStart; j < timeEnd; j++) { // Begin
                                    registrationTime += dateTime[j];
                                }
                            }
                        }
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

                } else if (line[i] == ':' && logTime == 1 && logNumber == 1 && logMessage == 1 && logValue == 0) { //LogValue
                    begin = end + 1;
                    end = line.length() - 1;
                    for (int n = begin; n < end; n++) {
                        info[4] += line[n];
                    }
                    logValue = 1;
                }

            }
            info[5] = line;

            /*
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

            if (stoi(info[0]) > 400 && stoi(info[0]) < 450) {
                cout << info[5] << "\n";
                cout << "Line: " << info[0] << " ";
                cout << "logTime: " << info[1] << " ";
                cout << "logNumber: " << info[2] << " ";
                cout << "logMessage: " << info[3] << " ";
                cout << "logValue: " << info[4] << " ";
                 << "\n";
            }cout
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

    /*
    for (int i=0;i<logMessageVector.size();i++) {
        string filename = "c:/Users/Justin/Downloads/Squad/SquadLogs/SQUAD_" + logMessageVector[i][0] + ".txt";
        ofstream eachFile(filename);
        for (int n = 0; n < logMessageVector[i].size(); n++) {
            eachFile << logMessageVector[i][n] << "\n";
            //cout << logMessageVector[i][n] << "\n";
        }
    }
    */

    //Server Statistics'
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
    /*
    int tempSize2 = gameVector.size();
    for (int i=0; i<tempSize2;i++) {
        cout << "Game: " << setw(50) << left << gameVector[i][0] << "  ";
        cout << "Date: " << gameVector[i][1] << "  ";
        cout << "Begin Time: " << gameVector[i][2] << "  ";
        cout << "End Time: " << gameVector[i][3] << "\n";
    }

    int tempSize1 = queueVector.size();
    for (int i=0; i<tempSize1;i++) {
        cout << "Date: " << queueVector[i][0] << "  ";
        cout << "Time: " << queueVector[i][1] << "  ";
        cout << "QueueSize: " << queueVector[i][2] << "\n";
    }
    */

    //int joinCount = 0;
    //int leaveCount = 0;
    //registration Names
    int registrationSize1 = registrationVector.size();
    for (int i = 0; i<registrationSize1;i++){
        if (registrationVector[i][2] == "RegisterClient") {
            //joinCount++;
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

    //cout << "Join Count: " << to_string(joinCount) << "  Leave Count: " << to_string(leaveCount) << "\n";
    /*
    int registrationSize = registrationVector.size();
    for (int i=0; i<registrationSize;i++) {
        cout << "Date: " << registrationVector[i][0] << "  ";
        cout << "Time: " << registrationVector[i][1] << "  ";
        cout << "RegistrationMessage: " << registrationVector[i][2] << "  ";
        cout << "RegistrationPlayerID: " << registrationVector[i][3] << "  ";
        cout << "RegistrationName: " << registrationVector[i][4] << "\n";
    }
     */


    //combine queueVectors
    int combineQueue1 = queueVector.size();
    for(int i = 0; i<combineQueue1;i++) {
        int temp1 = stoi(queueVector[i][2]);
        int temp2 = stoi(queueVector2[i][2]);
        int temp3 = temp1 + temp2;
        queueVector[i].at(2) = to_string(temp3);
    }


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
        //cout << totalTimeSeconds << "\n";
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

    logFile.close();

    /*
    string filename = "c:/Users/Justin/Downloads/Squad/SquadLogs/SQUAD_DerivedDataPerGame.txt";
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
        line += "QueuePeak: " + gameVector[i][6] + " ";
        line += "GameTimeMinutes: " + gameVector[i][7] + "\n";
        derivedFile << line;
    }
     */

}