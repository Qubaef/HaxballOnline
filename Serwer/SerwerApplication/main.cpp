#include "stdafx.h"

int main()
{
	Server server = Server();
	server.start();
	delete &server;
	return 1;
}