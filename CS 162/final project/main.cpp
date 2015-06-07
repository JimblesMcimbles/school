/******************************************************************************
File name: main.cpp
Description: Final project. Escape from the burning spaceship game! You've been
	kidnapped and you awake in a prison cell in a burning spaceship. There's
	nobody else left on board, and the door to your cell is unlocked. A burning
	smell stings your nostrils, and you estimate that you have seven minutes to
	get out. From a window in the prison cell room, you see an escape pod
	attached to the ship. You decide to make your way over there. You have only
	pockets to store items you find, and you can only hold 3 items maximum.
	Some of the rooms contain substances that will harm you, and others can
	heal you. You can pick up items in some rooms, and other rooms will require
	you to solve a riddle before giving you an item. Other rooms will prevent
	you from going in a certain direction without a specific item. Will you
	make it out?
	***SPOILER***
	You will need the key and the code to beat the game. The other items are
	either red herrings or useful, but unnecessary. Turn on hints at the
	beginning to see hints for each room, including hints for the riddles.
******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>
#include "room.h"
#include "subroom.h"
#include "player.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ios;

/* inputInt() takes an input integer and checks if it is within a given
	range (as the arguments) and returns that user input integer. */
int inputInt(int lower, int upper);
/* gameOver() checks if the game is over because the */
bool gameOver(Player *player, double elapsedTime);

