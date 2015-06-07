/******************************************************************************
File name: tournament.cpp
Description: Assignment 3 for CS 162. Fantasy combat tournament program.
******************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <stack>
#include "creature.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::queue;
using std::stack;

/* inputInt() with two arguments verifies that a user input integer is between
	the two input integers, inclusive. */
int inputInt(int lower, int upper);
/* inputInt() with one argument verifies that a user input integer is greater
	than the argument input integer. */
int inputInt(int lower);
/* inputString() verifies that a user input string is nonempty. */
string inputString();
/* populateLineup() populates a team's lineup with a number of fighters and
	gives each fighter a team name. */
void populateLineup(queue<Creature*>& lineup, int fighters, string team);
/* combat() resolves combat between two Creatures. */
void combat(Creature* unit1, Creature* unit2);
/* tournament() manages the lineups for both teams and the loser stacks for
	both teams. Uses combat() for each pair of fighters. */
void tournament(queue<Creature*>& lineup1, stack<Creature*>& loser1,
	queue<Creature*>& lineup2, stack<Creature*>& loser2);
/* tournamentResults() determines the winners of the tournament. */
void tournamentResults(queue<Creature*>& lineup1, stack<Creature*>& loser1,
	queue<Creature*>& lineup2, stack<Creature*>& loser2);

int main()
{
	srand(time(NULL));
	
	// Creature* unit1 = new Goblin("Buddy", "Team A");
	// Creature* unit2 = new Barbarian("Bro", "Team B");
	// combat(unit1, unit2);

	queue<Creature*> lineup1;
	queue<Creature*> lineup2;
	stack<Creature*> loser1;
	stack<Creature*> loser2;
	cout << "Welcome to the Creature Tournament!" << endl
		<< "Two players field one team each, with some number of fighters."
		<< endl;

	cout << "How many fighters to a team (at least 2)?" << endl;
	int fighters;
	fighters = inputInt(1);

	cout << "Fill the lineup for player 1!" << endl;
	cout << "Give a team name (e.g. Team Awesome):" << endl;
	string teamName1;
	teamName1 = inputString();
	populateLineup(lineup1, fighters, teamName1);
	cout << endl;

	cout << "Fill the lineup for player 2!" << endl;
	cout << "Give a team name (e.g. Team Amazing):" << endl;
	string teamName2;
	teamName2 = inputString();
	while (teamName2 == teamName1)
	{
		cout << "Please enter a different team name!" << endl;
		teamName2 = inputString();
	}
	populateLineup(lineup2, fighters, teamName2);
	cout << endl;

	tournament(lineup1, loser1, lineup2, loser2);

	tournamentResults(lineup1, loser1, lineup2, loser2);

	cout << "Thanks for playing!" << endl;

	return 0;
}

/* populateLineup() takes a queue of Creature pointers, a number of fighters
	to populate the queue with, and a team name to create a lineup of Creatures
	for the tournament.
	Input: queue of Creature pointers, int number of fighters, string team name
	Output: none */
void populateLineup(queue<Creature*>& lineup, int fighters, string team)
{
	for (int i = 0; i < fighters; i++)
	{
		cout << "1. Goblin" << endl
			<< "2. Barbarian" << endl
			<< "3. Reptile person" << endl
			<< "4. Blue man" << endl
			<< "5. The Shadow" << endl;
		cout << "Choose a fighter to add (1-5): ";
		int choice;
		choice = inputInt(1, 5);

		string name;
		cout << "Give your fighter a name: ";
		name = inputString();

		switch (choice)
		{
			case 1:
			{
				Creature* temp = new Goblin(name, team);
				lineup.push(temp);
				break;
			}
			case 2:
			{
				Creature* temp = new Barbarian(name, team);
				lineup.push(temp);
				break;
			}
			case 3:
			{
				Creature* temp = new Reptile(name, team);
				lineup.push(temp);
				break;
			}
			case 4:
			{
				Creature* temp = new BlueMen(name, team);
				lineup.push(temp);
				break;
			}
			case 5:
			{
				Creature* temp = new Shadow(name, team);
				lineup.push(temp);
				break;
			}
			/* If I interpreted Uli's post on the Assignment 4 discussion
				correctly, I can statically allocate pointers to Creature then
				allocate Creature objects to those pointers dynamically (so
				here, this is dynamic because this is based on the user input
				at runtime, and it uses the switch statements). */
		}
	}
	// // testing if Creatures were added correctly
	// for (int i = 0; i < fighters; i++)
	// {
	// 	cout << "Fighter #" << i + 1 << ": " << lineup.front()->showName()
	// 		<< " the " << lineup.front()->showType() << endl;
	// 		lineup.pop();
	// }
}

/* combat() function. A "coin toss" determines who attacks first, then vice
	versa, until one unit's strength value is equal to or less than 0.
	Announces a winner for the round, and then calls on the Creature class
	member function regainStrength() to allow the winner to have a chance to
	regain all lost health or just half of the lost health. Also gives points
	to the winner based on a weighted system.
	Input: first Creature unit, second Creature unit
	Output: none */
