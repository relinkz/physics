#ifndef PARSER_H
#define PARSER_H
#include "GlobalData.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

class Parser
{
public:
	Parser();
	virtual ~Parser();

	std::vector<Vertex2>& getRawData();
private:
	std::vector<Vector3>PositionData;
	std::vector<Vector3>NormalData;
	std::vector<Vector2>UVData;

	std::vector<Vertex2>finalData;

	void readFile();
	int3 stringToInt3(string src);
};
#endif