#pragma once
#define WIN32_LEAN_AND_MEAN

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
#define TEAM_SIZE 1
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"
#define TIMEOUT 10.0
using namespace std;

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