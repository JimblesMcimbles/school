/******************************************************************************
File name: subroom.h
Description: Header file for the subclasses of the base class Room.
******************************************************************************/

#ifndef SUBROOM_H
#define SUBROOM_H

#include <string>
#include "room.h"
using std::string;

// foward declaration of Player class
class Player;

/* ItemGivenRoom is derived from Room. Gives an item to the Player. */
class ItemGivenRoom : public Room
{
public:
	ItemGivenRoom();
	ItemGivenRoom(string roomName, string roomDirections,
		string roomDescription, string givenItem);
protected:
	virtual void roomAction(Player* player, bool showHint);
	virtual void giveItem(Player* player);
	string given;
};

/* ItemNeededRoom is derived from Room. Requires an item in the Player's
	inventory to go in a locked direction. */
class ItemNeededRoom : public Room
{
public:
	ItemNeededRoom();
	ItemNeededRoom(string roomNamem, string roomDirections,
		string roomDescription, string neededItem, string lockedDoor);
private:
	virtual void roomAction(Player* player, bool showHint);
	virtual void moveThrough(Player* player);
	string needed;
	string locked;
};

/* RiddleRoom is a derived class from ItemGivenRoom. Asks the user a riddle and
	rewards	the Player with an item once answered correctly. */
class RiddleRoom : public ItemGivenRoom
{
public:
	RiddleRoom();
	RiddleRoom(string roomName, string roomDirections, string roomDescription,
		string roomRiddle, string roomAnswer, string rewardItem);
private:
	virtual void roomAction(Player* player, bool showHint);
	virtual void giveItem(Player* player);
	string riddle;
	string answer;
	string reward;
};

/* DamageRoom is a derived class from Room. Damages the Player. */
class DamageRoom : public Room
{
public:
	DamageRoom();
	DamageRoom(string roomName, string roomDirections, string roomDescription,
		int roomDamage);
private:
	virtual void roomAction(Player* player, bool showHint);
	int damage;
};

/* HealRoom is a derived class from Room. Heals the Player. */
class HealRoom : public Room
{
public:
	HealRoom();
	HealRoom(string roomName, string roomDirections, string roomDescription,
		int roomHeal);
private:
	virtual void roomAction(Player* player, bool showHint);
	int heal;
};

#endif