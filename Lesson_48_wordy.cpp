#include <iostream>
#include <string>

void Reset(std::string& secretWord, std::string& guessedLetters, int& remainingAttempts) {
    secretWord = "secret";
    guessedLetters = std::string(secretWord.length(), '*');
    remainingAttempts = 5;
}

int maina123() {
    std::string secretWord;
    std::string guessedLetters;
    int remainingAttempts;

    std::cout << "Welcome to the Word Guess Game!" << std::endl;
    Reset(secretWord, guessedLetters, remainingAttempts);

    while (remainingAttempts > 0) {
        std::string guess;
        std::cout << "Enter your guess (6-letter word): ";
        std::cin >> guess;

        if (guess.length() != secretWord.length()) {
            std::cout << "Invalid guess! Please enter a 6-letter word." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (guess == secretWord) {
            std::cout << "Congratulations! You guessed the word correctly." << std::endl;

            char playAgain;
            std::cout << "Play again? (y/n): ";
            std::cin >> playAgain;

            if (playAgain == 'y' || playAgain == 'Y') {
                Reset(secretWord, guessedLetters, remainingAttempts);
                continue;
            }
            else {
                break;
            }
        }

        int correctLetters = 0;
        int correctPositions = 0;

        for (int i = 0; i < secretWord.length(); i++) {
            if (guess[i] == secretWord[i]) {
                correctPositions++;
                guessedLetters[i] = secretWord[i];
            }
            else if (secretWord.find(guess[i]) != std::string::npos) {
                correctLetters++;
            }
        }

        std::cout << "Correct letters: " << correctLetters << std::endl;
        std::cout << "Correct positions: " << correctPositions << std::endl;
        std::cout << "Guessed letters: " << guessedLetters << std::endl;

        remainingAttempts--;
        std::cout << "Attempts remaining: " << remainingAttempts << std::endl;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Game over! You ran out of attempts. The secret word was: " << secretWord << std::endl;

    return 0;
}
