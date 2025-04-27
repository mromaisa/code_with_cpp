#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    
    string words[] = {"canada", "england", "uganda", "unicorn", "rainbow"};
    int wordCount = sizeof(words) / sizeof(words[0]);

   
    srand(time(0));
    string chosenWord = words[rand() % wordCount];
    string guessedWord(chosenWord.length(), '_');

    int attempts = 5;
    char guess;

    cout << "                              --- Word Guessing Game ---\n";
    cout << "                           Guess the word letter by letter.\n\n";

    while (attempts > 0 && guessedWord != chosenWord) {
        cout << "                            Word: " << guessedWord << "\n";
        cout << "                            Attempts left: " << attempts << "\n";
        cout << "                            Enter a letter: ";
        cin >> guess;

        bool found = false;
        for (size_t i = 0; i < chosenWord.length(); i++) {
            if (tolower(guess) == tolower(chosenWord[i]) && guessedWord[i] == '_') {
                guessedWord[i] = chosenWord[i];
                found = true;
            }
        }

        if (!found) {
            cout << "                          Wrong guess!\n";
            attempts--;
        } else {
            cout << "                          Correct guess!\n";
        }
    }

    if (guessedWord == chosenWord) {
        cout << "                              Congratulations! You guessed the word: " << chosenWord << "\n";
    } else {
        cout << "                              Game Over! The word was: " << chosenWord << "\n";
    }

    return 0;
}