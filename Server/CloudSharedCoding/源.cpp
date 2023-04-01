#include"Log.h"
#include"TcpServer.h"
using namespace std;
int main()
{
	Log::Logger logger;
	TcpServer server("192.168.239.129", 9098, logger);
	server.tcpStart();
}