#include <iostream>
#include <string>
#include <ctime> //for seeding srand

#define WORDLIST_LENGTH 40
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
void HandlePlayerGuess(char PlayerGuess, string ChosenWord, string *CorrectGuesses, string *IncorrectGuesses, string *StatusMessage)
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
		*StatusMessage = "Correct! There is a ";
		*CorrectGuesses += PlayerGuess;
	}
	else
	{
		//letter is not in word
		*StatusMessage = "Nope! There are no ";
		*IncorrectGuesses += PlayerGuess;
	}

	*StatusMessage += PlayerGuess;
	*StatusMessage += " in the word.";
}

int main()
{
	cout << endl
		<< "    ==========" << endl
		<< "     Hangman!" << endl
		<< "    ==========" << endl << endl;

	EGameState GameState = EGameState::WaitingToStart;

	//40 most common english nouns over 3 letters long
	string Worldlist[WORDLIST_LENGTH] = { "time", "year", "people", "thing", "woman", "life", "child", "world", "school", "state", "family", "student", "group", "country", "problem", "hand", "part", "place", "case", "week", "company", "system", "issue", "side", "kind", "head", "house", "service", "friend", "father", "power", "hour", "game", "line", "member", "city", "community", "name", "president", "team" };

	//load notification with title
	cout << "Loaded default wordlist: " << endl 
		<< "\"Top 40 Most Common English Nouns Over 3 Letters Long\"." << endl;

	//choose a random word within that list
	cout << endl << "I'm thinking of a word..." << endl;
	srand(time(0));
	string ChosenWord = Worldlist[rand() % int(WORDLIST_LENGTH)];
	string CorrectGuesses;
	string IncorrectGuesses;
	cout << "Okay, I've got it!" << endl;

	cout << endl << endl;
	system("pause");

	bool bPlayAgain = true;
	
	while (bPlayAgain)
	{
		//start game
		GameState = EGameState::InProgress;
		bool bWonLastGame = false;
		string StatusMessage = "";

		//game loop
		while (GameState == InProgress)
		{
			system("cls"); //windows-only, sorry;

			//update the player on how they're doing
			DrawHangman(IncorrectGuesses.length());
			PrintGuesses(ChosenWord, CorrectGuesses, IncorrectGuesses);

			cout << endl <<  StatusMessage << endl << endl;

			//cout << "    ((DEBUG)) word is: " << ChosenWord << endl;

			cout << "Make a guess: ";
			string PlayerGuess;
			getline(cin, PlayerGuess);

			HandlePlayerGuess(PlayerGuess[0], ChosenWord, &CorrectGuesses, &IncorrectGuesses, &StatusMessage);

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
		cout << endl << endl;

		cout << "Want to try again? ";
		string PlayAgainInput = "";
		getline(cin, PlayAgainInput);
		bPlayAgain = (PlayAgainInput[0] == 'y');		
	}

	system("cls");
	cout << "Thanks for playing!" << endl << endl;
	system("pause");
}
