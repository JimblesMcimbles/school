/******************************************************************************
File name: creature.h
Description: Header file for the Creature class and its subclasses.
******************************************************************************/

#ifndef CREATURE_H
#define CREATURE_H

#include <string>
using std::string;

/* The class Creature is an abstract class with pure virtual functions. */
class Creature
{
public:
	Creature();
	// ~Creature();
	void combatTurn(Creature* attacker, Creature* defender);
	void regainStrength();
	// pure virtual function; redefined for every derived class
	virtual void addPoints(Creature* target) = 0;
	int showAttackDice();
	int showAttackSides();
	int showDefenseDice();
	int showDefenseSides();
	int showArmor();
	int showMaxStrength();
	int showStrength();
	int showPoints();
	string showType();
	string showName();
	string showTeam();
	bool showIsGoblin();
	bool isAlive();
	void setAttackSides(int value);
	void setName(string inputName);
protected:
	// redefined for Goblin derived class
	virtual int attack(Creature* target);
	// redefined for Shadow derived class
	virtual int defense();
	int attackDice;
	int attackSides;
	int defenseDice;
	int defenseSides;
	int armor;
	int maxStrength;
	int strength;
	int health;
	bool isGoblin;
	string type;
	string name;
	int points;
	string team;
};

class Goblin : public Creature
{
public:
	Goblin();
	Goblin(string inputName, string teamName);
private:
	virtual int attack(Creature* target);
	virtual void addPoints(Creature* target);

};

class Barbarian : public Creature
{
public:
	Barbarian();
	Barbarian(string inputName, string teamName);
	virtual void addPoints(Creature* target);
};

class Reptile : public Creature
{
public:
	Reptile();
	Reptile(string inputName, string teamName);
	virtual void addPoints(Creature* target);
};

class BlueMen : public Creature
{
public:
	BlueMen();
	BlueMen(string inputName, string teamName);
	virtual void addPoints(Creature* target);
};

class Shadow : public Creature
{
public:
	Shadow();
	Shadow(string inputName, string teamName);
private:
	virtual int defense();
	virtual void addPoints(Creature* target);
};

#endif
