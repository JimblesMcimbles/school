/******************************************************************************
File name: room.h
Description: Header file for the Room superclass.
******************************************************************************/

#ifndef ROOM_H
#define ROOM_H

#include <string>
using std::string;

// forward declaration of Player class
class Player;

/* The class Room is an abstract base class for the spaceship's Rooms. */
class Room
{
public:
	Room();
	Room(string roomName, string roomDirections, string roomDescription);
	void setRoomDirections(Room* northRoom, Room* southRoom, Room* eastRoom,
		Room* westRoom);
	virtual string getRoomName();
	virtual void roomAction(Player* player, bool showHint) = 0;
	void setHint(string hintText);
	void showHint();
protected:
	virtual void moveThrough(Player* player);
	Room* north;
	Room* south;
	Room* east;
	Room* west;
	string name;
	string directions;
	string description;
	string hint;
};

#endif