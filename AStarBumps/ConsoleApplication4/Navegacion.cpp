// Navegacion.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "windows.h"

using namespace std;

#include "Board.h"
#include "Path.h"

int currentLevel = 0;
Node* start = nullptr, *home = nullptr;
Node* bufferFloor = nullptr;

template<class T>
void removeFromVector(vector <T> &vec, T elem) {

	for (int i = vec.size() - 1; i >= 0; --i) {
		if (vec[i] == elem) {
			vec.erase(vec.begin() + i);
			break;
		}
	}
}

void addNeighbors(Node *node, vector<vector<Node>> &maze ) {

	int x = node->getX();
	int y = node->getY();
	int cols = maze[0].size();
	int rows = maze.size();

	if (x < cols - 1) {

		if (!maze[y][x].getHasWallRight()) {
			node->addNeighbor(&maze[y][x + 1]);
		}
	}

	if (x > 0) {

		if (!maze[y][x].getHasWallLeft()) {
			node->addNeighbor(&maze[y][x - 1]);
		}
	}
	if (y < rows - 1) {

		if (!maze[y][x].getHasWallDown()) {
			node->addNeighbor(&maze[y + 1][x]);
		}
	}
	if (y > 0) {

		if (!maze[y][x].getHasWallUp()) {
			node->addNeighbor(&maze[y - 1][x]);
		}
	}
}

template <class T>
bool isInVector(vector<T> &vec, T &elem) {
	for (T e : vec) {
		if (e == elem) {
			return true;
		}
	}

	return false;
}

int calculateDistance(Node *a, Node *b) {
	int dx = abs(a->getX() - b->getX());
	int dy = abs(a->getY() - b->getY()); 
	return (dx + dy); 
}

void updateWalls(vector <vector<Node>> &maze) {
	int cols = maze[0].size();
	int rows = maze.size();
	
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {	
			if (x - 1 < cols - 1) {

				if (maze[y][x].getHasWallRight()) {
					maze[y][x + 1].setHasWallLeft(true);
				}
			}

			if (x + 1 > 0) {

				if (maze[y][x ].getHasWallLeft()) {
					maze[y][x - 1].setHasWallRight(true);
				}
	
			}
			if (y -  1 < rows - 1) {
				
				if (maze[y][x].getHasWallDown()) {
					maze[y + 1][x].setHasWallUp(true);
				}
			}
			if (y + 1 > 0) {

				if (maze[y][x].getHasWallUp()) {
					maze[y - 1][x].setHasWallDown(true);
				}

			}
		}
	}
}

void printMaze(vector <vector <Node>> maze) {
	cout << "PRINTING MAZE...";
	for (int y = 0; y < maze.size(); ++y) {
		cout << endl;
		for (int x = 0; x < maze[0].size(); ++x) {
			if (maze[y][x].getHasWallUp()) {
				cout << " UP ";
			}
			else {
				cout << "|--|";
			}

			if (maze[y][x].getHasWallDown()) {
				cout << " DN ";
			}
			else {
				cout << "|--|";
			}
			if (maze[y][x].getHasWallLeft()) {
				cout << " LF ";
			}
			else {
				cout << "|--|";
			}
			if (maze[y][x].getHasWallRight()) {
				cout << " RT ";
			}
			else {
				cout << "|--|";
			}
			cout << "/";

		}
	}
}

//void searchForNextGoal(vector <vector <Node>>& maze, Node *start, Node *&end) {
//
//	int distance = 0, minorDistance = 1000, newX, newY;
//	cout << "INCIA DESDE " << start->getX() << " " << start->getY() << endl;
//	cout << "LAS NO VISITADAS SON : " << endl;
//	for (int y = 0; y < maze.size(); ++y) {
//		for (int x = 0; x < maze[0].size(); ++x) {
//			if (!maze[y][x].getWasVisited()) {
//				Node* candidate = &maze[y][x];
//				distance = calculateDistance(start, candidate);
//				cout << x << " " << y << "    " << distance << endl;
//				if (minorDistance > distance) {
//					minorDistance = distance;
//					end = candidate;
//
//					cout << "NUEVO VALOR DE END : " << x << " " << y << endl;
//				}
//			}
//		}
//	}
//
//	cout << "LA TILE MAS CERCANA NO VISITADA ES (" << end->getX() << "," << end->getY() << ")" << endl;
//}

