/******************************************************************************
File name: room.cpp
Description: Implementation file for the Room superclass.
******************************************************************************/

#include <iostream>
#include <cstdlib>
#include "room.h"
#include "player.h"
using std::cout;
using std::cin;
using std::endl;

/********************** Room **********************/

/* Default constructor for the base class Room. Takes no arguments. */
Room::Room()
{
	this->north = NULL;
	this->south = NULL;
	this->east = NULL;
	this->west = NULL;
}

/* Constructor for the base class Room. Takes the room name, directions, and
	a room description, and initializes all directions to NULL. */
Room::Room(string roomName, string roomInfo, string roomDescription)
{
	this->name = roomName;
	this->directions = roomInfo;
	this->description = roomDescription;
	this->north = NULL;
	this->south = NULL;
	this->east = NULL;
	this->west = NULL;
}

/* setRoomDirections() sets the directions for the Rooms in the spaceship. 
	Input: four Room pointers, north, south, east and west
	Output: none */
void Room::setRoomDirections(Room* northRoom, Room* southRoom, Room* eastRoom,
	Room* westRoom)
{
	this->north = northRoom;
	this->south = southRoom;
	this->east = eastRoom;
	this->west = westRoom;
}

/* getRoomName() returns the name of the Room as a string.
	Input: none
	Output: Room name string */
string Room::getRoomName()
{
	return this->name;
}

/* moveThrough() prompts the user to move from the current room into the next
	room. Sets the Player's next room.
	Input: pointer to a Player
	Output: none */
void Room::moveThrough(Player* player)
{
	Room* nextRoom;
	do
	{
		string input;
		cout << "Which direction do you want to try? (N/S/E/W)"
			<< endl;
		getline(cin, input);

		if (input == "north" || input == "North" ||
			input == "n" || input == "N")
		{
			if (this->north == NULL)
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
			if (this->south == NULL)
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
			if (this->east == NULL)
			{
				cout << "You cannot go that way..." << endl << endl
					<< "Available choices: " << this->directions << endl
					<< endl;
				nextRoom = NULL;
			}
			else
			{
				cout << "You go through the door." << endl << endl;
				nextRoom = this->east;
			}
		}
		else if (input == "west" || input == "West" ||
			input == "w" || input == "W")
		{
			if (this->west == NULL)
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

/* setHint() sets a string as a hint for a Room.
	Input: hint string
	Output: none */
void Room::setHint(string hintText)
{
	this->hint = hintText;
}

/* showHint() displays the hint for a Room.
	Input: none
	Output: none */
void Room::showHint()
{
	cout << "Hint: " << this->hint << endl << endl;
}