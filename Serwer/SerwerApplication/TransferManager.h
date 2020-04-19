#pragma once
#include "stdafx.h"

struct PlayerInitializePack
{
	string playerNickname;
	unsigned int playerNumber;
	unsigned int playerTeam;
};


class TransferManager
{
private:
	vector<ClientData*> clientsData;
	vector<thread> clientsThreads;
	void communicate(ClientData* data, unsigned int threadNumber);

	vector<bool> ifdataToSend;				// array of flags set to True, if dataContainer contains new data ready to send to clients
	vector<double> dataPackToSend;			// vector holding serialized data
	PlayerInitializePack* initPackToSend;	// pointer to an array containing init pack, which needs to be sent to the clients
	unsigned int dataContainerLength;		// length of data in dataContainer

	bool ifGameRunning;
	mutex serializationSemaphore;			//semaphore for critical section

	bool charToBool(char flag);
	string bufferToString(char* buffer, int length);
	unsigned short generateNewNumber();

	// return pointer to recv buffer
	// if error occured, return 0
	// otherwise, return 1
	int customRecv(ClientData* data, char* recvbuf);

	void disablePlayer(ClientData* data);
public:
	TransferManager();
	~TransferManager();
	void newClient(SOCKET clientSocket);
	bool readyToPlay();
	void buildInitializationPack();						// send info to all client threads, that they need to send initialization pack to their clients
	void deleteInitializationPack();					// send info to all client threads, that they need to send initialization pack to their clients
	void dataSent(int threadNumber);					// as a client thread, set your ifNewData flag to false
	vector<ClientData*>* getClientsData();
	void manageInputs(ClientData* pClientData);
	void gameSerialize(GameEngine* pGame);
	void readyToPlayReset();
	void setGameRunning(bool ifGameRunning);
};