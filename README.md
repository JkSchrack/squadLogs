squadLogs - Justin Schrack August 2020

Tested using Server Logs of Bellas Battleground July 25, 2020 - August 10, 2020
Program performs four operations to help decipher information from Squad Server Log Files.


Input: Use Absolute File Paths for all requested Files or Folders.
Output: Text Files or Metrics via the Terminal

1) Breakdown a Server Log File into seperate files based on their initial log message.
    Example Input: C:\Users\Justin\Downloads\Squad\SquadGame-backup-2020.07.25-09.37.31.log

2) Provide information for each game/map played consisting of the Map, Gamemode, Date Played, Average Queue, Queue Peak, Average Player Count and Game Time.
    Example Input: C:\Users\Justin\Downloads\Squad\SquadGame-backup-2020.07.25-09.37.31.log

3) Save the information from Operation #2 into a text file.
    Example Input: C:\Users\Justin\Downloads\Squad\SquadGame-backup-2020.07.25-09.37.31.log && c:/Users/Justin/Downloads/Squad/SQUAD_DerivedData.txt 

4) Analyze Previous Saved Data and provide the Averages for each Metric per Map & Gamemode alongside Play Dates & Times for each match.
    Example Input: c:/Users/Justin/Downloads/Squad/SQUAD_DerivedData.txt && C:\Users\Justin\Downloads\Squad\SQUAD_Analyzed.txt


