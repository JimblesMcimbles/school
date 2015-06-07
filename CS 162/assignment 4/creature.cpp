/******************************************************************************
File name: creature.cpp
Description: Implementation file for the Creature class and its subclasses.
******************************************************************************/

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "creature.h"
using std::cout;
using std::endl;
using std::string;

/********************** Creature **********************/

/* Default constructor for the Creature abstract base class. Intentionally
	left blank. */
Creature::Creature()
{
	// intentionally left blank
}

/* Destructor for the Creature class. Prints to the terminal a message
	indicating that the object is being destructed. */
// Creature::~Creature()
// {
// 	cout << "DESTRUCTING" << endl;
// }

/* attack() function for the Creature class. Is redefined for the Goblin
	derived class. Rolls an attack for the Creature based on the number
	of attack dice and number of sides per attack die. */
int Creature::attack(Creature* target)
{
	int attackSum = 0;
	for (int i = 0; i < attackDice; i++)
	{
		attackSum += rand() % attackSides + 1;
	}
	return attackSum;
}

/* defense() function for the Creature class. Is redefined for the Shadow
	derived class. Rolls a defense for the Creature based on the number of
	defense dice and number of sides per defense die. */
int Creature::defense()
{
	int defenseSum = 0;
	for (int i = 0; i < defenseDice; i++)
	{
		defenseSum += rand() % defenseSides + 1;
	}
	return defenseSum;
}

/* combatTurn() function for the Creature class. Calculates one turn of combat
	with one attacker and one defender. Combines attack and defense rolls.
	Takes armor negation into account. Has special cases for Goblin attack and
	Shadow defense. Modified to not allow for negative strength (min strength
	is 0). */
void Creature::combatTurn(Creature* attacker, Creature* defender)
{
	// show defense strength at beginning of round
	cout << defender->showTeam() << "'s " << defender->showName() << " the "
		<< defender->showType() << " has " << defender->showStrength()
		<< " strength." << endl;
	// displays who attacks whom
	cout << attacker->showTeam() << "'s " << attacker->showName() << " the "
		<< attacker->showType() << " attacks "
		<< defender->showTeam() << "'s " << defender->showName() << " the "
		<< defender->showType() << "!" << endl;

	// rolls attack and defense
	int attackRoll = attacker->attack(defender);
	int defenseRoll = defender->defense();

	cout << attacker->showTeam() << "'s " << attacker->showName() << " the "
		<< attacker->showType() << " rolls an attack of "
		<< attackRoll << ", and" << endl << defender->showTeam() << "'s "
		<< defender->showName() << " the "
		<< defender->showType();

	// if the Shadow dodges
	if (defenseRoll == 0 && defender->showType() == "Shadow")
	{
		cout << " successfully dodges the attack!" << endl;
	}
	// Achilles strike nullifies the defense!
	// only for Goblin attackers and non-Goblin defenders
	else if (attackRoll == 12 && attacker->showType() == "Goblin"
		&& defender->showType() != "Goblin")
	{
		cout << "'s defense is useless against the Achilles strike!"
			<< endl;
	}
	else
	{
		cout << " rolls a defense of " << defenseRoll << " and has "
			<< defender->armor << " armor." << endl;
	}
	
	// need to make sure attack > defense + armor
	// or else attack deals no damage
	if (defenseRoll == 0 && defender->showType() == "Shadow")
	{
		cout << "No damage is dealt to " << defender->showTeam()
			<< "'s " << defender->showName() << " the Shadow!" << endl;
	}
	// Achilles strike nullifies defense roll!
	// Goblin attacker rolls 12 and non-Goblin defender
	else if (attackRoll == 12 && attacker->showType() == "Goblin"
		&& defender->showType() != "Goblin")
	{
		// first case doesn't happen, as all armor is less than 12
		if (defender->armor >= attackRoll)
		{
			cout << "The " << defender->showTeam() << "'s "
				<< defender->showName() << " the "
				<< defender->showType()
				<< "'s defense is breached by the Achilles "
				<< "attack, but the armor nullifies the "
				<< "attack!" << endl;
		}
		// calculuate damage based on just attack and armor
		else
		{
			cout << attacker->showTeam() << "'s " << attacker->showName()
				<< " the " << attacker->showType() << " deals "
				<< attackRoll - defender->armor << " damage to "
				<< defender->showTeam() << "'s " << defender->showName()
				<< " the " << defender->showType() << "."
				<< endl;
			defender->strength -= (attackRoll - defender->armor);
		}
	}
	// normal attack where armor + defense >= attack
	else if ((defenseRoll + defender->armor) >= attackRoll)
	{
		cout << defender->showTeam() << "'s " << defender->showName()
			<< " the " << defender->showType()
			<< "'s defense and armor " << "nullify the attack!"
			<< endl;
	}
	// normal attack where attack > armor + defense
	else
	{
		cout << attacker->showTeam() << "'s " << attacker->showName()
			<< " the " << attacker->showType() << " deals "
			<< attackRoll - defenseRoll - defender->armor
			<< " damage to " << defender->showTeam() << "'s "
			<< defender->showName() << " the " << defender->showType() << "."
			<< endl;
		defender->strength -= (attackRoll - defenseRoll
			- defender->armor);
	}
	
	// prevents negative strength
	if (defender->showStrength() < 0)
	{
		defender->strength = 0;
	}

	cout << defender->showTeam() << "'s " << defender->showName()
		<< " the " << defender->showType() << "'s strength is now "
		<< defender->showStrength() << "." << endl;
	cout << endl;
}

