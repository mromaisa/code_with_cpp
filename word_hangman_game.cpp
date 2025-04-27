#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    // Words storage
    string words[] = {"apple", "banana", "cherry", "date", "elderberry"};

    // Initialize to a random word
    srand(time(0));
    string word = words[rand() % 5];

    // Initialize a variable which stores all the guessed letters at right places also displace the unguessed places
    string guess(word.size(), '_');

    int attempts = word.length() + 1;
    // start the game
    cout << "Welcome to the Word Hangman Game!\n";
    cout << "You have " << attempts << " attempts to guess the word.\n";
    cout << "The word has " << word.length() << " letters.\n";

    // display the initial guessed word
    for (char c : guess)
    {
        cout << c << " ";
    }

    // Game Logic
    char letter;

    while (attempts > 0 && guess != word)
    {
    // a goto label
    start:

        // Game Input
        cout << "\nEnter Your Letter: ";
        cin >> letter;

        if (isalpha(letter))
        {

            // declaring variables to check wrong guess and also the letters that are already been guessed
            bool found = false;
            bool already_guessed = false;

            // loop to check the letter in the word
            for (int i = 0; i < word.length(); i++)
            {
                if (letter == word[i] && guess[i] == '_')
                {
                    guess[i] = letter;
                    found = true;
                }
                else if (letter == word[i] && guess[i] != '_')
                {
                    already_guessed = true;
                }
            }

            // This step is important since if this is included inside the for loop then the the loop will check only the first letter of the word and not the whole word
            if (already_guessed)
            {
                cout << "You already guessed the letter.\n";
                attempts--;
                cout << "You have " << attempts << " attempts left.\n";
            }
            else if (!found)
            {
                cout << "You guessed the wrong letter.\n";
                attempts--;
                cout << "You have " << attempts << " attempts left.\n";
            }
            else
            {
                cout << "Good guess !! Keep Going.\n";
            }

            // update the guessed word
            for (char c : guess)
            {
                cout << c << " ";
            }
        }
        else
        {
            cout << "\nInvalid input. Please enter a letter." << endl;
            goto start;
        }
    }

    // Game conclusion
    if (guess == word)
    {
        cout << "\n\nCongratulations! You guessed the word: " << word << endl;
    }
    else
    {
        cout << "\n\nSorry, you lost. \nThe word was: " << word << endl;
    }

    return 0;
}
