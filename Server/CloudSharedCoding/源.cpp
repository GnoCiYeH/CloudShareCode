#include"Log.h"
#include"TcpServer.h"
using namespace std;
int main()
{
	Log::Logger logger;
	TcpServer server("192.168.239.129", 9098, logger);
	server.tcpStart();


	/*std::cout << Package::PackageType::LOGIN << std::endl;
	std::cout << Package::PackageType::REGISTER << std::endl;
	std::cout << Package::PackageType::TEXT_CHANGE << std::endl;
	std::cout << Package::PackageType::GET_PROJECT << std::endl;
	std::cout << Package::PackageType::INIT_PROJS << std::endl;
	std::cout << Package::PackageType::SUBMIT << std::endl;
	std::cout << Package::PackageType::POST_PROJECT << std::endl;
	std::cout << Package::PackageType::NEW_PROJECT << std::endl;
	std::cout << Package::PackageType::DEL_PROJECT << std::endl;
	std::cout << Package::PackageType::GET_FILE << std::endl;
	std::cout << Package::PackageType::NEW_FILE << std::endl;
	std::cout << Package::PackageType::DEL_FILE << std::endl;*/

	/*char buf[4];
	intToBytes(11, buf, 0, 4);
	std::cout << bytesToInt(buf, 0, 4);*/
	return 0;
}