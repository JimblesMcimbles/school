/******************************************************************************
File name: subroom.cpp
Description: Implementation file for the subclasses of the base class Room.
******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include "subroom.h"
#include "player.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

/********************** ItemGivenRoom **********************/

/* Default constructor for ItemGivenRoom. Takes no arguments. */
ItemGivenRoom::ItemGivenRoom() : Room()
{
	// intentionally left blank
}

/* Constructor for ItemGivenRoom. Sets the room name, directions, description,
	and item given to the Player. */
ItemGivenRoom::ItemGivenRoom(string roomName, string roomDirections,
	string roomDescription, string givenItem) : Room(roomName, roomDirections,
	roomDescription)
{
	this->given = givenItem;
}

/* giveItem() gives an item to a Player.
	Input: Player pointer
	Output: none */
void ItemGivenRoom::giveItem(Player* player)
{
	cout << "There is a " << this->given << " on the ground." << endl;
	// adds item to player's inventory if not already present
	if (player->searchInventory(this->given) == -1)
	{
		cout << "You pick it up." << endl;
		player->addItem(this->given);
	}
	else
	{
		cout << "You already have one. This one must have materialized..."
			<< endl;
	}
}

/* roomAction() is the main action for interacting with the Room. Displays Room
	name, description, and then gives an item to the Player. May show a hint.
	Displays available directions, then prompts user to move through the Rom.
	Input: Player pointer, bool for showing hint
	Output: none */
void ItemGivenRoom::roomAction(Player* player, bool showHint)
{
	cout << "You are in " << this->name << "." << endl;
	cout << this->description << endl;
	if (showHint)
	{
		player->getCurrentRoom()->showHint();
	}

	this->giveItem(player);
	player->showInventory();
	cout << "You can go " << this->directions << "." << endl;
	this->moveThrough(player);
}

/********************** ItemNeededRoom **********************/

/* Default constructor for ItemNeededRoom. Takes no arguments. */
ItemNeededRoom::ItemNeededRoom() : Room()
{
	// intentionally left blank
}

/* Constructor for ItemNeededRoom. Sets the room name, directions, description,
	item required to move forward, and the restricted direction. */
ItemNeededRoom::ItemNeededRoom(string roomName, string roomDirections,
	string roomDescription, string neededItem, string lockedDoor) :
	Room(roomName, roomDirections, roomDescription)
{
	this->needed = neededItem;
	this->locked = lockedDoor;
}

/* roomAction() is the main action for interacting with the Room. Displays Room
	name and description. May show a hint. Displays available directions, and
	tells user the locked direction and item required to unlock it, then
	prompts user to move through the Room.
	Input: Player pointer, bool for showing hint
	Output: none */
void ItemNeededRoom::roomAction(Player* player, bool showHint)
{
	cout << "You are in " << this->name << "." << endl;
	cout << this->description << endl;
	if (showHint)
	{
		player->getCurrentRoom()->showHint();
	}

	cout << "You can go " << this->directions << ", but you can only go "
		<< locked << " if you have\nthe " << needed << "." << endl;
	player->showInventory();
	this->moveThrough(player);
}

/* moveThrough() is the redefined moveThrough() function for the ItemNeededRoom
	class. Searches a Player's inventory for item required by the Room, then
	prompts the user to move through the Room. If the user tries to move in the
	restricted direction without the required item, the function will not allow
	this move. Sets the Player's next room.
	Input: Player pointer
	Output: none */
void ItemNeededRoom::moveThrough(Player* player)
{
	Room* nextRoom;
	bool hasItem = false;
	if (player->searchInventory(this->needed) != -1)
	{
		hasItem = true;
	}

	do
	{
		string input;
		cout << "Which direction do you want to try? (N/S/E/W)"
			<< endl;
		getline(cin, input);

		if (input == "north" || input == "North" ||
			input == "n" || input == "N")
		{
			if (this->locked == "north" && !hasItem)
			{
				cout << "You don't have the " << this->needed << ". You "
					"cannot go " << locked << "." << endl;
				nextRoom = NULL;
			}
			else if (this->north == NULL)
			{
				cout << "You cannot go that way..." << endl << endl
					<< "Available choices: " << this->directions << endl
					<< endl;
				nextRoom = NULL;
			}
			else
			{
				cout << "You go through the door."
					<< endl << endl;
				nextRoom = this->north;
			}
		}
		else if (input == "south" || input == "South" ||
			input == "s" || input == "S")
		{
			if (this->locked == "south" && !hasItem)
			{
				cout << "You don't have the " << this->needed << ". You "
					"cannot go " << locked << "." << endl;
				nextRoom = NULL;
			}
			else if (this->south == NULL)
			{
				cout << "You cannot go that way..." << endl << endl
					<< "Available choices: " << this->directions << endl
					<< endl;
				nextRoom = NULL;
			}
			else
			{
				cout << "You go through the door."	<< endl << endl;
				nextRoom = this->south;
			}
		}
		else if (input == "east" || input == "East" ||
			input == "e" || input == "E")
		{
			if (this->locked == "east" && !hasItem)
			{
				cout << "You don't have the " << this->needed << ". You "
					"cannot go " << locked << "." << endl;
				nextRoom = NULL;
			}
			else if (this->east == NULL)
			{
				cout << "You cannot go that way..." << endl << endl
					<< "Available choices: " << this->directions << endl
					<< endl;
				nextRoom = NULL;
			}
			else
			{
				cout << "You go through the door."
					<< endl << endl;
				nextRoom = this->east;
			}
		}
		else if (input == "west" || input == "West" ||
			input == "w" || input == "W")
		{
			if (this->locked == "west" && !hasItem)
			{
				cout << "You don't have the " << this->needed << ". You "
					"cannot go " << locked << "." << endl;
				nextRoom = NULL;
			}
			else if (this->west == NULL)
			{
				cout << "You cannot go that way..." << endl << endl
					<< "Available choices: " << this->directions << endl
					<< endl;
				nextRoom = NULL;
			}
			else
			{
				cout << "You go through the door." << endl << endl;
				nextRoom = this->west;
			}
		}
		else if (input == "exit" || input == "Exit")
		{
			cout << "You've chosen to exit the game..." << endl
				<< "Sometimes, giving up is okay." << endl;
			exit(0);
		}
		else
		{
			cout << "Invalid input!" << endl;
			nextRoom = NULL;
		}
	}
	while (nextRoom == NULL);

	player->setCurrentRoom(nextRoom);
}

