/******************************************************************************
File name: maze.cpp
Description: Lab 8. Build a linked structure using pointers: a maze using
	dynamic memory allocation.
******************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>
using std::cout;
using std::cin;
using std::endl;
using std::string;

struct Room
{
	string name;
	string info;
	Room* north;
	Room* south;
	Room* east;
	Room* west;
	Room(string roomName, string roomDesc,
		Room* northRoom = NULL, Room* southRoom = NULL,
		Room* eastRoom = NULL, Room* westRoom = NULL)
	{
		name = roomName;
		info = roomDesc;
	}
};

/* setRoom() sets the directions for the room. This is necessary because the
	all rooms must be initialized before setting rooms (can't try to set
	a room to go somewhere that doesn't exist yet). */
void setRoom(Room* room, Room* northRoom, Room* southRoom,
	Room* eastRoom, Room* westRoom);
/* roomInfo() displays the room name and available directions. */
void roomInfo(Room* room);
/* moveThrough() is the main function for moving between Rooms. */
Room* moveThrough(Room* room);

int main()
{
	//initialize Rooms and descriptions
	Room* end = new Room("the end", "");
	Room* begin = new Room("the beginning", "South or East");
	Room* red = new Room("a red room", "South or West");
	Room* blue = new Room("a blue room", "North, South, or East");
	Room* black = new Room("a black room", "South or East");
	Room* green = new Room("a green room", "North, South, East, or West");

	//set Room directions
	setRoom(begin, NULL, blue, red, NULL);
	setRoom(red, NULL, black, NULL, begin);
	setRoom(blue, begin, green, black, NULL);
	setRoom(black, NULL, green, red, NULL);
	setRoom(green, black, end, black, blue);

	//room label display
	bool displayInfo = true;
	cout << "Welcome to the escape game..." << endl
		<< "Would you like to show room information? (Yes/no)"
		<< endl;
	string input;
	getline(cin, input);
	if (input == "no" || input == "NO" || input == "n" || input == "N")
	{
		displayInfo = false;
	}
	cout << endl;
	
	//begins maze
	Room* currentRoom = begin;
	do
	{
		if (displayInfo == true)
		{
			roomInfo(currentRoom);
		}
		currentRoom = moveThrough(currentRoom);
	}
	while (currentRoom->name != "the end");

	if (currentRoom->name == "the end")
	{
		cout << "Congratulations. You've escaped, but at what cost?"
			<< endl << "Everything outside is so different now..." << endl;
	}

	delete end;
	delete begin;
	delete blue;
	delete red;
	delete black;
	delete green;

	return 0;
}

/* setRoom() sets the directions for a Room.
	Input: Room, Room to the north, Room to the south, Room to the east,
		Room to the West
	Output: none */
void setRoom(Room* room, Room* northRoom, Room* southRoom,
	Room* eastRoom, Room* westRoom)
{
	room->north = northRoom;
	room->south = southRoom;
	room->east = eastRoom;
	room->west = westRoom;
}

/* roomInfo() displays the room name and information.
	Input: Room pointer
	Output: none */
void roomInfo(Room* room)
{
	cout << "You are in " << room->name << "." << endl;
	cout << "You can go " << room->info << "." << endl;
}

/* moveThrough() prompts the user to move through the current room and returns
	the next room that the user moves to.
	Input: current Room pointer
	Output: next Room pointer */
Room* moveThrough(Room* room)
{
	Room* returnRoom;
	do
	{
		string input;
		cout << "Which direction do you want to try? (N/S/E/W)"
			<< endl;
		getline(cin, input);

		if (input == "north" || input == "North" ||
			input == "n" || input == "N")
		{
			if (room->north == NULL)
			{
				cout << "You cannot go that way..." << endl << endl;
				returnRoom = NULL;
			}
			else
			{
				cout << "You go through the door. The hinges squeak."
					<< endl << endl;
				returnRoom = room->north;
			}
		}
		else if (input == "south" || input == "South" ||
			input == "s" || input == "S")
		{
			if (room->south == NULL)
			{
				cout << "You cannot go that way..." << endl << endl;
				returnRoom = NULL;
			}
			else
			{
				cout << "You go through the door."	<< endl << endl;
				returnRoom = room->south;
			}
		}
		else if (input == "east" || input == "East" ||
			input == "e" || input == "E")
		{
			if (room->east == NULL)
			{
				cout << "You cannot go that way..." << endl << endl;
				returnRoom = NULL;
			}
			else
			{
				cout << "You go through the door. It slams loudly behind you."
					<< endl << endl;
				returnRoom = room->east;
			}
		}
		else if (input == "west" || input == "West" ||
			input == "w" || input == "W")
		{
			if (room->west == NULL)
			{
				cout << "You cannot go that way..." << endl << endl;
				returnRoom = NULL;
			}
			else
			{
				cout << "You go through the door." << endl << endl;
				returnRoom = room->west;
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
			returnRoom = NULL;
		}
	}
	while (returnRoom == NULL);
	return returnRoom;
}
