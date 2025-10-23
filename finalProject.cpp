/*
    Project Name: Badminton Scoring Program
    Group Number: 2
    Group Members:
    - Garduque, Divine Grace G.
    - Kalaw, Zoe Ulrik L.
    - Palisoc, Maria Raychelle R.
    - Sangahin, Ree Anne P.
*/

/*
========================================
BadmintonMatch Class - Function List
========================================

1. BadmintonMatch()  
   - Constructor, initializes all scores and sets the first server.

2. void setupGame()  
   - Asks if Singles or Doubles, reads names, sets up the match.

3. void coinToss()
   - Performs a coin toss to determine which side serves first.

4. void updateServer()  
   - Updates the current server depending on score and format.

5. void scorePoint(bool teamAScores)  
   - Awards points, updates service, displays score, and checks set win.

6. void checkSetWin()  
   - Checks if a set is won, updates set scores, increments setsWon.

7. void checkMatchWin()  
   - Checks if the match is won (best of 3 sets).

8. void displayCurrentScore()  
   - Prints the live score of the current set.

9. void displayMatchStatus()  
   - Displays sets won, scores from past sets, and current server.

10. void displayFinalScore()  
    - Shows final results after the match ends.

11. void playMatch()  
    - Game loop: keeps asking who scores until someone wins 2 sets.

12. bool askPlayAgain()  
    - Asks the user if they want to play another match (y/n).

13. void resetMatch()  
    - Resets all match data to start fresh for a new game.

----------------------------------------
Global (outside class):
----------------------------------------

14. int main()  
    - Entry point. Runs matches in a loop until the user chooses not to play again.
*/

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

class BadmintonMatch {
private:
    // Game format
    bool isSingles;
    
    // Player/Team names 
    string players[2];        // For singles: [0]=PlayerA, [1]=PlayerB
    string teamA[2];         // For doubles: Team A players
    string teamB[2];         // For doubles: Team B players
    
    // Match scoring 
    int set1ScoreA, set1ScoreB;
    int set2ScoreA, set2ScoreB;
    int set3ScoreA, set3ScoreB;
    int currentSetA, currentSetB;
    int setsWonA, setsWonB;
    int currentSet; // 1, 2, or 3
    
    // Service tracking
    bool serviceA; // true if A serves, false if B serves
    string currentServer;
    
    // Doubles rotation tracking
    int serverIndexA; // 0 or 1 for teamA
    int serverIndexB; // 0 or 1 for teamB
    
public:
    BadmintonMatch() {
        currentSetA = 0;
        currentSetB = 0;
        setsWonA = 0;
        setsWonB = 0;
        currentSet = 1;
        serviceA = true;
        serverIndexA = 0;
        serverIndexB = 0;
        
        // Initialize set scores
        set1ScoreA = 0; set1ScoreB = 0;
        set2ScoreA = 0; set2ScoreB = 0;
        set3ScoreA = 0; set3ScoreB = 0;
        
        // Seed random number generator
        srand(time(0));
    }
    
    void coinToss() {
        cout << "\n=== COIN TOSS ===" << endl;
        cout << "Flipping coin to determine first server..." << endl;
        cout << "Press Enter to flip the coin...";
        cin.ignore();
        cin.get();
        
        // Generate random number (0 or 1)
        int tossResult = rand() % 2;
        
        cout << "\nCoin flipping..." << endl;
        cout << "..." << endl;
        
        if (tossResult == 0) {
            serviceA = true;
            if (isSingles) {
                currentServer = players[0];
                cout << "\nResult: " << players[0] << " wins the toss and will serve first!" << endl;
            } else {
                currentServer = teamA[0];
                serverIndexA = 0;
                cout << "\nResult: Team A wins the toss and will serve first!" << endl;
                cout << "First server: " << teamA[0] << endl;
            }
        } else {
            serviceA = false;
            if (isSingles) {
                currentServer = players[1];
                cout << "\nResult: " << players[1] << " wins the toss and will serve first!" << endl;
            } else {
                currentServer = teamB[0];
                serverIndexB = 0;
                cout << "\nResult: Team B wins the toss and will serve first!" << endl;
                cout << "First server: " << teamB[0] << endl;
            }
        }
        
        cout << "\nPress Enter to start the match...";
        cin.get();
    }
    
