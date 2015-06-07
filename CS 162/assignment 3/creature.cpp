/******************************************************************************
File name: creature.cpp
Description: Implementation file for the Creature class and its subclasses.
******************************************************************************/

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "creature.h"
#include <cstdio>
using std::string;
using std::cout;
using std::endl;

/********************** Creature **********************/

/* Default constructor for the Creature abstract base class. Intentionally
	left blank. */
Creature::Creature()
{
	//intentionally left blank
}

/* attack() function for the Creature class. Is redefined for the Goblin
	derived class. */
int Creature::attack(Creature &target)
{
	int attackSum = 0;
	for (int i = 0; i < attackDice; i++)
	{
		attackSum += rand() % attackSides + 1;
	}
	return attackSum;
}

/* defense() function for the Creature class. Is redefined for the Shadow
	derived class. */
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
	with one attacker and one defender. */
void Creature::combatTurn(Creature &attacker, Creature &defender)
{
	cout << "The " << attacker.showType() << " attacks the "
		<< defender.showType() << "!" << endl;
	
	int attackRoll = attacker.attack(defender);
	int defenseRoll = defender.defense();
	
	cout << "The " << attacker.showType() << " rolls an attack of "
		<< attackRoll << ", and the " << defender.showType();
	//if the Shadow dodges
	if (defenseRoll == 0 && defender.showType() == "Shadow")
	{
		cout << " successfully dodges the attack!" << endl;
	}
	//Achilles strike nullifies the defense!
	if (attackRoll == 12 && attacker.showType() == "Goblin")
	{
		cout << "'s defense is useless against the Achilles strike!"
			<< endl;
	}
	else
	{
		cout << " rolls a defense of " << defenseRoll << " and has "
			<< defender.armor << " armor." << endl;
	}
	
	//need to make sure attack > defense + armor
	//or else attack deals no damage
	if (defenseRoll == 0 && defender.showType() == "Shadow")
	{
		cout << "No damage is dealt to The Shadow!" << endl;
	}
	//Achilles strike
	else if (attackRoll == 12 && attacker.showType() == "Goblin")
	{
		//first case doesn't happen, as all armor is less than 12
		if (defender.armor >= attackRoll)
		{
			cout << "The " << defender.showType()
				<< "'s defense is breached by the Achilles "
				<< "attack, but the armor nullifies the "
				<< "attack!" << endl;
		}
		//calculuate damage based on just attack and armor
		else
		{
			cout << "The " << attacker.showType() << " deals "
				<< attackRoll - defender.armor << " damage to "
				<< "the " << defender.showType() << "."
				<< endl;
			defender.strength -= (attackRoll - defender.armor);
		}
	}
	//normal attack where armor + defense >= attack
	else if ((defenseRoll + defender.armor) >= attackRoll)
	{
		cout << "The " << defender.showType()
			<< "'s defense and armor " << "nullify the attack!"
			<< endl;
	}
	//normal attack where attack > armor + defense
	else
	{
		cout << "The " << attacker.showType() << " deals "
			<< attackRoll - defenseRoll - defender.armor
			<< " damage to the " << defender.showType() << "."
			<< endl;
		defender.strength -= (attackRoll - defenseRoll
			- defender.armor);
	}
	
	cout << "The " << defender.showType() << "'s strength is "
		<< defender.showStrength() << "." << endl;
	cout << endl;
}

/* Accessor function for attackDice. */
int Creature::showAttackDice()
{
	return attackDice;
}

/* Accessor function for attackSides. */
int Creature::showAttackSides()
{
	return attackSides;
}

/* Accessor function for defenseDice. */
int Creature::showDefenseDice()
{
	return defenseDice;
}

/* Accessor function for defenseSides. */
int Creature::showDefenseSides()
{
	return defenseSides;
}

/* Accessor function for armor. */
int Creature::showArmor()
{
	return armor;
}

/* Accessor function for strength. */
int Creature::showStrength()
{
	return strength;
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
void Creature::setName(string order)
{
	type = order + type;
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
	strength = 8;
	isGoblin = true;
	type = "Goblin";
}

/* attack() function for the Goblin derived class. This one is a redefined
	version of the attack() function from the Creature class. For a goblin,
	rolling a 12 causes the Goblin's opponent's attack roll halved. I took
	this to mean that an opponent with a 2d6 (so 2-12) becomes 2d3 (2-6)
	instead of a 1d6 (1-6). This is because one of the other creatures has
	a 3d6 attack, which you can't really divide by half evenly. */
int Goblin::attack(Creature &target)
{
	int attackSum = 0;
	for (int i = 0; i < attackDice; i++)
	{
		attackSum += rand() % attackSides + 1;
	}
	if (attackSum == 12 && target.showIsGoblin() == false)
	{
		target.setAttackSides(target.showAttackSides() / 2);
		cout << "The goblin hit the Achilles of the "
			<< target.showType() << "!" << endl;
		cout << "The attack power of the " << target.showType()
			<< " is now " << target.showAttackDice() << "d"
			<< target.showAttackSides() << "!" << endl;
	}
	return attackSum;
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
	strength = 12;
	isGoblin = false;
	type = "Barbarian";
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
	strength = 18;
	isGoblin = false;
	type = "Reptile Person";
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
	strength = 12;
	isGoblin = false;
	type = "Blue Man";
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
	strength = 12;
	isGoblin = false;
	type = "Shadow";
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
