/******************************************************************************
File name: main.cpp
Description: Assignment 3 for CS 162. Fantasy combat program.
******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "creature.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

int inputInt(int lower, int upper);
void menu();
void combat(Creature &unit1, Creature &unit2);

int main()
{
	srand(time(NULL));
	
	menu();

	return 0;
}

void menu()
{
	int choice1, choice2;
	cout << "1. Goblin" << endl
		<< "2. Barbarian" << endl
		<< "3. Reptile people" << endl
		<< "4. Blue men" << endl
		<< "5. The Shadow" << endl;
	cout << "Choose a creature type as the first combatant: ";
	choice1 = inputInt(1, 5);
	cout << "Choose a second creature type: ";
	choice2 = inputInt(1, 5);
	Creature *creature1, *creature2;
	switch (choice1)
	{
		case 1:
		{
			creature1 = new Goblin();
			break;
		}
		case 2:
		{
			creature1 = new Barbarian();
			break;
		}
		case 3:
		{
			creature1 = new Reptile();
			break;
		}
		case 4:
		{
			creature1 = new BlueMen();
			break;
		}
		case 5:
		{
			creature1 = new Shadow();
			break;
		}
	}
	switch (choice2)
	{
		case 1:
		{
			creature2 = new Goblin();
			break;
		}
		case 2:
		{
			creature2 = new Barbarian();
			break;
		}
		case 3:
		{
			creature2 = new Reptile();
			break;
		}
		case 4:
		{
			creature2 = new BlueMen();
			break;
		}
		case 5:
		{
			creature2 = new Shadow();
			break;
		}
	}
	
	//distinguish between same creatures
	if (choice1 == choice2)
	{
		creature1->setName("first ");
		creature2->setName("second ");
	}
	
	cout << endl;
	combat(*creature1, *creature2);
	
	delete creature1;
	delete creature2;
	
	cout << "Would you like to play again? (y/n)" << endl;
	string playAgain;
	getline(cin, playAgain);
	if (playAgain == "n" || playAgain == "N")
	{
		cout << "Goodbye!" << endl;
		exit(0);
	}
	else
	{
		menu();
	}
}
	
/* combat() function. The first unit attacks the second unit, then vice versa,
	until one unit's strength value is equal to or less than 0.
	Input: first Creature unit, second Creature unit
	Output: none */
void combat(Creature &unit1, Creature &unit2)
{
	while (unit1.isAlive() && unit2.isAlive())
	{
		//unit1 attacks first
		unit1.combatTurn(unit1, unit2);
		//unit2 attacks unit1 if unit2 still has strength > 0
		if (unit2.isAlive())
		{
			unit1.combatTurn(unit2, unit1);
		}
	}
	if (unit1.isAlive())
	{
		cout << "The " << unit2.showType() << " has fallen! The "
			<< unit1.showType() << " wins!" << endl;
	}
	else
	{
		cout << "The " << unit1.showType() << " has fallen! The "
			<< unit2.showType() << " wins!" << endl;
	}
}

/* inputInt() verifies integer input and ensures that it falls within a range.
	This version of the function has two integer arguments.
	Input: lower and upper bounds as int
	Output: returns user input number */
int inputInt(int lower, int upper)
{
	int tempNum;
	cin >> tempNum;
	while (cin.fail() || (tempNum < lower || tempNum > upper))
	{
		cin.clear();
		cin.ignore();
		cout << "Please enter an integer from " << lower << " to "
			<< upper << ": ";
		cin >> tempNum;
	}
	cin.ignore(1000, '\n');
	return tempNum;
}