vector<Node*> getCandidates(vector<vector<Node>>& maze) {
	vector <Node*> vec;
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {
			if (maze[y][x].getWasVisited()) {

			//	cout << "LA TILE VISITADA ES (" << x << "," << y << ")" << endl;
				vector <Node*> neighbors = maze[y][x].getNeighbors();

			//	cout << "SUS VECINOS NO VISITADOS SON : ";

				for (int i = neighbors.size() - 1; i >= 0; --i) {
					if (!neighbors[i]->getWasVisited()) {
						Node* elem = neighbors[i];
						if (!isInVector(vec, elem)) {
							vec.emplace_back(neighbors[i]);
				//			cout << "(" << neighbors[i]->getX() << "," << neighbors[i]->getY() << ")  ";
						}
					}
				}

				//cout << endl;
			}
		}
	}
	return vec;
}

vector <Node*> getClosestFrom(vector <Node*>& vec, Node* start) {
	vector <Node*> listOfClosest;

	int distance = 0, minorDistance = 1000;
	for (int i = 0; i < vec.size(); ++i) {
		Node* candidate = vec[i];
		distance = calculateDistance(start, candidate);
		if(minorDistance >= distance) {
			if (minorDistance > distance) {
				minorDistance = distance;
				listOfClosest.clear();
			}

			listOfClosest.emplace_back(candidate);
		}
	}

	//cout << "LA TILE NO VISITADA MAS CERCANA (PROVENIENTE DE UNA TILE VISITADA) ES : " << end->getX() << " " << end->getY() << endl;
	return listOfClosest;

}

Path getPath(vector <vector<Node>>& maze, int xS, int yS, int xE, int yE) {
	vector <Node*> openSet;
	vector <Node*> closedSet;
	Node* start = &maze[yS][xS];
	Node* end = &maze[yE][xE];
	Path finalPath;
	finalPath.setCost(100000);

	vector<Node*> endNeighbors = end->getNeighbors();

	for (vector<Node>& row : maze)
		for (Node& node : row) {
			node.clear();

		}
	
	openSet.emplace_back(start);


	while (openSet.size() > 0) {

		int winnerIndex = 0;
		for (int i = 0; i < openSet.size(); ++i) {
			if (openSet[i]->getF() < openSet[winnerIndex]->getF()) {
				winnerIndex = i;
			}
		}

		Node* current = openSet[winnerIndex];

		if (current == end) {
			// cout << "DONE!" << endl;
			vector <Node*> path;
			Node* temp = current;
			path.emplace_back(temp);

			int cost = 0;

			while (temp->getParent() != nullptr) {
				cost += temp->getCost();
				path.emplace_back(temp->getParent());
				temp = temp->getParent();
			}
			
			finalPath = Path(path);
			finalPath.setCost(cost);

			return finalPath;
		}

		removeFromVector(openSet, current);
		closedSet.emplace_back(current);

		addNeighbors(current, maze);

		for (Node* neighbor : current->getNeighbors()) {

			if ((neighbor->getWasVisited()) || neighbor == end) {

				if (!isInVector(closedSet, neighbor)) {
					//	int tempG = current->getG() + 1; // version without bump count
				
					int tempG =	current->getG() + neighbor->getCost();

					bool newPath = false;
					if (isInVector(openSet, neighbor)) {
						if (tempG < neighbor->getG()) {
							neighbor->setG(tempG);
							newPath = true;
						}
					}
					else {
						neighbor->setG(tempG);
						openSet.emplace_back(neighbor);
						newPath = true;
					}
					if (newPath) {
						neighbor->setH(calculateDistance(neighbor, end));
						neighbor->setF(neighbor->getG() + neighbor->getH());
						neighbor->setParent(current);
					}

				}
			}

		}


	}

	cout << "NO SOLUTION" << endl;
	return Path();

}