/* regainStrength() function for the Creature class. Calculates some amount of
	strength to regain for a fighter when a bout is over. 25% chance to regain
	all of the missing strength. */
void Creature::regainStrength()
{
	int missingHP;
	missingHP = this->maxStrength - this->strength;
	cout << this->team << "'s " << this->name << " the " << this->type
		<< " is missing " << missingHP << " strength!" << endl;
	// if no health is missing, function ends
	if (missingHP == 0)
	{
		cout << this->team << "'s " << this->name << " the " << this->type
			<< " is at full strength, at " << this->strength << "." << endl;
		return;
	}
	// 1 in 4 chance to regain all health; otherwise, regain half of missing
	// health
	int healthChance;
	healthChance = rand() % 4;
	if (healthChance == 0)
	{
		cout << this->team << "'s " << this->name << " the " << this->type
			<< " luckily regains all of its missing strength!"
			<< endl;
		strength += missingHP;
		cout << this->team << "'s " << this->name << " the " << this->type
			<< " now has " << this->strength << " strength." << endl;
	}
	else
	{
		cout << this->team << "'s " << this->name << " the " << this->type
			<< " regains half of its missing strength." << endl;
		strength += missingHP / 2;
		cout << this->team << "'s " << this->name << " the " << this->type
			<< " now has " << this->strength << " strength." << endl;
	}
}

/* Accessor function for attackDice. */
int Creature::showAttackDice()
{
	return this->attackDice;
}

/* Accessor function for attackSides. */
int Creature::showAttackSides()
{
	return this->attackSides;
}

/* Accessor function for defenseDice. */
int Creature::showDefenseDice()
{
	return this->defenseDice;
}

/* Accessor function for defenseSides. */
int Creature::showDefenseSides()
{
	return this->defenseSides;
}

/* Accessor function for armor. */
int Creature::showArmor()
{
	return this->armor;
}

/* Accessor function for strength. */
int Creature::showMaxStrength()
{
	return this->maxStrength;
}

/* Accessor function for current strength. */
int Creature::showStrength()
{
	return this->strength;
}

/* Accessor function for current points. */
int Creature::showPoints()
{
	return this->points;
}

/* Accessor function for type. */
string Creature::showType()
{
	return this->type;
}

/* Accessor function for name. */
string Creature::showName()
{
	return this->name;
}

/* Accessor function for team. */
string Creature::showTeam()
{
	return this->team;
}

/* Accessor function for isGoblin. */
bool Creature::showIsGoblin()
{
	return isGoblin;
}

/* isAlive() function for the Creature class. Returns a boolean based on if the
	object's strength is greater than 0 or not. */
