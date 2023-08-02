#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <csignal>
#include <ctime>
using namespace std;


class Item {
protected:

	int x;
	int y;
	string objName;
public:
	char character;
	virtual void description() = 0;
	virtual int getX() = 0;
	virtual int getY() = 0;
	virtual void setCoords(int x, int y) { return; }
	virtual string getObjName() {
		return "";
	};
};

class Player : public Item {
public:

	Player(int x, int y) {
		objName = "player";
		character = '@';
		this->x = x;
		this->y = y;
	}
	void description() override {

	}
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	int getX() override { return x; };
	int getY() override { return y; };

	void TakeItem(Item* item) {
		backpack = item;
	}
	Item& DropItem() {
		Item* temp = backpack;
		backpack = nullptr;
		return *temp;
	}
	bool HoldingObject() {
		return isHoldingObject;
	}
	void HoldingObject(bool isHolding) {
		isHoldingObject = isHolding;
	}
private:
	bool isHoldingObject = false;
	Item* backpack = nullptr;
};

class Box : public Item {
public:
	Box(string objName, string material, int weight, int width, int x, int y) {
		this->objName = objName;
		character = '?';
		this->material = material;
		this->weight = weight;
		this->width = width;
		this->x = x;
		this->y = y;
	}
	int getX() override {
		return x;
	}
	int getY() override {
		return y;
	}
	void description() override {
		cout << "object: " << objName << " material " << material << " weight: " << weight << " width: " << width << endl;
	}


private:
	string material;
	int weight;
	int width;
};



class FreeSpace : public Item {
public:
	FreeSpace() {
		objName = "FreeSpace";
		character = '#';
	}
	void description() override {

	}
	int getX() override { return x; };
	int getY() override { return y; };
};

class UnBreakableWall : public Item {
public:
	UnBreakableWall(string material, int x, int y) {
		objName = "UnBreakableWall";
		character = '%';
		this->material = material;
		this->x = x;
		this->y = y;
	}
	void description() override {

	}
	string getObjName() {
		return objName;
	}
	int getX() override { return x; };
	int getY() override { return y; };
private:
	string material;
};