void completeSection(vector<Board*>& safeKeeper) {
	bool roundComplete = false;
	bool firstRun = false;
	while (!roundComplete) {
		cout << "-------------------------L E V E L  " << currentLevel << "-------------------------" << endl;

		Board* board = safeKeeper[currentLevel];

		vector<vector<Node>>& maze = board->getMap();
		if (!firstRun) {
			start = &maze[2][0];
			home = &maze[2][0];
			firstRun = true;
		}else
			start = bufferFloor;

		if (board->getRampAt(start) != nullptr) {
			board->getRampAt(start)->setUsed(true);
		}


		for (int y = 0; y < maze.size(); ++y) {
			for (int x = 0; x < maze[0].size(); ++x) {
				addNeighbors(&maze[y][x], maze);
			}
		}

		vector<Node*> candidates = getCandidates(maze);


		while (candidates.size() != 0) {

			vector <Node*> target = getClosestFrom(candidates, start);

			int minorIndex = 0, minorCost = 10000;
			Path bestPath;
			for (int i = 0; i < target.size(); ++i) {
				Path buffer = getPath(maze, start->getX(), start->getY(), target[i]->getX(), target[i]->getY());
				int cost = buffer.getCost();
				if (cost < minorCost) {
					minorIndex = i;
					minorCost = cost;
					bestPath = buffer;
				}
			}


			Node* goal = target[minorIndex];

			cout << "StartX: " << start->getX() << " StartY: " << start->getY() << endl;
			cout << "EndX: " << goal->getX() << " EndY: " << goal->getY() << endl;

			if (target.size() > 0) {
				bestPath.printPath();
				goal->setWasVisited(true);
				start = goal;

				if (goal->getIsRamp()) {

					break;
				}
				candidates = getCandidates(maze);
				int i;
				cin >> i;
			}
			else {
				cout << "Nothing to do" << endl;
				int i;
				cin >> i;
			}
		}
		int completedMaps = 0;
		for (int i = 0; i < safeKeeper.size(); ++i) {
			int candidatesSize = getCandidates(safeKeeper[i]->getMap()).size();
			bool levelComplete = !(safeKeeper[i]->hasNotUsedRamps());
			if (candidatesSize == 0 && levelComplete) {
				completedMaps++;
				safeKeeper[i]->setWasMapCompleted(true);
			}
		}
		cout << "COMPLETED MAPS : " << completedMaps << " SAFEKEEPER SIZE : " << safeKeeper.size() << endl;
		if (completedMaps == safeKeeper.size()) {
			roundComplete = true;
			break;
		}


		if (board->hasNotUsedRamps()) {
			cout << "VALID RAMP TO VISIT" << endl;
			vector<Node*> ramps;
			for (Ramp& ramp : board->getRamps()) {
				if (!ramp.getUsed()) {
					ramps.emplace_back(ramp.getOrigin());

				}
			}

			vector <Node*> target = getClosestFrom(ramps, start);
		//	cout << "TARGET SIZE: " << target.size() << endl;
			int minorIndex = 0, minorCost = 10000;
			Path bestPath;

			for (int i = 0; i < target.size(); ++i) {
				Path buffer = getPath(maze, start->getX(), start->getY(), target[i]->getX(), target[i]->getY());
				int cost = buffer.getCost();
				if (cost < minorCost) {
					minorIndex = i;
					minorCost = cost;
					bestPath = buffer;
				}
			}

			bestPath.printPath();
			Ramp* ramp = board->getRampAt(target[minorIndex]);
	//		cout << "RAMPA CHECANDO " << ramp->getOrigin()->getX() << " " << ramp->getOrigin()->getY() << endl;
			ramp->setUsed(true);

			currentLevel = ramp->getFloorTarget();
			bufferFloor = ramp->getEnd();

			safeKeeper[currentLevel]->getMap()[bufferFloor->getY()][bufferFloor->getX()].setWasVisited(true);

			for (int y = 0; y < safeKeeper[currentLevel]->getMap().size(); ++y) {
				for (int x = 0; x < safeKeeper[currentLevel]->getMap()[0].size(); ++x) {
					addNeighbors(&safeKeeper[currentLevel]->getMap()[y][x], safeKeeper[currentLevel]->getMap());
				}
			}

		}
		
		

		if (safeKeeper[currentLevel]->getWasMapCompleted()) {
			vector<Ramp>& targetRamp = safeKeeper[currentLevel]->getRamps();
			int targetRampIndex = 0, lowestLevel = currentLevel;
			for (int i = 0; i < targetRamp.size(); ++i) {
				if (targetRamp[i].getFloorTarget() < currentLevel) {
					lowestLevel = targetRamp[i].getFloorTarget();
					targetRampIndex = i;
				}
			}

			Node* target = targetRamp[targetRampIndex].getOrigin();
			Path returnPath = getPath(safeKeeper[currentLevel]->getMap(), start->getX(), start->getY(), target->getX(), target->getY());
			returnPath.printPath();
			bufferFloor = targetRamp[targetRampIndex].getEnd();
			cout << "STARTX: " << bufferFloor->getX() << " STARTY: " << bufferFloor->getY() << endl;
			currentLevel = targetRamp[targetRampIndex].getFloorTarget();

		}


	

		cout << "END" << endl;
	}
}

void addRampToMap(vector <Board*> safeKeeper, Node* origin, Node* end, int levelOrigin, int levelEnd) {
	Board* boardOrigin = safeKeeper[levelOrigin];
	vector<vector<Node>> & mapOrigin = boardOrigin->getMap();

	Board* boardEnd = safeKeeper[levelEnd];
	vector<vector<Node>> & mapEnd = boardEnd->getMap();


	mapOrigin[origin->getY()][origin->getX()].setIsRamp(true);
	mapEnd[end->getY()][end->getX()].setIsRamp(true);

	
	Ramp rampOrigin(origin, levelEnd);
	rampOrigin.setEnd(end);
	Ramp rampEnd(end, levelOrigin);
	rampEnd.setEnd(origin);

	boardOrigin->addRamp(rampOrigin);
	boardEnd->addRamp(rampEnd);

}

