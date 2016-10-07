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

	ID3D11ShaderResourceView * LoadTarga(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename);
	void readFile();
private:
	std::vector<Vector3>PositionData;
	std::vector<Vector3>NormalData;
	std::vector<Vector2>UVData;

	std::vector<Vertex2>finalData;

	unsigned char* targaData;

	int3 stringToInt3(string src);
};
#endif