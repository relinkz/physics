#include "Parser.h"

Parser::Parser()
{
	this->readFile();
}

Parser::~Parser()
{

}

void Parser::readFile()
{
	stringstream errorMessage;
	stringstream dataStream;
	string data = "";

	float vertex[3];
	float UVcoord[2];
	float vertexNormals[3];
	int crap;

	ifstream file;
	file.open("sphere1.txt");
	//file.open("obj.txt");


	if (file.is_open() == false)
	{
		errorMessage << "Could not find the destination" << endl;
	}
	else
	{
		while (file.eof() == false)
		{
			file >> data;
			//handle the verticies
			if (data == "v")
			{
				file >> vertex[0];
				file >> vertex[1];
				file >> vertex[2];

				this->PositionData.push_back(Vector3(vertex[0], vertex[1], vertex[2]));
			}
			else if (data == "vt")
			{
				file >> UVcoord[0];
				file >> UVcoord[1];

				this->UVData.push_back(Vector2(UVcoord[0], UVcoord[1]));
			}
			//handle the normals
			else if (data == "vn")
			{
				file >> vertexNormals[0];
				file >> vertexNormals[1];
				file >> vertexNormals[2];

				this->NormalData.push_back(Vector3(vertexNormals[0], vertexNormals[1], vertexNormals[2]));
			}
			////the geometry
			//else if (data == "g")
			//{
			//	//if needed
			//	file >> this->geometry;
			//}
			////Type of OBJ description
			//else if (data == "usemtl")
			//{
			//	//if needed
			//	file >> this->usemtl;
			//}
			////getting the material library
			//else if (data == "mtllib")
			//{
			//	file >> this->mtllib;
			//}
			////faces
			else if (data == "f")
			{
				int3 face;
				Vertex2 rawData;

				Vector3* temp3;
				Vector2* temp2;

				for (int i = 0; i < 3; i++)
				{
					file >> data;
					face = this->stringToInt3(data);

					//index for pos
					temp3 = &(this->PositionData.at(face.i1 - 1));
					rawData.Pos = DirectX::XMFLOAT3(temp3->x, temp3->y, temp3->z);

					//index for UV
					temp2 = &(this->UVData.at(face.i2 - 1));
					rawData.UVs = DirectX::XMFLOAT2(temp2->x, temp2->y);

					//index for Normal
					temp3 = &(this->NormalData.at(face.i3 - 1));
					rawData.Normal = DirectX::XMFLOAT3(temp3->x, temp3->y, temp3->z);

					this->finalData.push_back(rawData);
				}
			}

		}
	}
	file.close();
}

int3 Parser::stringToInt3(string src)
{
	int3 face;
	int s_size = src.size();
	int cutter = 0;
	string stringLeft = "";
	string source = src;

	stringLeft = src;

	for (int i = 0; i < 3; i++)
	{
		//search string for first '/', save pos;
		cutter = source.find('/');

		//if '/' exists in string
		if (cutter != -1)
		{
			//cutstring into two
			stringLeft = source.substr(0, cutter);
			source.erase(0, cutter + 1);
		}
		else
		{
			stringLeft = source;
		}

		//convert string to int in face
		switch (i)
		{
		case 0:
			face.i1 = stoi(stringLeft);
			break;
		case 1:
			face.i2 = stoi(stringLeft);
			break;
		case 2:
			face.i3 = stoi(stringLeft);
			break;
		}
	}

	return face;
}

std::vector<Vertex2>& Parser::getRawData()
{
	return this->finalData;
}