int main() {
	vector<Board*> safeKeeper;
	Board level1, level2, level3, level4;

	vector <vector<Node>> maze1, maze2, maze3;//, maze4;

	int cols1 = 3, rows1 = 3, cols2 = 4, rows2 = 4, cols3 = 3, rows3 = 3; // , cols4 = 2, rows4 = 2;

	for (int y = 0; y < rows1; ++y) {
		maze1.emplace_back();
		for (int x = 0; x < cols1; ++x) {
			maze1[y].emplace_back(Node(x, y));
		}
	}

	for (int y = 0; y < rows2; ++y) {
		maze2.emplace_back();
		for (int x = 0; x < cols2; ++x) {
			maze2[y].emplace_back(Node(x, y));
		}
	}

	for (int y = 0; y < rows3; ++y) {
		maze3.emplace_back();
		for (int x = 0; x < cols3; ++x) {
			maze3[y].emplace_back(Node(x, y));
		}
	}

	//for (int y = 0; y < rows4; ++y) {
	//	maze4.emplace_back();
	//	for (int x = 0; x < cols4; ++x) {
	//		maze4[y].emplace_back(Node(x, y));
	//	}
	//}

	maze1[2][0].setWasVisited(true);

	maze1[2][0].setHasWallRight(true);
	maze1[0][1].setBumpLevel(Small);
	maze1[1][1].setBumpLevel(Max);
	maze1[1][1].setHasWallRight(true);
	maze1[0][2].setHasWallDown(true);

	//maze4[0][1].setHasWallLeft(true);
	//maze4[0][1].setHasWallDown(true);

	maze2[0][0].setHasWallDown(true);
	maze2[2][0].setHasWallRight(true);
	maze2[2][0].setHasWallDown(true);
	maze2[0][1].setHasWallDown(true);
	maze2[1][1].setBumpLevel(Medium);
	maze2[1][1].setHasWallLeft(true);
	maze2[2][1].setHasWallDown(true);
	maze2[0][2].setHasWallRight(true);
	maze2[1][2].setHasWallDown(true);
	maze2[2][2].setBumpLevel(Small);
	maze2[3][2].setHasWallRight(true);
	maze2[1][3].setBumpLevel(Max);


	maze3[0][0].setBumpLevel(Medium);
	maze3[2][0].setBumpLevel(Medium);
	maze3[0][2].setBumpLevel(Small);
	maze3[1][2].setBumpLevel(Small);
	maze3[1][2].setHasWallLeft(true);
	maze3[2][2].setHasWallLeft(true);
	maze3[1][1].setHasWallUp(true);
	maze3[1][1].setHasWallLeft(true);

	updateWalls(maze1);
	updateWalls(maze2);
	updateWalls(maze3);

	//updateWalls(maze4);

	level1.setMap(maze1);
	level2.setMap(maze2);
	level3.setMap(maze3);
	//level4.setMap(maze4);

	safeKeeper.emplace_back(&level1);
	safeKeeper.emplace_back(&level2);
	safeKeeper.emplace_back(&level3);
	//safeKeeper.emplace_back(&level4);

	addRampToMap(safeKeeper, &maze1[0][2], &maze2[0][0], 0, 1);
	addRampToMap(safeKeeper, &maze1[2][1], &maze3[0][1], 0, 2);
	//addRampToMap(safeKeeper, &maze2[3][2], &maze4[0][1], 1, 3);

	completeSection(safeKeeper);

	while (currentLevel > 0) {
		cout << "-------------------------L E V E L  " << currentLevel << "-------------------------" << endl;
		start = bufferFloor;

		vector<Ramp>& targetRamp = safeKeeper[currentLevel]->getRamps();
		int targetRampIndex = 0, lowestLevel = currentLevel;
		for (int i = 0; i < targetRamp.size(); ++i) {
			if (targetRamp[i].getFloorTarget() < currentLevel) {
				lowestLevel = targetRamp[i].getFloorTarget();
				targetRampIndex = i;
			}
		}

		Node* target = targetRamp[targetRampIndex].getOrigin();
		Path returnPath = getPath(safeKeeper[currentLevel]->getMap(), start->getX(), start->getY(), target->getX(), target->getY());
		returnPath.printPath();
		start = targetRamp[targetRampIndex].getEnd();
		currentLevel--;
	}
	cout << "-------------------------L E V E L  " << currentLevel << "-------------------------" << endl;
	
	Path returnPath = getPath(safeKeeper[currentLevel]->getMap(), bufferFloor->getX(), bufferFloor->getY(), home->getX(), home->getY());
	returnPath.printPath();

	int i;
	cout << "AFUERA" << endl;
	cin >> i;
	return 0;
}