    void setupGame() {
        cout << "=== BADMINTON SCORING PROGRAM ===" << endl;
        cout << "Choose game format:" << endl;
        cout << "1. Singles (1 player per side)" << endl;
        cout << "2. Doubles (2 players per side)" << endl;
        
        int choice;
        try {
            cin >> choice;
            if (cin.fail() || (choice != 1 && choice != 2)) {
                throw invalid_argument("Invalid input! Please enter 1 or 2.");
            }
            cin.ignore(); // Clear newline
            
            if (choice == 1) {
                isSingles = true;
                cout << "\nEnter Player A name: ";
                getline(cin, players[0]);
                cout << "Enter Player B name: ";
                getline(cin, players[1]);
            } else {
                isSingles = false;
                cout << "\nEnter Team A Player 1 name: ";
                getline(cin, teamA[0]);
                cout << "Enter Team A Player 2 name: ";
                getline(cin, teamA[1]);
                cout << "\nEnter Team B Player 1 name: ";
                getline(cin, teamB[0]);
                cout << "Enter Team B Player 2 name: ";
                getline(cin, teamB[1]);
            }
            
            // Perform coin toss to determine first server
            coinToss();
            
            cout << "\n=== MATCH STARTING ===" << endl;
            displayMatchStatus();
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please try again.\n" << endl;
            setupGame(); // Try again
        }
    }
    
    void updateServer() {
        if (isSingles) {
            // Singles: serve from right if score is even, left if odd
            if (serviceA) {
                string side = (currentSetA % 2 == 0) ? "right" : "left";
                cout << currentServer << " serves from the " << side << " service court." << endl;
            } else {
                string side = (currentSetB % 2 == 0) ? "right" : "left";
                cout << currentServer << " serves from the " << side << " service court." << endl;
            }
        } else {
            // Doubles: serve from right if even, left if odd
            if (serviceA) {
                string side = (currentSetA % 2 == 0) ? "right" : "left";
                cout << "Service: " << currentServer << " (Team A) serves from the " << side << " court." << endl;
            } else {
                string side = (currentSetB % 2 == 0) ? "right" : "left";
                cout << "Service: " << currentServer << " (Team B) serves from the " << side << " court." << endl;
            }
        }
    }
    
    void scorePoint(bool teamAScores) {
        if (teamAScores) {
            currentSetA++; // Increment by 1 point
            cout << "\n1 Point to ";
            if (isSingles) {
                cout << players[0];
            } else {
                cout << "Team A";
            }
            cout << "!" << endl;
            
            // Service handling
            if (!serviceA) {
                // Service changes to team A (receiving team won)
                serviceA = true;
                if (isSingles) {
                    currentServer = players[0];
                } else {
                    // In doubles, when serving team loses, they rotate
                    serverIndexB = (serverIndexB == 0) ? 1 : 0;
                    // Receiving team that won becomes serving team
                    currentServer = teamA[serverIndexA];
                }
            }
            // If Team A was already serving and scores, they continue (no rotation)
            
        } else {
            currentSetB++; // Increment by 1 point
            cout << "\n1 Point to ";
            if (isSingles) {
                cout << players[1];
            } else {
                cout << "Team B";
            }
            cout << "!" << endl;
            
            // Service handling
            if (serviceA) {
                // Service changes to team B (receiving team won)
                serviceA = false;
                if (isSingles) {
                    currentServer = players[1];
                } else {
                    // In doubles, when serving team loses, they rotate
                    serverIndexA = (serverIndexA == 0) ? 1 : 0;
                    // Receiving team that won becomes serving team
                    currentServer = teamB[serverIndexB];
                }
            }
            // If Team B was already serving and scores, they continue (no rotation)
        }
        
        updateServer();
        displayCurrentScore();
        
        // Check if set is won
        checkSetWin();
    }
    
