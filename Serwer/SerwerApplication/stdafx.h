#pragma once
#define WIN32_LEAN_AND_MEAN

#include <cstdio>
#include <cmath>
#include <chrono>	// clock implementation
#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include <ws2tcpip.h>
#include <winsock2.h> 
#include <ctime> 
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define CIRCLE_SIZE 5
#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT "8080"
#define TIMEOUT 10.0

//player input defines
#define BALL_CONTROL 1
#define KICK 2
#define MOUSE_LEFT 4
#define MOUSE_RIGHT 8
#define MOUSE_UP 16
#define MOUSE_DOWN 32
using namespace std;

struct UserInput
{
	char command;
	double mouseXPos;
	double mouseYPos;
};

#include "Vector2D.h"
#include "CirclePhysical.h"
#include "Ball.h"
#include "Player.h"
#include "Post.h"
#include "Goal.h"
#include "Team.h"
#include "GameEngine.h"
#include "ClientData.h"
#include "TransferManager.h"
#include "Server.h"