/******************************************************************************
File name: creature.h
Description: Header file for the Creature class and its subclasses.
******************************************************************************/

#ifndef CREATURE_H
#define CREATURE_H

#include <string>
using std::string;

/* The class Creature is an abstract class with pure virtual functions the
	creature type. */
class Creature
{
public:
	Creature();
	void combatTurn(Creature &attacker, Creature &defender);
	/* It seems a bit unnecessary to have this be a pure virtual function,
		but I made it this way to prevent any instances of trying to
		initialize a Creature object. */
	virtual string showType() = 0;
	int showAttackDice();
	int showAttackSides();
	int showDefenseDice();
	int showDefenseSides();
	int showArmor();
	int showStrength();
	bool showIsGoblin();
	bool isAlive();
	void setAttackSides(int value);
	void setName(string order);
protected:
	/* I chose to redefine the attack function for the Goblin derived
		class. */
	virtual int attack(Creature &target);
	/* I chose to redefine the defense function for the Shadow derived
		class. */
	virtual int defense();
	int attackDice;
	int attackSides;
	int defenseDice;
	int defenseSides;
	int armor;
	int strength;
	bool isGoblin;
	string type;
};

class Goblin : public Creature
{
public:
	Goblin();
private:
	virtual int attack(Creature &target);
	virtual string showType()
	{
		return type;
	}
};

class Barbarian : public Creature
{
public:
	Barbarian();
	virtual string showType()
	{
		return type;
	}
};

class Reptile : public Creature
{
public:
	Reptile();
	virtual string showType()
	{
		return type;
	}
};

class BlueMen : public Creature
{
public:
	BlueMen();
	virtual string showType()
	{
		return type;
	}
};

class Shadow : public Creature
{
public:
	Shadow();
private:
	virtual int defense();
	virtual string showType()
	{
		return type;
	}
};

#endif