/********************** RiddleRoom **********************/

/* Default constructor for RiddleRoom. Takes no arguments. */
RiddleRoom::RiddleRoom() : ItemGivenRoom()
{
	// intentionally left blank
}

/* Constructor for RiddleRoom. Sets the Room name, directions, description,
	riddle asked by the Room, correct answer, and item to reward. */
RiddleRoom::RiddleRoom(string roomName, string roomDirections,
	string roomDescription, string roomRiddle, string roomAnswer,
	string rewardItem) : ItemGivenRoom(roomName, roomDirections,
	roomDescription, rewardItem)
{
	this->riddle = roomRiddle;
	this->answer = roomAnswer;
	this->reward = rewardItem;
}

/* roomAction() is the main action for interacting with the Room. Displays Room
	name, description, and riddle. May show a hint. Prompts user for the answer
	and rewards an item for the correct answer, then prompts the user to move
	through the Room.
	Input: Player pointer, bool for showing hint
	Output: none */
void RiddleRoom::roomAction(Player* player, bool showHint)
{
	cout << "You are in " << this->name << "." << endl;
	cout << this->description << endl;
	if (player->searchInventory(this->given) == -1)
	{
		cout << this->riddle << endl;
		if (showHint)
		{
			player->getCurrentRoom()->showHint();
		}

		string playerAnswer;
		do
		{
			getline(cin, playerAnswer);
		}
		while (playerAnswer != this->answer && playerAnswer != "exit");

		if (playerAnswer == "exit")
		{
			cout << "Giving up? That's okay." << endl;
			exit(0);
		}
		this->giveItem(player);
	}
	else
	{
		cout << "You already have a " << this->given << "." << endl;
	}

	player->showInventory();
	cout << "You can go " << this->directions << "." << endl;
	this->moveThrough(player);
}

/* giveItem() is the function for the RiddleRoom to give an item to a player.
	Input: Player pointer
	Output: none */
void RiddleRoom::giveItem(Player *player)
{
	cout << "A " << this->given << " materializes before you." << endl;
	if (player->searchInventory(this->given) == -1)
	{
		cout << "You pick it up." << endl;
		player->addItem(this->given);
	}
	else
	{
		cout << "You already have one." << endl;
	}
}

/********************** DamageRoom **********************/

/* Default constructor for DamageRoom. Takes no arguments. */
DamageRoom::DamageRoom() : Room()
{
	// intentionally left blank
}

/* Constructor for DamageRoom. Sets the Room name, directions, description, and
	damage dealt by the room. */
DamageRoom::DamageRoom(string roomName, string roomDirections,
	string roomDescription, int roomDamage) : Room(roomName, roomDirections,
	roomDescription)
{
	this->damage = roomDamage;
}

/* roomAction() is the main action for interacting with the Room. Displays Room
	name and description, then damages the player. May show a hint. Prompts the
	uesr to move through the Room.
	Input: Player pointer, bool for showing hint
	Output: none */
void DamageRoom::roomAction(Player* player, bool showHint)
{
	cout << "You are in " << this->name << "." << endl;
	cout << this->description << endl;
	player->damage(this->damage);
	if (!player->isAlive())
	{
		return;
	}
	if (showHint)
	{
		player->getCurrentRoom()->showHint();
	}

	cout << "You can go " << this->directions << "." << endl;
	this->moveThrough(player);
}

/********************** HealRoom **********************/

/* Default constructor for HealRoom. Takes no arguments. */
HealRoom::HealRoom() : Room()
{
	// intentionally left blank
}

/* Constructor for HealRoom. Sets the Room name, directions, description, and
	health healed by the room. */
HealRoom::HealRoom(string roomName, string roomDirections,
	string roomDescription, int roomHeal) : Room(roomName, roomDirections,
	roomDescription)
{
	this->heal = roomHeal;
}

/* roomAction() is the main action for interacting with the Room. Displays Room
	name and description, then heals the player. May show a hint. Prompts the
	uesr to move through the Room.
	Input: Player pointer, bool for showing hint
	Output: none */
void HealRoom::roomAction(Player* player, bool showHint)
{
	cout << "You are in " << this->name << "." << endl;
	cout << this->description << endl;
	player->heal(this->heal);
	if (showHint)
	{
		player->getCurrentRoom()->showHint();
	}

	cout << "You can go " << this->directions << "." << endl;
	this->moveThrough(player);
}
