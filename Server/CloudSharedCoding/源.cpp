#include"Log.h"
#include"TcpServer.h"
using namespace std;
int main()
{
	Log::Logger logger;
	TcpServer server("172.30.130.144", 9098, logger);
	server.tcpStart();

	/*std::cout << (int)(int)(int)Package::PackageType::LOGIN << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::REGISTER << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::TEXT_CHANGE << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::GET_PROJECT << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::INIT_PROJS << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::SUBMIT << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::POST_PROJECT << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::NEW_PROJECT << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::DEL_PROJECT << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::GET_FILE << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::NEW_FILE << std::endl;
	std::cout << (int)(int)(int)Package::PackageType::DEL_FILE << std::endl;*/

	/*char buf[4];
	intToBytes(11, buf, 0, 4);
	std::cout << bytesToInt(buf, 0, 4);*/
	return 0;
}