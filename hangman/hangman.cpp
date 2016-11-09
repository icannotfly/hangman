#include <iostream>
#include <string>
#include <ctime> //for seeding srand

#define WORDLIST_LENGTH 20
#define MAX_INCORRECT_GUESSES 6

using namespace std;

enum EGameState : int
{
	WaitingToStart,
	InProgress,
	Over
};

//draws the current state of the hangman
void DrawHangman(int IncorrectGuesses)
{
	//draw hangman
	char Face = (IncorrectGuesses > 0) ? 'O' : ' ';
	char Torso = (IncorrectGuesses > 1) ? '|' : ' ';
	char ArmR = (IncorrectGuesses > 2) ? '/' : ' ';
	char ArmL = (IncorrectGuesses > 3) ? '\\' : ' ';
	char LegR = (IncorrectGuesses > 4) ? '/' : ' ';
	char LegL = (IncorrectGuesses > 5) ? '\\' : ' ';
	cout << endl
		<< "\t   +----+" << endl
		<< "\t   |    |" << endl
		<< "\t   " << Face << "    |" << endl
		<< "\t  " << ArmR << Torso << ArmL << "   |" << endl
		<< "\t  " << LegR << " " << LegL << "   |" << endl
		<< "\t        |" << endl
		<< "\t==========" << endl
		<< endl;
}

//print guesses
void PrintGuesses(string &ChosenWord, string &CorrectGuesses, string &IncorrectGuesses)
{
	cout << "Word: ";
	for (int i = 0; i < ChosenWord.length(); i++)
	{
		if (CorrectGuesses.find(ChosenWord[i]) != string::npos)
		{
			cout << ChosenWord[i] << " ";
		}
		else
		{
			cout << "_ ";
		}
	}
	cout << endl << endl;

	cout << "Incorrect Guesses: " << endl;
	for (int i = 0; i < IncorrectGuesses.length(); i++)
	{
		cout << IncorrectGuesses[i] << " ";
	}
	cout << endl;
}

//determines if the player's guess was correct or not
void HandlePlayerGuess(char PlayerGuess, string ChosenWord, string *CorrectGuesses, string *IncorrectGuesses)
{
	//see if we've guessed this letter already
	if (CorrectGuesses->find(PlayerGuess) != string::npos || IncorrectGuesses->find(PlayerGuess) != string::npos)
	{
		cout << "You've already guessed " << PlayerGuess << "!" << endl;
		cout << "Try again." << endl;
	}
	else if (ChosenWord.find(PlayerGuess) != string::npos)
	{
		//letter is in word
		cout << "Correct! There is a " << PlayerGuess << " in the word." << endl;
		*CorrectGuesses += PlayerGuess;
	}
	else
	{
		//letter is not in word
		cout << "WROOONG" << endl;
		*IncorrectGuesses += PlayerGuess;
	}
}

int main()
{
	cout << "Hangman!" << endl << endl;

	EGameState GameState = EGameState::WaitingToStart;

	//20 most common english words over 3 letters long
	string Worldlist[WORDLIST_LENGTH] = { "time", "year", "people", "thing", "woman", "life", "child", "world", "school", "state", "family", "student", "group", "country", "problem", "hand", "part", "place", "case", "week" };

	//choose a random word within that list
	srand(time(0));
	string ChosenWord = Worldlist[rand() % int(WORDLIST_LENGTH)];
	string CorrectGuesses;
	string IncorrectGuesses;
	
	//start game
	GameState = EGameState::InProgress;
	bool bWonLastGame = false;

	//game loop
	while (GameState == InProgress)
	{
		system("cls"); //windows-only, sorry;

		//update the player on how they're doing
		DrawHangman(IncorrectGuesses.length());
		PrintGuesses(ChosenWord, CorrectGuesses, IncorrectGuesses);

		//cout << "    ((DEBUG)) word is: " << ChosenWord << endl;

		cout << "Make a guess: ";
		string PlayerGuess;
		getline(cin, PlayerGuess);

		HandlePlayerGuess(PlayerGuess[0], ChosenWord, &CorrectGuesses, &IncorrectGuesses);

		if (IncorrectGuesses.length() >= MAX_INCORRECT_GUESSES)
		{
			GameState = EGameState::Over;
			bWonLastGame = false;
		}
		else if (CorrectGuesses.length() == ChosenWord.length())
		{
			GameState = EGameState::Over;
			bWonLastGame = true;
		}
	}

	//draw end screen
	system("cls");
	DrawHangman(IncorrectGuesses.length());
	cout << endl << "Game over: " << (bWonLastGame ? "You won!" : "You lost.") << endl;
	if (!bWonLastGame)
	{
		cout << "The word was \"" << ChosenWord << "\"." << endl;
	}

	cout << "Want to try again? ";

	cout << endl << endl;
}