int main()
{
	// INITIALIZE THE ROOMS OF THE SPACESHIP
	// prison/beginning room (gives crowbar)
	string prisonDesc = "The cell door is open.\n";
		// "There are foreign symbols on the wall.";
	Room* prison = new ItemGivenRoom("a room with a prison cell",
		"North, South, East, or West", prisonDesc, "crowbar");
	prison->setHint("You have to find a way to the escape pod.");

	// red mist room (damages player)
	string redMistDesc = "There is a mysterious red mist in the room.\n"
		"Your skin tingles a bit, and it's not a good tingle... Ouch!";
	Room* redMist = new DamageRoom("a room full of red mist", "North or South",
		redMistDesc, 2);
	redMist->setHint("You should avoid this room!");

	// warp room (gives space suit)
	string warpDesc = "The room is darkened, except for a mysterious red "
		"glowing shaft of light\nin the south of the room. It appears to be "
		"a teleport. It's the only way out.";
	Room* warp = new ItemGivenRoom("a single warp room", "South", warpDesc,
		"space suit");
	warp->setHint("There are multiple ways in here, but only way out.");

	// blue key room (gives key)
	string blueKeyDesc = "There are small screens along the walls. You're not "
		"sure of their purpose.\nA glint of light catches your eye. "
		"It's a key.";
	Room* blueKey = new ItemGivenRoom("a bright blue room", "North or East",
		blueKeyDesc, "key");
	blueKey->setHint("This key looks important. You might want to keep it.");

	// code riddle room (rewards a code)
	string codeDesc = "A printer and computer sit on a table in the "
		"middle of the room.\nWhoever your kidnappers are, they've taken an "
		"interest in these two objects,\nfor whatever reason. Maybe you can "
		"get it to give you something useful.\nSurprisingly, English is on "
		"the screen. There appears to be a riddle.";
	string codeRiddle = "Poor people have it. Rich people need it. If you eat "
		"it, you'll die. What is\nit?";
	string codeAnswer = "nothing";
	Room* codeRoom = new RiddleRoom("a room with a computer and printer",
		"North or South", codeDesc, codeRiddle, codeAnswer, "code");
	codeRoom->setHint("The answer is in the form: n***ing.");

	// green healing room (heals player)
	string greenHealDesc = "The room glows a soft green. You feel your aches "
		"start to fade... ";
	Room* greenHeal = new HealRoom("a green healing room",
		"North, South, East, or West", greenHealDesc, 2);
	greenHeal->setHint("You should return to this room if you need to!");

	// broccoli riddle room (rewards broccoli)
	string broccoliDesc = "A large screen is before you. You are "
		"surprised to see English on it. A riddle\nis printed.";
	string broccoliRiddle = "What word becomes shorter when you add two "
		"letters to it?";
	string broccoliAnswer = "short";
	Room* broccoli = new RiddleRoom("a room with a large screen",
		"North, South, or West", broccoliDesc, broccoliRiddle,
		broccoliAnswer, "broccoli");
	broccoli->setHint("The answer is in the form: s***t.");

	// purple room with locked door (need key to go east)
	string purpleDesc = "There is a lock on the door to the east. You "
		"probably need some way to unlock\nit, like a key.";
	Room* purpleLock = new ItemNeededRoom("a purple room with a locked door",
		"North, South, East, or West", purpleDesc, "key", "east");
	purpleLock->setHint("It looks you need to find a key for this door.");

	// black sludge room (damages player)
	string blackDesc = "A black sludge covers the floor of this room. The "
		"smell is awful, and fumes\nsting your eyes.";
	Room* blackSludge = new DamageRoom("a pitch-black room", "South or East",
		blackDesc, 3);
	blackSludge->setHint("Ouch! You should avoid this room!");

	// carrot room (gives carrot)
	string carrotDesc = "The room is blank, save for a single solitary carrot "
		"on the ground. You wonder\nwhy it's there.";
	Room* carrot = new ItemGivenRoom("a white room", "North, East, or West",
		carrotDesc, "carrot");
	carrot->setHint("You don't think that carrot will be helpful...");

	// pink healing room (heals player)
	string pinkHealDesc = "The walls are lined with cotton candy. Whoever "
		"kidnapped you seems to have a\nliking for the sweet treat. You "
		"pull some off the wall and eat it. Somehow, it\nfeels soothing.";
	Room* pinkHeal = new HealRoom("a soft pink room", "North or West",
		pinkHealDesc, 1);
	pinkHeal->setHint("You should come back if you're low on health.");

	// airlock with locked door (need code to go north)
	string airlockDesc = "This appears to be the airlock room. The spaceship "
		"is in the room north, but\nit's not powered on. There's a keypad "
		"with symbols that might be numbers. You\nprobably need a code for "
		"this.";
	Room* airlock = new ItemNeededRoom("the airlock before the spaceship",
		"North, South, or West", airlockDesc, "code", "north");
	airlock->setHint("It looks like you need a code to go forward.");

	// escape pod room (only used to check for end game state)
	Room* end = new DamageRoom("the end", "", "", 0);

	// SET ROOM DIRECTIONS
	prison->setRoomDirections(redMist, greenHeal, broccoli, blueKey);
	warp->setRoomDirections(NULL, prison, NULL, NULL);
	redMist->setRoomDirections(warp, prison, NULL, NULL);
	blueKey->setRoomDirections(prison, NULL, greenHeal, NULL);
	greenHeal->setRoomDirections(prison, codeRoom, purpleLock, blueKey);
	codeRoom->setRoomDirections(greenHeal, warp, NULL, NULL);
	broccoli->setRoomDirections(warp, purpleLock, NULL, prison);
	purpleLock->setRoomDirections(broccoli, warp, carrot, greenHeal);
	blackSludge->setRoomDirections(NULL, carrot, airlock, NULL);
	carrot->setRoomDirections(blackSludge, NULL, pinkHeal, purpleLock);
	pinkHeal->setRoomDirections(airlock, NULL, NULL, carrot);
	airlock->setRoomDirections(end, pinkHeal, NULL, blackSludge);

	// DISPLAY THE BEGINNING DIALOG
	cout << "\n...\n\n\n...\n"
		"From a strange blackness, you feel yourself stirring. "
		"Your body aches, and you\nfeel cold. You open your eyes.\n\n"
		"It looks like you're in a prison cell. That's odd. How did you get "
		"here? There\nare symbols on the walls. You don't recognize them. "
		"The last thing you remember\nis a bright light and a struggle.\n\n"
		"You tell yourself that's the least of your worries.\n"
		"You look outside a window. You're in space.\n\n"
		"There appears to be a small space pod attached to this "
		"ship. You decide to make\nyour way over there.\n"
		"You smell something burning! Is the ship on fire? You should "
		"get out before\nthat happens! You estimate you have 7 minutes.\n\n"
		"You have pants pockets to hold items, but you can only hold three "
		"items." << endl << endl;

	// ADDITIONAL MENU OPTION TO DISPLAY HINT FOR ALL ROOMS
	cout << "Do you want to display hints for rooms? (yes/No)" << endl;
	string response;
	bool displayHints = false;
	getline(cin, response);
	if (response == "yes" || response == "Yes" || response == "y" ||
		response == "Y" || response == "YES")
	{
		displayHints = true;
	}

	// SUMMARY OF GAME INFORMATION
	cout << "Game information:" << endl << "Escape the spaceship!\n"
		"Get to the escape pod.\nYou have 7 minutes to get out.\n"
		"You can only hold 3 items in your pockets.\n"
		"Type \"exit\" to exit the game.\nGood luck!";
	cout << endl << endl << endl;

	Player* player = new Player();
	player->setCurrentRoom(prison);

	// START KEEPING TRACK OF TIME
	auto start = std::chrono::high_resolution_clock::now();
	auto timeNow = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsedTime;

	// GAMEPLAY
	// runs while Player still has health and 7 minutes have not elapsed and
	// while the Player still has not reached the end.
	do
	{
		// perform room action
		player->getCurrentRoom()->roomAction(player, displayHints);
		// calculate elapsed time
		timeNow = std::chrono::high_resolution_clock::now();
		elapsedTime = timeNow - start;
	}
	while (player->getCurrentRoom()->getRoomName() != "the end" &&
		!gameOver(player, elapsedTime.count()));

	// if Player is dead or time runs out
	if (gameOver(player, elapsedTime.count()))
	{
		if (!player->isAlive())
		{
			cout << "Game over... You died. A blackness takes over your "
				"senses..." << endl << endl;
		}
		else
		{
			cout << "You ran out of time! The ship explodes, and blackness "
				"takes over..." << endl << endl;
		}
	}
	// if Player reaches the end
	else if (player->getCurrentRoom()->getRoomName() == "the end")
	{
		cout << "You punch in the code resembling the symbols on your slip of "
			"paper.\nYou hear beeping... And suddenly, the door slides "
			"open! You run inside, and the\nspace pod door closes. You can "
			"see a blue planet off in the distance.\nYou push a big green "
			"button, and leave behind the burning spaceship!" << endl
			<< endl;
		cout << "Congratulations! You escaped!" << endl << endl;
	}
	else
	{
		cout << "Error! Game loop exited before reaching an end state!"
			<< endl;
		exit(1);
	}

	cout << "Thanks for playing!" << endl;

	delete prison;
	delete warp;
	delete redMist;
	delete blueKey;
	delete greenHeal;
	delete codeRoom;
	delete broccoli;
	delete purpleLock;
	delete blackSludge;
	delete carrot;
	delete pinkHeal;
	delete airlock;
	delete end;

	return 0;
}

/* inputInt() verifies integer input and ensures that it falls within a range.
	This version of the function has two integer arguments.
	Input: lower and upper bounds as int
	Output: returns user input number */
int inputInt(int lower, int upper)
{
	int tempNum;
	cin >> tempNum;
	while (cin.fail() || (tempNum < lower || tempNum > upper))
	{
		cin.clear();
		cin.ignore();
		cout << "Please enter an integer from " << lower << " to "
			<< upper << ": ";
		cin >> tempNum;
	}
	cin.ignore(1000, '\n');
	return tempNum;
}

bool gameOver(Player* player, double elapsedTime)
{
	bool gameOver = false;
	if (!player->isAlive())
	{
		gameOver = true;
	}

	if (elapsedTime > 420)
	{
		gameOver = true;
	}

	return gameOver;
}
