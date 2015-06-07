/******************************************************************************
File name: player.h
Description: Header file for the Player class.
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <stack>
#include <vector>
using std::stack;
using std::string;
using std::vector;

// forward declaration of Room class
class Room;

/* The class Player is the class for handling Player health and inventory. */
class Player
{
public:
	Player();
	void setCurrentRoom(Room* room);
	Room* getCurrentRoom();
	void addItem(string item);
	void dropItem();
	void showInventory();
	int searchInventory(string searchItem);
	void damage(int damageAmt);
	void heal(int healAmt);
	bool isAlive();
	stack<Room*> visitedRooms;
private:
	vector<string> inventory;
	Room* currentRoom;
	int health;
};

#endif