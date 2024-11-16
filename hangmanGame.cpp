// hangmanGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

// function to print a formatted message box around a message
void PrintMessage(string message, bool printTop = true, bool printBottom = true)
{
	if (printTop)
	{
		cout << "+---------------------------------+" << endl;
		cout << "|";
	}
	else
	{
		cout << "|";
	}
	bool front = true;
	for (int i = message.length(); i < 33; i++)
	{
		// adds spaces to center-align the message in the box
		if (front)
		{
			message = " " + message;
		}
		else
		{
			message = message + " ";
		}
		front = !front;
	}
	cout << message.c_str();

	if (printBottom)
	{
		cout << "|" << endl;
		cout << "+---------------------------------+" << endl;
	}
	else
	{
		cout << "|" << endl;
	}
}

// function to draw the hangman figure based on number of wrong guesses
void DrawHangman(int guessCount = 0)
{
	// adds hangman parts one by one as guessCount increases
	if (guessCount >= 1)
		PrintMessage("|", false, false);
	else
		PrintMessage("", false, false);

	if (guessCount >= 2)
		PrintMessage("|", false, false);
	else
		PrintMessage("", false, false);

	if (guessCount >= 3)
		PrintMessage("O", false, false); // head
	else
		PrintMessage("", false, false);

	if (guessCount == 4)
		PrintMessage("/  ", false, false); // left arm

	if (guessCount == 5)
		PrintMessage("/| ", false, false); // upper body and one arm

	if (guessCount >= 6)
		PrintMessage("/|\\", false, false); // full upper body
	else
		PrintMessage("", false, false);

	if (guessCount >= 7)
		PrintMessage("|", false, false); // torso
	else
		PrintMessage("", false, false);

	if (guessCount == 8)
		PrintMessage("/", false, false); // left leg

	if (guessCount >= 9)
		PrintMessage("/ \\", false, false); // full legs
	else
		PrintMessage("", false, false);
}

// function to print available letters in two rows (split into 'A'-'M' and 'N'-'Z')
void PrintLetters(string input, char from, char to)
{
	string s;
	for (char i = from; i <= to; i++)
	{
		if (input.find(i) == string::npos)
		{
			s += i; // if letter hasn’t been guessed, add it to available list
			s += " ";
		}
		else
			s += "  "; // if letter was guessed, leave blank
	}
	PrintMessage(s, false, false);
}


// function to print letters still available for guessing
void PrintAvailableLetters(string taken)
{
	PrintMessage("Available letters");
	PrintLetters(taken, 'A', 'M');
	PrintLetters(taken, 'N', 'Z');
}

// function to print the current word with guessed letters filled in, underscores for unguessed letters
// also returns true if the player has won
bool PrintWordAndCheckWin(string word, string guessed)
{
	bool won = true;
	string s;
	for (int i = 0; i < word.length(); i++)
	{
		if (guessed.find(word[i]) == string::npos)
		{
			won = false;
			s += "_ "; // unguessed letters are shown as underscores
		}
		else
		{
			s += word[i]; // guessed letters are shown
			s += " ";
		}
	}
	PrintMessage(s, false);
	return won;
}

// function to load a random word from a file
string LoadRandomWord(string path)
{
	int lineCount = 0;
	string word;
	vector<string> v;
	ifstream reader(path);
	if (reader.is_open())
	{
		// read each line and store words in vector
		while (std::getline(reader, word))
			v.push_back(word);

		// choose a random word
		int randomLine = rand() % v.size();

		word = v.at(randomLine);
		reader.close();
	}
	return word;
}

// function to count incorrect guesses
int TriesLeft(string word, string guessed)
{
	int error = 0;
	for (int i = 0; i < guessed.length(); i++)
	{
		// if guessed letter isn't in the word, increment error
		if (word.find(guessed[i]) == string::npos)
			error++;
	}
	return error;
}

int main()
{
	srand(time(0)); // initialize random seed
	string guesses;
	string wordToGuess;
	wordToGuess = LoadRandomWord("words.txt"); // get random word from file
	int tries = 0;
	bool win = false;
	do
	{
		system("cls"); // clear screen for a fresh display each turn
		PrintMessage("HANGMAN");
		DrawHangman(tries);
		PrintAvailableLetters(guesses);
		PrintMessage("Guess the word");
		win = PrintWordAndCheckWin(wordToGuess, guesses);

		if (win)
			break;

		char x;
		cout << ">"; cin >> x; // prompt player to guess a letter

		if (guesses.find(x) == string::npos)
			guesses += x;

		tries = TriesLeft(wordToGuess, guesses);

	} while (tries < 10);

	if (win)
		PrintMessage("YOU WON!");
	else
		PrintMessage("GAME OVER");

	system("pause"); // pause at the end so the player can see the result
	getchar();
	return 0;
}

/*
+---------------------------------+
|             HANGMAN             |
+---------------------------------+
|               |                 |
|               |                 |
|               O                 |
|              /|\                |
|               |                 |
|              / \                |
|         +----------+            |
|         |          |            |
+---------------------------------+
|        Available letters        |
+---------------------------------+
|     A B C D E F G H I J K L M   |
|     N O P Q R S T U V W X Y Z   |
+---------------------------------+
|         Guess the word          |
+---------------------------------+
| _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ |
+---------------------------------+
>
*/