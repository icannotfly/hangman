#include <iostream>
#include <fstream>
#include <string>
#include <ctime> //for seeding srand
#include <vector>

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
		*StatusMessage = "You've already guessed ";
		*StatusMessage += PlayerGuess;
		*StatusMessage += "; try again.";
		return;
	}
	else if (ChosenWord.find(PlayerGuess) != string::npos)
	{
		//letter is in word
		*StatusMessage = "Correct! There is a \"";
		*CorrectGuesses += PlayerGuess;
	}
	else
	{
		//letter is not in word
		*StatusMessage = "Nope! There is no \"";
		*IncorrectGuesses += PlayerGuess;
	}

	*StatusMessage += PlayerGuess;
	*StatusMessage += "\" in the word.";
}

int main()
{
	cout << endl
		<< "    ==========" << endl
		<< "     Hangman!" << endl
		<< "    ==========" << endl << endl;

	EGameState GameState = EGameState::WaitingToStart;

	string WordlistName = "";
	vector<string> WordlistDefault;

	//load default wordlist
	ifstream infile("default.wordlist");
	if (!infile)
	{
		//try one directory up, just in case we're running from the debug directory
		infile = ifstream("..\default.wordlist");
		if (!infile) {
			system("cls");
			cerr << "Could not load default wordlist!" << endl << "Ensure that default.wordlist is in the same directory as this program and try again." << endl << endl << endl;
			system("pause");

			system("cls");
			cout << "Using a temporary wordlist instead." << endl;
			WordlistName = "Backup Wordlist";
			WordlistDefault = { "time", "year", "people", "thing", "woman", "life", "child", "world", "school", "state", "family", "student", "group", "country", "problem", "hand", "part", "place", "case", "week", "company", "system", "issue", "side", "kind", "head", "house", "service", "friend", "father", "power", "hour", "game", "line", "member", "city", "community", "name", "president", "team" };
		}
	}

	//parse wordlist
	while (infile)
	{
		string instring = "";
		getline(infile, instring);
		
		//skip comments and newlines
		if (instring[0] != '#' && !instring.empty())
		{
			if (WordlistName == "")
			{
				WordlistName = instring;
			}
			else
			{
				WordlistDefault.push_back(instring);
			}
		}
	}
	infile.close();

	//load notification with title
	cout << "Loaded wordlist \"" << WordlistName << "\", containing " << WordlistDefault.size() << " words." << endl;

	bool bPlayAgain = true;
	while (bPlayAgain)
	{
		//start game
		GameState = EGameState::InProgress;
		bool bWonLastGame = false;
		string StatusMessage = "";

		//choose a random word within the list
		cout << endl << "I'm thinking of a word..." << endl;
		srand(time(0));
		string ChosenWord = WordlistDefault[rand() % int(WORDLIST_LENGTH)];
		string CorrectGuesses;
		string IncorrectGuesses;
		cout << "Okay, I've got it!" << endl;

		cout << endl << endl;
		system("pause");

		//game loop
		while (GameState == InProgress)
		{
			system("cls"); //windows-only, sorry;

			//update the player on how they're doing
			DrawHangman(IncorrectGuesses.length());
			PrintGuesses(ChosenWord, CorrectGuesses, IncorrectGuesses);
			cout << endl <<  StatusMessage << endl << endl;

			cout << "Make a guess: ";
			string sPlayerGuess;
			getline(cin, sPlayerGuess);
			char PlayerGuess = tolower(sPlayerGuess[0]);
			HandlePlayerGuess(PlayerGuess, ChosenWord, &CorrectGuesses, &IncorrectGuesses, &StatusMessage);

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
		cout << "The word was \"" << ChosenWord << "\"." << endl << endl << endl;

		bool bTryAgainloop = true;
		while (bTryAgainloop)
		{
			cout << "Want to try again? ";
			string PlayAgainInput = "";
			getline(cin, PlayAgainInput);
			if (tolower(PlayAgainInput[0]) == 'y')
			{
				bPlayAgain = true;
				bTryAgainloop = false;
			}
			else if (tolower(PlayAgainInput[0]) == 'n')
			{
				bPlayAgain = false;
				bTryAgainloop = false;
			}
			else
			{
				bTryAgainloop = true;
				system("cls");
			}
		}
	}

	system("cls");
	cout << "Thanks for playing!" << endl << endl;
	system("pause");
}