class Door : public Item {
public:
	Door(bool doorToNext) {
		this->doorToNext = doorToNext;
		objName = "Door";
		character = '0';
	}
	void setCoords(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void setNewCoords(int x, int y) {
		this->newX = x;
		this->newY = y;
	}
	void description() override {
		cout << "Its just a door" << endl;
	}
	void generateNewRoom() {

	}
	bool isDoorToNext() {
		return doorToNext;
	}
	void setRoom(vector<vector<Item*>> room) {
		roomLink = room;
		roomLinked = true;
	}
	vector<vector<Item*>>* getRoom() {
		if (roomLinked) {
			return &roomLink;
		}
		return nullptr;
	}
	string getObjName() {
		return objName;
	}
	int getX() override { return newX; }
	int getY() override { return newY; }
	int getOldX() { return x; }
	int getOldY() { return y; }

private:
	bool doorToNext;
	int newX;
	int newY;
	vector<vector<Item*>> roomLink;
	bool roomLinked = false;
};

void showWorld(vector<vector<Item*>>& world) {
	for (int i = 0; i < world.size(); i++) {
		for (int j = 0; j < world[i].size(); j++) {
			cout << world[i][j]->character;
		}
		cout << endl;

	}
}


void generateRoom(vector <vector<Item*>>& room, Player* player, vector <vector<Item*>>* previousRoom = nullptr, int oldDoorX = -1, int oldDoorY = -1, int doorX = -1, int doorY = -1) {
	int start = 1;
	int end = 10;
	if (previousRoom != nullptr) {
		Door* doorToPrevious = new Door(false);
		doorToPrevious->setRoom(*previousRoom);
		doorToPrevious->setCoords(oldDoorX, oldDoorY);
		doorToPrevious->setNewCoords(doorX, doorY);
		delete room[doorX][doorY];
		room[doorX][doorY] = doorToPrevious;
	}
	for (int i = 0; i < room.size(); i++) {
		for (int j = 0; j < room[i].size(); j++) {
			if (i == player->getX() && j == player->getY()) {
				continue;
			}
			int result = start + rand() % (end - start + 1);
			if (result == 3 && room[i][j]->character == '#') { //box
				Box* box = new Box("Box", "wood", (5 + rand() % (45 - 5 + 1)), (2 + rand() % (20 - 2 + 1)), i, j);
				room[i][j] = box;
			}
			else if (result == 5 && room[i][j]->character == '#') { //unbreakable wall 
				UnBreakableWall* wall = new UnBreakableWall("Metal", i, j);
				room[i][j] = wall;
			}
		}
	}

	Door* doorToNext = new Door(true);
	int doorXNew = 0 + rand() % (room.size() - 1 - 0 + 1);
	int doorYNew = 0 + rand() % (room[room.size() - 1].size() - 1 - 0 + 1);
	while (true) {
		if (doorXNew != doorX && doorYNew != doorY) {
			room[doorXNew][doorYNew] = doorToNext;
			doorToNext->setCoords(doorXNew, doorYNew);
			break;
		}
		doorXNew = 0 + rand() % (room.size() - 1 - 0 + 1);
		doorYNew = 0 + rand() % (room[room.size() - 1].size() - 1 - 0 + 1);
	}

}

void cleanupMemory(vector<vector<Item*>>& world) {
	for (int i = 0; i < world.size(); i++) {
		for (int j = 0; j < world[i].size(); j++) {
			delete world[i][j];
		}
	}
}

void main() {
	int start = 1;
	int end = 10;
	srand(time(NULL));
	vector<vector<Item*>> world;
	for (int i = 0; i < 10; i++) {
		vector<Item*> temp;
		for (int j = 0; j < 10; j++) {
			FreeSpace* space = new FreeSpace;
			temp.push_back(space);
		}
		world.push_back(temp);
	}
	Player* player = new Player(world.size() / 2, 0);
	world[player->getX()][player->getY()] = player;
	generateRoom(world, player);
	showWorld(world);
	int x = 0; int y = 0;
	FreeSpace space;
	Item* tempObj = &space;
	while (true) {
		if (_kbhit()) {
			system("cls");
			showWorld(world);
			if (_getch() == 's') {
				if (player->getX() != world.size() - 1 && world[player->getX() + 1][player->getY()]->getObjName() != "UnBreakableWall") {
					world[player->getX()][player->getY()] = tempObj;
					player->setX(player->getX() + 1);
					tempObj = world[player->getX()][player->getY()];
					x = player->getX(); y = player->getY();
					world[player->getX()][player->getY()] = player;
				}
			}

			else if (_getch() == 'w') {
				if (player->getX() != 0 && world[player->getX() - 1][player->getY()]->getObjName() != "UnBreakableWall") {
					world[player->getX()][player->getY()] = tempObj;
					player->setX(player->getX() - 1);
					tempObj = world[player->getX()][player->getY()];
					x = player->getX(); y = player->getY();
					world[player->getX()][player->getY()] = player;
				}
			}
			else if (_getch() == 'd') {
				if (player->getY() != world[player->getX()].size() - 1 && world[player->getX()][player->getY() + 1]->getObjName() != "UnBreakableWall") {

					world[player->getX()][player->getY()] = tempObj;
					player->setY(player->getY() + 1);
					tempObj = world[player->getX()][player->getY()];
					x = player->getX(); y = player->getY();
					world[player->getX()][player->getY()] = player;
				}
			}
			else if (_getch() == 'a') {
				if (player->getY() != 0 && world[player->getX()][player->getY() - 1]->getObjName() != "UnBreakableWall") {

					world[player->getX()][player->getY()] = tempObj;
					player->setY(player->getY() - 1);
					tempObj = world[player->getX()][player->getY()];
					x = player->getX(); y = player->getY();
					world[player->getX()][player->getY()] = player;
				}
			}
			else if (_getch() == 'e') {
				if (tempObj->character != '#') {
					cout << endl;
					tempObj->description();

				}
			}
			else if (_getch() == 'g' && player->HoldingObject() == false) {
				if (tempObj->character == '?') {
					player->TakeItem(tempObj);
					tempObj = &space;
					player->HoldingObject(true);
					cout << endl;
					cout << "You took object" << endl;

				}
			}
			else if (_getch() == 'g' && player->HoldingObject() == true) {

				tempObj = &player->DropItem();

				player->HoldingObject(false);
				cout << endl;
				cout << "You drop object" << endl;
			}
			else if (_getch() == 'f') {
				if (tempObj->character == '0') { //door
					Door* door = dynamic_cast<Door*>(tempObj);
					if (door->getRoom() != nullptr) { //previous room
						int DoorX;
						int DoorY;
						if (door->isDoorToNext()) {
							DoorX = door->getX();
							DoorY = door->getY();
						}
						else {
							DoorX = door->getOldX();
							DoorY = door->getOldY();
						}
						world = *door->getRoom();
						tempObj = world[DoorX][DoorY];
						player->setX(DoorX);
						player->setY(DoorY);
						world[DoorX][DoorY] = player;
						system("cls");
						showWorld(world);
					}
					else { //new room
						int DoorX = 0 + rand() % (world.size() - 1 - 0 + 1);
						int DoorY = 0 + rand() % (world[world.size() - 1].size() - 1 - 0 + 1);
						vector < vector<Item*>> newRoom;
						for (int i = 0; i < 10; i++) {
							vector<Item*> temp;
							for (int j = 0; j < 10; j++) {
								FreeSpace* space = new FreeSpace;
								temp.push_back(space);
							}
							newRoom.push_back(temp);
						}

						int oldx = player->getX(); int oldy = player->getY();
						door->setCoords(player->getX(), player->getY());
						door->setNewCoords(DoorX, DoorY);

						player->setX(DoorX);
						player->setY(DoorY);
						world[oldx][oldy] = tempObj;
						generateRoom(newRoom, player, &world, oldx, oldy, DoorX, DoorY);
						door->setRoom(newRoom);
						world = newRoom;
						tempObj = world[DoorX][DoorY];
						world[DoorX][DoorY] = player;
						system("cls");
						showWorld(world);
					}
				}
			}
		}
	}
	cleanupMemory(world);
}