    void checkSetWin() {
        bool setWon = false;
        
        // Standard win condition: first to 21, must win by 2
        if (currentSetA >= 21 || currentSetB >= 21) {
            if (currentSetA - currentSetB >= 2 || currentSetB - currentSetA >= 2) {
                setWon = true;
            }
        }
        
        // Special rule: if 29-29, first to 30 wins
        if (currentSetA == 29 && currentSetB == 29) {
            cout << "\n*** DEUCE at 29-29! First to 30 wins! ***" << endl;
        }
        if (currentSetA == 30 || currentSetB == 30) {
            setWon = true;
        }
        
        if (setWon) {
            // Save current set scores
            if (currentSet == 1) {
                set1ScoreA = currentSetA;
                set1ScoreB = currentSetB;
            } else if (currentSet == 2) {
                set2ScoreA = currentSetA;
                set2ScoreB = currentSetB;
            } else if (currentSet == 3) {
                set3ScoreA = currentSetA;
                set3ScoreB = currentSetB;
            }
            
            if (currentSetA > currentSetB) {
                setsWonA++;
                cout << "\n*** SET " << currentSet << " WON by ";
                if (isSingles) cout << players[0];
                else cout << "Team A";
                cout << " (" << currentSetA << "-" << currentSetB << ") ***" << endl;
            } else {
                setsWonB++;
                cout << "\n*** SET " << currentSet << " WON by ";
                if (isSingles) cout << players[1];
                else cout << "Team B";
                cout << " (" << currentSetA << "-" << currentSetB << ") ***" << endl;
            }
            
            // Reset for next set
            currentSetA = 0;
            currentSetB = 0;
            currentSet++;
            
            // Reset server indices for doubles
            serverIndexA = 0;
            serverIndexB = 0;
            
            // Check match winner
            checkMatchWin();
        }
    }
    
    void checkMatchWin() {
        // Best of 3 sets - first to win 2 sets wins the match
        if (setsWonA == 2 || setsWonB == 2) {
            cout << "\n======================================" << endl;
            cout << "*** MATCH COMPLETED ***" << endl;
            
            if (setsWonA == 2) {
                cout << "WINNER: ";
                if (isSingles) cout << players[0];
                else cout << "Team A (" << teamA[0] << " & " << teamA[1] << ")";
            } else {
                cout << "WINNER: ";
                if (isSingles) cout << players[1];
                else cout << "Team B (" << teamB[0] << " & " << teamB[1] << ")";
            }
            
            cout << endl;
            displayFinalScore();
            cout << "\nCongratulations on a great match!" << endl;
            cout << "======================================" << endl;
        } else if (currentSet <= 3) {
            cout << "\nStarting Set " << currentSet << "..." << endl;
            displayMatchStatus();
        }
    }
    
    void displayCurrentScore() {
        cout << "\nCurrent Score: ";
        if (isSingles) {
            cout << players[0] << " " << currentSetA << " - " << currentSetB << " " << players[1];
        } else {
            cout << "Team A " << currentSetA << " - " << currentSetB << " Team B";
        }
        cout << endl;
    }
    
    void displayMatchStatus() {
        cout << "\n----------------------------------------" << endl;
        cout << "MATCH STATUS - Set " << currentSet << endl;
        cout << "Sets Won: ";
        if (isSingles) {
            cout << players[0] << " " << setsWonA << " - " << setsWonB << " " << players[1];
        } else {
            cout << "Team A " << setsWonA << " - " << setsWonB << " Team B";
        }
        cout << endl;
        
        // Show previous set scores
        if (currentSet > 1) {
            cout << "Set 1: " << set1ScoreA << "-" << set1ScoreB;
            if (currentSet > 2) {
                cout << ", Set 2: " << set2ScoreA << "-" << set2ScoreB;
            }
            cout << endl;
        }
        
        displayCurrentScore();
        cout << "Current Server: " << currentServer << endl;
        cout << "----------------------------------------" << endl;
    }
    
