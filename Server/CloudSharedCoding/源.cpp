#include"Log.h"
#include"TcpServer.h"
using namespace std;
int main()
{
	Log::Logger logger;
	TcpServer server("127.0.0.1", 9897, logger);
	server.tcpStart();
}