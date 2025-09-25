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

3. void updateServer()  
   - Updates the current server depending on score and format.

4. void scorePoint(bool teamAScores)  
   - Awards points, updates service, displays score, and checks set win.

5. void checkSetWin()  
   - Checks if a set is won, updates set scores, increments setsWon.

6. void checkMatchWin()  
   - Checks if the match is won (best of 3 sets).

7. void displayCurrentScore()  
   - Prints the live score of the current set.

8. void displayMatchStatus()  
   - Displays sets won, scores from past sets, and current server.

9. void displayFinalScore()  
   - Shows final results after the match ends.

10. void playMatch()  
    - Game loop: keeps asking who scores until someone wins 2 sets.

11. bool askPlayAgain()  
    - Asks the user if they want to play another match (y/n).

12. void resetMatch()  
    - Resets all match data to start fresh for a new game.

----------------------------------------
Global (outside class):
----------------------------------------

13. int main()  
    - Entry point. Runs matches in a loop until the user chooses not to play again.
*/

#include <iostream>
#include <string>
#include <stdexcept>

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
    
public:
    BadmintonMatch() {
        currentSetA = 0;
        currentSetB = 0;
        setsWonA = 0;
        setsWonB = 0;
        currentSet = 1;
        serviceA = true;
        
        // Initialize set scores
        set1ScoreA = 0; set1ScoreB = 0;
        set2ScoreA = 0; set2ScoreB = 0;
        set3ScoreA = 0; set3ScoreB = 0;
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
                currentServer = players[0];
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
                currentServer = teamA[0];
            }
            
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
            // Doubles: service alternates between players
            cout << "Service: " << currentServer << endl;
        }
    }
    
    void scorePoint(bool teamAScores) {
        if (teamAScores) {
            currentSetA ++; // Increment by 1 point
            cout << "\n1 Point to ";
            if (isSingles) {
                cout << players[0];
            } else {
                cout << "Team A";
            }
            cout << "!" << endl;
            
            // Service continues if serving side scores
            if (!serviceA) {
                // Service changes to team A
                serviceA = true;
                if (isSingles) {
                    currentServer = players[0];
                } else {
                    currentServer = teamA[0];
                }
            }
        } else {
            currentSetB ++; // Increment by 1 point
            cout << "\n1 Point to ";
            if (isSingles) {
                cout << players[1];
            } else {
                cout << "Team B";
            }
            cout << "!" << endl;
            
            // Service continues if serving side scores
            if (serviceA) {
                // Service changes to team B
                serviceA = false;
                if (isSingles) {
                    currentServer = players[1];
                } else {
                    currentServer = teamB[0];
                }
            }
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
            cout << "Restarting program....." << endl;
        }
    }
    
    cout << "\nThank you for playing!" << endl;
    return 0;
}