    void displayFinalScore() {
        cout << "\nFINAL MATCH SCORE:" << endl;
        cout << "Sets Won: ";
        if (isSingles) {
            cout << players[0] << " " << setsWonA << " - " << setsWonB << " " << players[1];
        } else {
            cout << "Team A " << setsWonA << " - " << setsWonB << " Team B";
        }
        cout << endl;
        
        cout << "Set Scores: ";
        cout << "Set 1: " << set1ScoreA << "-" << set1ScoreB;
        if (set2ScoreA > 0 || set2ScoreB > 0) {
            cout << ", Set 2: " << set2ScoreA << "-" << set2ScoreB;
        }
        if (set3ScoreA > 0 || set3ScoreB > 0) {
            cout << ", Set 3: " << set3ScoreA << "-" << set3ScoreB;
        }
        cout << endl;
    }
    
    void playMatch() {
        while (setsWonA < 2 && setsWonB < 2) {
            try {
                cout << "\nWho scores the next point?" << endl;
                if (isSingles) {
                    cout << "1. " << players[0] << endl;
                    cout << "2. " << players[1] << endl;
                } else {
                    cout << "1. Team A" << endl;
                    cout << "2. Team B" << endl;
                }
                cout << "Choice (1 or 2): ";
                
                int choice;
                cin >> choice;
                
                if (cin.fail() || (choice != 1 && choice != 2)) {
                    throw invalid_argument("Invalid input! Please enter 1 or 2.");
                }
                
                if (choice == 1) {
                    scorePoint(true);
                } else {
                    scorePoint(false);
                }
            }
            catch (const invalid_argument& e) {
                cout << "Error: " << e.what() << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Please try again." << endl;
            }
        }
    }
    
    bool askPlayAgain() {
        char choice;
        try {
            cout << "\nDo you want to play another match? (y/n): ";
            cin >> choice;
            
            if (cin.fail()) {
                throw invalid_argument("Invalid input!");
            }
            
            if (choice == 'y' || choice == 'Y') {
                return true;
            } else if (choice == 'n' || choice == 'N') {
                return false;
            } else {
                throw invalid_argument("Please enter 'y' for yes or 'n' for no.");
            }
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            return askPlayAgain(); // Try again
        }
    }
    
    void resetMatch() {
        // Reset all scores and variables for a new match
        currentSetA = 0;
        currentSetB = 0;
        setsWonA = 0;
        setsWonB = 0;
        currentSet = 1;
        serviceA = true;
        serverIndexA = 0;
        serverIndexB = 0;
        
        // Reset set scores
        set1ScoreA = 0; set1ScoreB = 0;
        set2ScoreA = 0; set2ScoreB = 0;
        set3ScoreA = 0; set3ScoreB = 0;
        
        // Reset server
        if (isSingles) {
            currentServer = players[0];
        } else {
            currentServer = teamA[0];
        }
    }

};

int main() {
    bool playAgain = true;
    
    while (playAgain) {
        try {
            BadmintonMatch match;
            
            match.setupGame();
            match.playMatch();
            
            playAgain = match.askPlayAgain();
            
            if (playAgain) {
                cout << "\n" << string(50, '=') << endl;
                cout << "Starting a new match..." << endl;
                cout << string(50, '=') << endl;
            }
        }
        catch (const exception& e) {
            cout << "An unexpected error occurred: " << e.what() << endl;
            cout << "Restarting program..." << endl;
        }
    }
    
    cout << "\n\n======================================================================\n";
    cout << "                        THANK YOU FOR PLAYING!                          \n";
    cout << "                      Badminton Scoring Program                         \n";
    cout << "                          Group 2 - CSS121P                              \n";
    cout << "======================================================================\n\n";
    return 0;
}
