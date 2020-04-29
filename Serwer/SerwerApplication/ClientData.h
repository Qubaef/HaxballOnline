
/**
 * The following class describes and stores all players' data and information.
 */

#pragma once
#include "stdafx.h"

class ClientData
{
private:
	string nickname;			// player's nickname
	Player* pPlayer;			// pointer to this player
	SOCKET clientSocket;		// socket which allows client to communicate with the server
	bool ready;					// flag to confirm that player is ready to start the game
	unsigned short number;		// number from the back of the player's T-shirt :) 
	UserInput userInput;		// defined in "stdafx.h" file

public:
	ClientData(SOCKET clientSocket);
	~ClientData();
	
	bool getReady();				//getting to know if player is ready 
	void setReady(bool ready);		// setting that the player is ready
	
	void setPlayer(Player* pPlayer);// setter for a player
	Player* getPlayer();			// getter for a player
	
	SOCKET getSocket();				// getting socket used for a particular client and the server
	
	int getNumber();				// getting player's number ( the number from a back of the t-shirt :) )
	void setNumber(int number);		// setting that number
	
	string getNickname();			// getting user's nickname
	void setNickname(string nickname);// setting it

	UserInput getUserInput() const;	// getting users input

	void setUserInput(char* input);	// setting received data to player's atributes
};