bool Creature::isAlive()
{
	if (strength > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Mutator function for attackSides. */
void Creature::setAttackSides(int value)
{
	attackSides = value;
}

/* Mutator function for type. Only used when two of the same creature are
	fighting each other. */
void Creature::setName(string inputName)
{
	this->name = inputName;
}

/********************** Goblin **********************/

/* Default constructor for the Goblin derived class. */
Goblin::Goblin() : Creature()
{
	attackDice = 2;
	attackSides = 6;
	defenseDice = 1;
	defenseSides = 6;
	armor = 3;
	maxStrength = 8;
	strength = 8;
	points = 0;
	isGoblin = true;
	type = "Goblin";
	name = "";
}

/* Constructor for the Goblin derived class that takes a name as a string and
	a team name as a string. */
Goblin::Goblin(string inputName, string teamName) : Creature ()
{
	attackDice = 2;
	attackSides = 6;
	defenseDice = 1;
	defenseSides = 6;
	armor = 3;
	maxStrength = 8;
	strength = 8;
	points = 0;
	isGoblin = true;
	type = "Goblin";
	name = inputName;
	team = teamName;
}

/* attack() function for the Goblin derived class. This one is a redefined
	version of the attack() function from the Creature class. For a goblin,
	rolling a 12 causes the Goblin's opponent's attack roll halved. I took
	this to mean that an opponent with a 2d6 (so 2-12) becomes 2d3 (2-6)
	instead of a 1d6 (1-6). This is because one of the other creatures has
	a 3d6 attack, which you can't really divide by half evenly. */
int Goblin::attack(Creature* target)
{
	int attackSum = 0;
	for (int i = 0; i < attackDice; i++)
	{
		attackSum += rand() % attackSides + 1;
	}
	if (attackSum == 12 && target->showIsGoblin() == false)
	{
		target->setAttackSides(target->showAttackSides() / 2);
		cout << "The goblin hit the Achilles of the "
			<< target->showType() << "!" << endl;
		cout << "The attack power of the " << target->showType()
			<< " is now " << target->showAttackDice() << "d"
			<< target->showAttackSides() << "!" << endl;
	}
	return attackSum;
}

/* addPoints() function for the Goblin derived class. Calculates points to add
	if the Creature wins against another Creature. Weighted by how powerful a
	creature is. A weaker creature defeating a powerful creature gains more
	points. */
void Goblin::addPoints(Creature* target)
{
	if (target->showType() == "Goblin")
	{
		points += 3;
	}
	else if (target->showType() == "Barbarian")
	{
		points += 4;
	}
	else if (target->showType() == "Reptile Person")
	{
		points += 7;
	}
	else if (target->showType() == "Blue Man")
	{
		points += 10;
	}
	else if (target->showType() == "Shadow")
	{
		points += 8;
	}
}

/********************** Barbarian **********************/

/* Default constructor for the Barbarian derived class. */
Barbarian::Barbarian() : Creature()
{
	attackDice = 2;
	attackSides = 6;
	defenseDice = 2;
	defenseSides = 6;
	armor = 0;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Barbarian";
}

/* Constructor for the Barbarian derived class that takes a name as a string
	and a team name as a string. */
Barbarian::Barbarian(string inputName, string teamName) : Creature()
{
	attackDice = 2;
	attackSides = 6;
	defenseDice = 2;
	defenseSides = 6;
	armor = 0;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Barbarian";
	name = inputName;
	team = teamName;
}

/* addPoints() function for the Barbarian derived class. Calculates points to
	add	if the Creature wins against another Creature. Weighted by how powerful
	a creature is. A weaker creature defeating a powerful creature gains more
	points. */
void Barbarian::addPoints(Creature* target)
{
	if (target->showType() == "Goblin")
	{
		points += 2;
	}
	else if (target->showType() == "Barbarian")
	{
		points += 3;
	}
	else if (target->showType() == "Reptile Person")
	{
		points += 6;
	}
	else if (target->showType() == "Blue Man")
	{
		points += 9;
	}
	else if (target->showType() == "Shadow")
	{
		points += 7;
	}
}

/********************** Reptile **********************/

/* Default constructor for the Reptile derived class. */
Reptile::Reptile() : Creature()
{
	attackDice = 3;
	attackSides = 6;
	defenseDice = 1;
	defenseSides = 6;
	armor = 7;
	maxStrength = 18;
	strength = 18;
	points = 0;
	isGoblin = false;
	type = "Reptile Person";
}

/* Constructor for the Reptile derived class that takes a name as a string and
	a team name as a string. */
Reptile::Reptile(string inputName, string teamName) : Creature()
{
	attackDice = 3;
	attackSides = 6;
	defenseDice = 1;
	defenseSides = 6;
	armor = 7;
	maxStrength = 18;
	strength = 18;
	points = 0;
	isGoblin = false;
	type = "Reptile Person";
	name = inputName;
	team = teamName;
}

/* addPoints() function for the Reptile derived class. Calculates points to add
	if the Creature wins against another Creature. Weighted by how powerful a
	creature is. A weaker creature defeating a powerful creature gains more
	points. */
void Reptile::addPoints(Creature* target)
{
	if (target->showType() == "Goblin")
	{
		points += 1;
	}
	else if (target->showType() == "Barbarian")
	{
		points += 2;
	}
	else if (target->showType() == "Reptile Person")
	{
		points += 3;
	}
	else if (target->showType() == "Blue Man")
	{
		points += 5;
	}
	else if (target->showType() == "Shadow")
	{
		points += 3;
	}
}

/********************** BlueMen **********************/

/* Default constructor for the BlueMen derived class. */
BlueMen::BlueMen() : Creature()
{
	attackDice = 2;
	attackSides = 10;
	defenseDice = 3;
	defenseSides = 6;
	armor = 3;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Blue Man";
}

/* Constructor for the BlueMen derived class that takes a name as a string and
	a team name as a string. */
BlueMen::BlueMen(string inputName, string teamName) : Creature()
{
	attackDice = 2;
	attackSides = 10;
	defenseDice = 3;
	defenseSides = 6;
	armor = 3;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Blue Man";
	name = inputName;
	team = teamName;
}

/* addPoints() function for the BlueMen derived class. Calculates points to add
	if the Creature wins against another Creature. Weighted by how powerful a
	creature is. A weaker creature defeating a powerful creature gains more
	points. */
void BlueMen::addPoints(Creature* target)
{
	if (target->showType() == "Goblin")
	{
		points += 1;
	}
	else if (target->showType() == "Barbarian")
	{
		points += 1;
	}
	else if (target->showType() == "Reptile Person")
	{
		points += 2;
	}
	else if (target->showType() == "Blue Man")
	{
		points += 3;
	}
	else if (target->showType() == "Shadow")
	{
		points += 2;
	}
}

/********************** Shadow **********************/

/* Default constructor for the Shadow derived class. */
Shadow::Shadow() : Creature()
{
	attackDice = 2;
	attackSides = 10;
	defenseDice = 1;
	defenseSides = 6;
	armor = 0;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Shadow";
}

/* Constructor for the Shadow derived class that takes a name as a string and
	a team name as a string. */
Shadow::Shadow(string inputName, string teamName) : Creature()
{
	attackDice = 2;
	attackSides = 10;
	defenseDice = 1;
	defenseSides = 6;
	armor = 0;
	maxStrength = 12;
	strength = 12;
	points = 0;
	isGoblin = false;
	type = "Shadow";
	name = inputName;
	team = teamName;
}

/* defense() function for the Shadow derived class. The Shadow object has a 50%
	chance to evade all attacks. If an attack is evaded, the function
	returns 0. The downstream combatTurn function is able to distinguish
	the dodge from a regular defensive roll. */
int Shadow::defense()
{
	int dodge = rand() % 2;
	if (dodge == 1)
	{
		return 0;
	}
	else
	{
		int defenseSum = 0;
		for (int i = 0; i < defenseDice; i++)
		{
			defenseSum += rand() % defenseSides + 1;
		}
		return defenseSum;
	}
}

/* addPoints() function for the Shadow derived class. Calculates points to add
	if the Creature wins against another Creature. Weighted by how powerful a
	creature is. A weaker creature defeating a powerful creature gains more
	points. */
void Shadow::addPoints(Creature* target)
{
	if (target->showType() == "Goblin")
	{
		points += 1;
	}
	else if (target->showType() == "Barbarian")
	{
		points += 2;
	}
	else if (target->showType() == "Reptile Person")
	{
		points += 3;
	}
	else if (target->showType() == "Blue Man")
	{
		points += 6;
	}
	else if (target->showType() == "Shadow")
	{
		points += 3;
	}
}