void combat(Creature* unit1, Creature* unit2)
{
	cout << unit1->showTeam() << "'s " << unit1->showName() << " the "
		<< unit1->showType() << " versus " << unit2->showTeam() << "'s "
		<< unit2->showName() << " the " << unit2->showType() << "!"
		<< endl;
	int coinToss;
	coinToss = rand() % 2;
	if (coinToss == 0)
	{
		cout << unit1->showTeam() << "'s " << unit1->showName() << " the "
			<< unit1->showType() << " goes first!" << endl << endl;
		while (unit1->isAlive() && unit2->isAlive())
		{
			// unit1 attacks first
			unit1->combatTurn(unit1, unit2);
			// unit2 attacks unit1 if unit2 still has strength > 0
			if (unit2->isAlive())
			{
				unit1->combatTurn(unit2, unit1);
			}
		}
	}
	else
	{
		cout << unit2->showTeam() << "'s " << unit2->showName() << " the "
			<< unit2->showType() << " goes first!" << endl << endl;
		while (unit1->isAlive() && unit2->isAlive())
		{
			// unit2 attacks first
			unit2->combatTurn(unit2, unit1);
			// unit2 attacks unit1 if unit2 still has strength > 0
			if (unit1->isAlive())
			{
				unit2->combatTurn(unit1, unit2);
			}
		}
	}

	if (unit1->isAlive())
	{
		cout << unit2->showTeam() << "'s " << unit2->showName() << " the "
			<< unit2->showType() << " has fallen! "
			<< unit1->showTeam() << "'s " << unit1->showName() << " the "
			<< unit1->showType() << " wins!" << endl;
	}
	else
	{
		cout << unit1->showTeam() << "'s " << unit1->showName() << " the "
			<< unit1->showType() << " has fallen! "
			<< unit2->showTeam() << "'s " << unit2->showName() << " the "
			<< unit2->showType() << " wins!" << endl;
	}

	if (unit1->isAlive())
	{
		unit1->regainStrength();
		unit1->addPoints(unit2);
		cout << unit1->showTeam() << "'s " << unit1->showName() << " the "
			<< unit1->showType() << " has " << unit1->showPoints()
			<< " points." << endl;
	}
	else
	{
		unit2->regainStrength();
		unit2->addPoints(unit1);
		cout << unit2->showTeam() << "'s " << unit2->showName() << " the "
			<< unit2->showType() << " has " << unit2->showPoints()
			<< " points." << endl;
	}
}

/* tournament() encompasses the fighting between the two lineups for the
	tournament. Pits the first Creatures in the queues against each other, and
	puts the winner back in its team's queue and the loser to its team's loser
	stack.
	Input: first team's queue of Creature pointers, first team's stack of loser
		Creature pointers, second team's queue of Creature pointers, second
		team's stack of loser Creature pointers
	Output: none */
void tournament(queue<Creature*>& lineup1, stack<Creature*>& loser1,
	queue<Creature*>& lineup2, stack<Creature*>& loser2)
{
	cout << "Let the tournament begin!" << endl << endl;
	while (lineup1.size() > 0 && lineup2.size() > 0)
	{
		Creature* unit1 = lineup1.front();
		Creature* unit2 = lineup2.front();
		lineup1.pop();
		lineup2.pop();
		/* If I interpreted Uli's post on the Assignment 4 discussion
			correctly, I can statically allocate Creature pointers, which
			is what I'm doing here. The queues were dynamically generated at
			the beginning. */

		combat(unit1, unit2);
		cout << "This round is over!" << endl << endl << endl;

		if (unit1->isAlive())
		{
			lineup1.push(unit1);
			loser2.push(unit2);
		}
		else
		{
			loser1.push(unit1);
			lineup2.push(unit2);
		}
	}
	// // debugging
	// cout << lineup1.size() << " " << lineup2.size() << endl;
	// cout << loser1.size() << " " << loser2.size() << endl;
}

/* tournamentResults() outputs the results of the tournament. It first gets
	the name of the team that still has living fighters. It then copies all
	fighters to a vector and adds the team points together, and then sorts the
	vector in descending order based on points. The function outputs the team
	with the last member(s) standing, then the winning team points, then
	the individual winners.
	Input: first team's queue of Creature pointers, first team's stack of loser
		Creature pointers, second team's queue of Creature pointers, second
		team's stack of loser Creature pointers
	Output: none */
