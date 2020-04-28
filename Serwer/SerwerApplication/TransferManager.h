#pragma once
#include "stdafx.h"

// structure created to store data sent to the player before game starts
struct PlayerInitializePack
{
	string playerNickname;
	unsigned int playerNumber;
	unsigned int playerTeam;
};

class TransferManager
{
private:
	vector<ClientData*> clientsData;		// set of all data about connected clients 
	vector<thread> clientsThreads;			// set of all threads created in case to communicate with the server
	vector<bool> ifdataToSend;				// array of flags set to True, if dataContainer contains new data ready to send to clients
	int threadsNumber;						// the number given to a particular thread

	// function creates communication between client and given thread 
	void communicate(ClientData* data, unsigned int threadNumber);

	vector<double> gamePackToSend;			// vector holding serialized data
	PlayerInitializePack* initPackToSend;	// pointer to an array containing init pack, which needs to be sent to the clients
	unsigned int dataContainerLength;		// length of data in dataContainer

	bool ifGameRunning;						// flag which tells if the game is still running 
	mutex gamePackToSendMutex;				// mutex for critical section
	mutex clientDataMutex;					// mutex for critical section
	mutex userInputMutex;					// mutex for critical section

	bool charToBool(char flag);				// converts char to bool variable 
	string bufferToString(char* buffer, int length);	// converts given buffer to string variable 
	unsigned short generateNewNumber();		// generates number for a new player 		

	// return pointer to recv buffer
	// if error occured, return 0
	// otherwise, return 1
	int customRecv(ClientData* data, char* recvbuf);

	// kicking the player out of the pitch ( due to bad communication or any other error ) 
	void disablePlayer(ClientData* data);

public:
	TransferManager();
	~TransferManager();
	// creates new thread for every newly connected client
	void newClient(SOCKET clientSocket);

	// returns if all players confirmed that they are ready to play
	bool readyToPlay();

	// send info to all client's threads that they need to send initialization pack to their clients
	void buildInitializationPack();

	void deleteInitializationPack();

	// gets information about all connected clients
	vector<ClientData*>* getClientsData();

	// gets and manages input data from connected clients
	void manageInputs(ClientData* pClientData);

	// serializes game to a compressed form
	void gameSerialize(GameEngine* pGame);

	// resets settings - we asume that all players are NOT ready to play, yet..
	void readyToPlayReset();

	// so there is NO data to send by all threads ( at the moment )
	void dataToSendReset();

	// setting bool variable to given value 
	void setGameRunning(bool ifGameRunning);

	//clear data of players in next game if disconnect occured
	void cleanPlayers();
};