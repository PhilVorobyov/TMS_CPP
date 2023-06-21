#include <iostream>
#include <string>

enum class GameState {
    IN_PROGRESS,
    WON,
    LOST
};

class WordGuessGame {
private:
    std::string secretWord;
    std::string guessedLetters;
    int remainingAttempts;

public:
    WordGuessGame() {
        Reset();
    }

    void Reset() {
        secretWord = "secret";
        guessedLetters = std::string(secretWord.length(), '*');
        remainingAttempts = 5;
    }

    void Play() {
        std::cout << "Welcome to the Word Guess Game!" << std::endl;
        GameState gameState = GameState::IN_PROGRESS;

        while (gameState == GameState::IN_PROGRESS) {
            std::string guess;
            std::cout << "Enter your guess (" << secretWord.length() << "-letter word): ";
            std::cin >> guess;

            if (guess.length() != secretWord.length()) {
                std::cout << "Invalid guess! Please enter a " << secretWord.length() << "-letter word." << std::endl;
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
                    Reset();
                    continue;
                }
                else {
                    gameState = GameState::WON;
                    break;
                }
            }
            else {
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

                if (remainingAttempts == 0) {
                    gameState = GameState::LOST;
                    break;
                }
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (gameState == GameState::WON) {
            std::cout << "Congratulations! You won the game." << std::endl;
        }
        else if (gameState == GameState::LOST) {
            std::cout << "Game over! You ran out of attempts. The secret word was: " << secretWord << std::endl;
        }
    }
};

int main49() {
    WordGuessGame game;
    game.Play();

    return 0;
}