void tournamentResults(queue<Creature*>& lineup1, stack<Creature*>& loser1,
	queue<Creature*>& lineup2, stack<Creature*>& loser2)
{
	vector<Creature*> standings;
	// store team with members still in queue
	// last team standing
	string lastTeam;
	if (!lineup1.empty())
	{
		lastTeam = lineup1.front()->showTeam();
	}
	else
	{
		lastTeam = lineup2.front()->showTeam();
	}

	// get team names before putting fighters into the vector
	string team1;
	if (!lineup1.empty())
	{
		team1 = lineup1.front()->showTeam();
	}
	else
	{
		team1 = loser1.top()->showTeam();
	}

	string team2;
	if (!lineup2.empty())
	{
		team2 = lineup2.front()->showTeam();
	}
	else
	{
		team2 = loser2.top()->showTeam();
	}

	// get team points while putting fighters into vector
	int teamPts1 = 0;
	// copy all Creatures to a vector
	while (!lineup1.empty())
	{
		teamPts1 += lineup1.front()->showPoints();
		standings.push_back(lineup1.front());
		lineup1.pop();
	}
	while (!loser1.empty())
	{
		teamPts1 += loser1.top()->showPoints();
		standings.push_back(loser1.top());
		loser1.pop();
	}

	int teamPts2 = 0;
	while (!lineup2.empty())
	{
		teamPts2 += lineup2.front()->showPoints();
		standings.push_back(lineup2.front());
		lineup2.pop();
	}
	while (!loser2.empty())
	{
		teamPts2 += loser2.top()->showPoints();
		standings.push_back(loser2.top());
		loser2.pop();
	}

	// sort in descending order by points the fighters
	// bubble sort
	Creature* temp;
	bool swap;
	do
	{
		swap = false;
		for (unsigned int i = 0; i < (standings.size() - 1); i++)
		{
			if (standings.at(i)->showPoints() <
				standings.at(i + 1)->showPoints())
			{
				temp = standings.at(i);
				standings.at(i) = standings.at(i + 1);
				standings.at(i + 1) = temp;
				swap = true;
			}
		}
	}
	while (swap);

	// // debugging
	// for (unsigned int i = 0; i < standings.size(); i++)
	// {
	// 	cout << standings.at(i)->showPoints() << endl;
	// }
	// cout << teamPts1 << " " << teamPts2 << endl;

	cout << "Team results: " << endl;
	cout << "The team with the last member(s) standing: " << lastTeam
		<< "!" << endl;

	if (teamPts1 > teamPts2)
	{
		cout << team1 << " wins with a total of " << teamPts1 << " points!"
			<< endl;
		cout << team2 << " had " << teamPts2 << " points." << endl << endl;
	}
	else if (teamPts2 > teamPts1)
	{
		cout << team2 << " wins with a total of " << teamPts2 << " points!"
			<< endl;
		cout << team1 << " had " << teamPts1 << " points." << endl << endl;
	}
	else
	{
		cout << team1 << " and " << team2 << " tied with " << teamPts1
			<< " points each!" << endl << endl;
	}

	cout << "Individual results: " << endl;
	// if less than 3 total fighters
	// program does not allow for 0 fighters, technically shouldn't allow for
	// just 1, either, but just in case...
	if (standings.size() < 3)
	{
		cout << "Something's wrong. You should have at least 4 total fighers."
			<< endl;
		exit(1);
	}
	else
	{
		// ignoring ties
		cout << "First place: "
				<< standings.at(0)->showTeam() << "'s "
				<< standings.at(0)->showName() << " the "
				<< standings.at(0)->showType() << ", with "
				<< standings.at(0)->showPoints() << " points!" << endl;
		cout << "Second place: "
				<< standings.at(1)->showTeam() << "'s "
				<< standings.at(1)->showName() << " the "
				<< standings.at(1)->showType() << ", with "
				<< standings.at(1)->showPoints() << " points!" << endl;
		cout << "Third place: "
				<< standings.at(2)->showTeam() << "'s "
				<< standings.at(2)->showName() << " the "
				<< standings.at(2)->showType() << ", with "
				<< standings.at(2)->showPoints() << " points!" << endl;
	}

	cout << endl << endl;

	// deleting objects from memory, not just the pointers to the objects
	// the pop operations earlier just deleted pointers
	for (unsigned int i = 0; i < standings.size(); i++)
	{
		Creature* temp2 = standings.at(i);
		delete temp2;
		// if the destructor in creature.cpp and creature.h is commented out
		// you can see the confirmation messages
	}

	standings.clear();
	// removes all elements from the vector and destroys them
	// (according to STL documentation)
	// the vector is actually a vector of pointers, not objects
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

/* inputInt() verifies user integer input and ensures that it is greater than
	the argument input integer.
	This version of the function takes one argument.
	Input: integer lower bound
	Output: returns user input number */
int inputInt(int lower)
{
	int tempNum;
	cin >> tempNum;
	while (cin.fail() || (tempNum <= lower))
	{
		cin.clear();
		cin.ignore();
		cout << "Please enter an integer greater than " << lower << ": ";
		cin >> tempNum;
	}
	cin.ignore(1000, '\n');
	return tempNum;
}

/* inputString() verifies that a string input is nonempty. This function takes
	no arguments.
	Input: none
	Output: returns user input string as a string */
string inputString()
{
	string tempString("");
	getline(cin, tempString);
	while (tempString.length() == 0 || cin.fail())
	{
		cout << "Name is empty; try again: ";
		getline(cin, tempString);
	}
	return tempString;
}
