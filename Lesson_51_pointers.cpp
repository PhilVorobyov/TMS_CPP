#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

enum class GameState {
    IN_PROGRESS,
    WON,
    LOST
};

class WordGuessGame {
private:
    std::string* secretWords;
    int numWords;
    std::string secretWord;
    std::string guessedLetters;
    int remainingAttempts;
    bool initialized;

public:
    WordGuessGame(std::string* words, int count) {
        secretWords = words;
        numWords = count;
        initialized = false;
        Reset();
    }


    void Reset() {
        srand(static_cast<unsigned int>(time(0)));
        int randomIndex = rand() % numWords;
        secretWord = secretWords[randomIndex];
        guessedLetters = std::string(secretWord.length(), '*');
        remainingAttempts = 4;
        initialized = true;
    }

    void Play() {
        std::cout << "Welcome to the Word Guess Game!" << std::endl;
        GameState gameState = GameState::IN_PROGRESS;

        while (gameState == GameState::IN_PROGRESS) {
            std::string guess = GetGuess();
            if (!IsValidGuess(guess)) {
                continue;
            }

            if (IsCorrectGuess(guess)) {
                HandleCorrectGuess();
                if (!AskPlayAgain()) {
                    gameState = GameState::WON;
                }
                else {
                    Reset();
                }
            }
            else {
                HandleIncorrectGuess(guess);
                if (IsGameLost()) {
                    if (!AskPlayAgain()) {
                        gameState = GameState::LOST;
                    }
                    else {
                        Reset();
                    }
                }
            }
        }

        PrintGameResult(gameState);
    }

private:
    std::string GetGuess() {
        std::string guess;
        std::cout << "Enter your guess (" << secretWord.length() << "-letter word): ";
        std::cin >> guess;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return guess;
    }

    bool IsValidGuess(const std::string& guess) {
        if (guess.length() != secretWord.length()) {
            std::cout << "Invalid guess! Please enter a " << secretWord.length() << "-letter word." << std::endl;
            return false;
        }
        return true;
    }

    bool IsCorrectGuess(const std::string& guess) {
        return guess == secretWord;
    }

    void HandleCorrectGuess() {
        std::cout << "Congratulations! You guessed the word correctly." << std::endl;
        Reset();
    }

    bool AskPlayAgain() {
        char playAgain;
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return (playAgain == 'y' || playAgain == 'Y');
    }

    void HandleIncorrectGuess(const std::string& guess) {
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
    }

    bool IsGameLost() {
        return (remainingAttempts == 0);
    }

    void PrintGameResult(GameState gameState) {
        if (gameState == GameState::WON) {
            std::cout << "Congratulations! You won the game." << std::endl;
        }
        else if (gameState == GameState::LOST) {
            std::cout << "Game over! You ran out of attempts. The secret word was: " << secretWord << std::endl;
        }
    }
};

int main() {
    std::string words[] = { "apple", "banana", "orange", "melon" };
    int numWords = sizeof(words) / sizeof(words[0]);

    WordGuessGame* game = new WordGuessGame(words, numWords);
    game->Play();

    return 0;
}
 