#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

class Player {
private:
    string name;
    int balance;
public:
    Player(string n, int b = 1000) : name(n), balance(b) {}

    string getName() const { return name; }
    int getBalance() const { return balance; }

    void updateBalance(int amount) {
        balance += amount;
        if (balance < 0) balance = 0; // prevent negative balance
    }

    void displayStatus() const {
        cout << endl;
        cout << "Player: " << name << " | Balance: $" << balance << endl;
        cout << endl;
    }
};

class CasinoGame {
private:
    Player &player;
    map<string, pair<int,int>> difficultyLevels; 
public:
    CasinoGame(Player &p) : player(p) {
        difficultyLevels["Easy"]   = {5, 100};
        difficultyLevels["Medium"] = {10, 300};
        difficultyLevels["Hard"]   = {20, 1000};
    }

    void showRules() {
        cout << endl;
        cout << "Welcome to the Casino Number Guessing Game!" << endl << endl;
        cout << "Rules:" << endl;
        cout << "1. Choose a difficulty level: Easy, Medium, or Hard." << endl;
        cout << "2. The computer will pick a random number in a given range:" << endl;
        cout << "   - Easy: Guess between 1 and 5 (Prize: $100)" << endl;
        cout << "   - Medium: Guess between 1 and 10 (Prize: $300)" << endl;
        cout << "   - Hard: Guess between 1 and 20 (Prize: $1000)" << endl;
        cout << "3. If you guess correctly, you win the prize money!" << endl;
        cout << "4. If you guess wrong, you lose half the prize amount." << endl;
        cout << "5. Your balance starts at $1000. Try to maximize it!" << endl;
        cout << endl << "Good luck!" << endl << endl;
    }

    void play(const string& difficulty) {
        if (difficultyLevels.find(difficulty) == difficultyLevels.end()) {
            cout << endl << "Invalid difficulty level!" << endl << endl;
            return;
        }

        int range = difficultyLevels[difficulty].first;
        int prize = difficultyLevels[difficulty].second;

        int secretNumber = rand() % range + 1;
        int guess;

        cout << endl << "Guess a number between 1 and " << range << ": ";
        cin >> guess;

        if (guess == secretNumber) {
            cout << endl << "Congratulations! You guessed correctly!" << endl;
            player.updateBalance(prize);
        } else {
            cout << endl << "Wrong guess. The correct number was " << secretNumber << "." << endl;
            player.updateBalance(-prize/2);
        }

        player.displayStatus();
    }
};

// Load leaderboard from file
void loadLeaderboard(vector<Player>& leaderboard) {
    ifstream file("leaderboard.txt");
    if (!file) return; // no file yet

    string name;
    int balance;
    while (file >> name >> balance) {
        leaderboard.emplace_back(name, balance);
    }
    file.close();
}

// Save leaderboard back to the same file
void saveLeaderboard(vector<Player>& leaderboard) {
    sort(leaderboard.begin(), leaderboard.end(), [](const Player& a, const Player& b) {
        return a.getBalance() > b.getBalance();
    });

    ofstream file("leaderboard.txt");
    if (!file) {
        cerr << "Error: Could not open leaderboard file for writing." << endl;
        return;
    }

    for (size_t i = 0; i < leaderboard.size(); i++) {
        file << leaderboard[i].getName() << " " << leaderboard[i].getBalance() << endl;
    }

    file.close();

    cout << endl << "Leaderboard updated in leaderboard.txt" << endl << endl;
}

// Display leaderboard nicely
void showLeaderboard(vector<Player>& leaderboard) {
    cout << endl;
    cout << "==============================================" << endl;
    cout << "                 LEADERBOARD                  " << endl;
    cout << "==============================================" << endl;

    sort(leaderboard.begin(), leaderboard.end(), [](const Player& a, const Player& b) {
        return a.getBalance() > b.getBalance();
    });

    for (size_t i = 0; i < leaderboard.size(); i++) {
        cout << i+1 << ". " << leaderboard[i].getName() 
             << " - $" << leaderboard[i].getBalance() << endl;
    }

    cout << "==============================================" << endl << endl;
}

int main() {
    srand(time(0));

    vector<Player> leaderboard;
    loadLeaderboard(leaderboard); // load existing scores

    char playAgainPlayers = 'y';
    while (playAgainPlayers == 'y' || playAgainPlayers == 'Y') {
        cout << endl;
        cout << "==============================================" << endl;
        cout << "         CASINO NUMBER GUESSING GAME          " << endl;
        cout << "==============================================" << endl;
        cout << endl;

        Player tempPlayer("Temp");
        CasinoGame tempGame(tempPlayer);
        tempGame.showRules();

        string name;
        cout << "Enter your name: ";
        cin >> name;
        cout << endl;

        Player player(name);
        CasinoGame game(player);

        char choice = 'y';
        while (choice == 'y' || choice == 'Y') {
            cout << "Choose difficulty (Easy/Medium/Hard): ";
            string difficulty;
            cin >> difficulty;

            game.play(difficulty);

            cout << "Do you want to play again? (y/n): ";
            cin >> choice;
            cout << endl;
        }

        cout << "Thanks for playing, " << player.getName() << "! Final Balance: $" 
             << player.getBalance() << endl;

        leaderboard.push_back(player);

        cout << endl;
        cout << "Would another player like to try? (y/n): ";
        cin >> playAgainPlayers;
    }

    showLeaderboard(leaderboard);
    saveLeaderboard(leaderboard);

    return 0;
}