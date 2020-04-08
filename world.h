#pragma once
#include <vector>
#include <time.h>

constexpr auto UPDATE_FREQUENCY = 0.5f;

class Entity;
class Exit;
class NPC;
class Player;
class Room;

class World {
public:
	clock_t tick_timer;
	std::vector<Entity*> entities;

	World();
	~World();
	bool Update(std::vector<std::string>& args);
	bool ParseCommand(std::vector<std::string>& args);
	void GameLoop();

private:
	//Rooms
	Room* forest;
	Room* forest2;
	Room* forest3;
	Room* house;
	Room* basement;
	Room* clearing;
	//Exits
	Exit* front_door;
	Exit* basement_door;
	Exit* back_door;
	Exit* forest_path;
	Exit* forest_path2;
	Exit* forest_path3;
	//Enemies
	NPC* butler;
	NPC* wolf;
	//Player
	Player* player;

	bool HandleOneWordInput(std::vector<std::string>& args);
	bool HandleTwoWordInput(std::vector<std::string>& args);
	bool HandleFourWordInput(std::vector<std::string>& args);
	void MovePlayer(std::vector<std::string>& args, const char* param);
	void CreateRooms();
	void CreateExits();
	void CreateEnemies();
	void CreateItems();
	void CreatePlayer();
};
