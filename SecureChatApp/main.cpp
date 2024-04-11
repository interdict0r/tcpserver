#include "Server/Server.h"

int main()
{
	CServer server("127.0.0.1", 8080);
	server.Start();




	return 0;
}
