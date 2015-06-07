/******************************************************************************
File name: player.h
Description: Implementation file for the Player class.
******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "player.h"
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

int inputInt(int lower, int upper);

/* Default constructor for the Player class. Takes no arguments. */
Player::Player()
{
	this->health = 10;
}

/* setCurrentRoom() sets the current room for the Player.
	Input: Room pointer
	Output: none */
void Player::setCurrentRoom(Room* room)
{
	this->currentRoom = room;
}

/* getCurrentRoom() returns a pointer for the Player's current room.
	Input: none
	Output: pointer to Player's current Room */
Room* Player::getCurrentRoom()
{
	return this->currentRoom;
}

/* addItem() adds an item to the Player's inventory. The inventory can hold
	only three items. Checks inventory size, and asks if Player wants to
	discard an item to add a new one if the inventory is already full.
	Input: string item to add
	Output: none */
void Player::addItem(string item)
{
	if (this->inventory.size() < 3)
	{
		this->inventory.push_back(item);
		cout << "You added the " << item << " to your pocket." << endl;
	}
	else
	{
		cout << "Your pocket already has three items!" << endl;
		cout << "Do you want to drop one? (yes/No)" << endl;
		string response;
		getline(cin, response);
		if (response == "y" || response == "Y" || response == "yes" ||
			response == "YES" || response == "Yes")
		{
			this->dropItem();
			if(this->inventory.size() < 3)
			{
				this->addItem(item);
			}
		}
	}
}

/* dropItem() drops an item from the Player's inventory. Displays the current
	items in the Player's invemtory, then prompts for the user for one to drop.
	Input: none
	Output: none */
void Player::dropItem()
{
	cout << "Your pocket contains:" << endl;
	for (unsigned int i = 0; i < this->inventory.size(); i++)
	{
		cout << i + 1 << ": " << this->inventory.at(i) << endl;
	}
	cout << "Which one do you want to drop? Choose 1-"
		<< this->inventory.size() << ", or 0 to cancel: ";
	int input = inputInt(0, this->inventory.size());
	if (input == 0)
	{
		return;
	}
	else
	{
		cout << "Dropping: " << this->inventory.at(input - 1) << endl;
		this->inventory.erase(inventory.begin() + input - 1);
	}
}

/* showInventory() shows the Player's current inventory.
	Input: none
	Output: none */
void Player::showInventory()
{
	cout << "Your pocket contains " << this->inventory.size() << " of 3 items:"
		<< endl;
	for (unsigned int i = 0; i < this->inventory.size(); i++)
	{
		cout << i + 1 << ": " << this->inventory.at(i) << endl;
	}
}

/* searchInventory() searches the Player inventory for an item. Returns -1 if
	item is not found.
	Input: string item to search for
	Output: int position of item in the inventory, -1 if not present */
int Player::searchInventory(string searchItem)
{
	unsigned int index = 0;
	int position = -1;
	bool found = false;
	
	while (index < this->inventory.size() && !found)
	{
		if (this->inventory.at(index) == searchItem)
		{
			found = true;
			position = index;
		}
		index++;
	}
	
	return position;
}

/* damage() calculates the damage done to a Player by a Room. Damage is halved
	if the Player has the space suit in the inventory.
	Input: int damage
	Output: none */
void Player::damage(int damageAmt)
{
	bool hasSpaceSuit = false;
	for (unsigned int i = 0; i < this->inventory.size(); i++)
	{
		if (this->inventory.at(i) == "space suit")
		{
			hasSpaceSuit = true;
		}
	}
	if (hasSpaceSuit)
	{
		cout << "You have a protective space suit! You avoid some damage!"
			<< endl;
		this->health -= damageAmt / 2;
		cout << "You take " << damageAmt / 2 << " damage!" << endl;
	}
	else
	{
		this->health -= damageAmt;
		cout << "You take " << damageAmt << " damage!" << endl;
	}
	if (this->health < 0)
	{
		this->health = 0;
	}
	cout << "You have " << this->health << " health!" << endl;
}

/* heal() heals some amount to a Player's health. Does not go over 10.
	Input: int heal amount
	Output: none */
void Player::heal(int healAmt)
{
	if (this->health < 10 && this->health > 0)
	{
		cout << "You heal " << healAmt << " health." << endl;
		this->health += healAmt;
		if (this->health > 10)
		{
			this->health = 10;
		}
		cout << "You have " << this->health << " health!" << endl;
	}
	else
	{
		cout << "You already have full health." << endl;
	}
}

/* isAlive() returns a boolean depending on if a Player still has health > 0.
	Input: none
	Output: true if health > 0, false if health <= 0 */
bool Player::isAlive()
{
	if (this->health <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}