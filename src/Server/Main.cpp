#include "GameServer/GameServer.hpp"

#include "Common/Serialize/Serializer.hpp"

int main() {
	GameServer server(1500);
	
	server.start();
	std::cout << "Server started...\n";

	getchar();
	
	std::cout << "Server stopping...\n";
	server.stop();
}