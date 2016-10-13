#include "Parser.h"

Parser::Parser()
{
	this->targaData = nullptr;
	//this->readFile();
}

Parser::~Parser()
{
	if (this->targaData != nullptr)
	{
		delete this->targaData;
		this->targaData = nullptr;
	}
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

void Parser::ReverseTriangle()
{
	int nrOfVertexes = this->finalData.size();
	Vertex2 temp;
	for (int i = 0; i < nrOfVertexes; i += 3)
	{
		temp = this->finalData.at(i + 1);
		this->finalData.at(i + 1) = this->finalData.at(i + 2);
		this->finalData.at(i + 2) = temp;
	}
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

ID3D11ShaderResourceView * Parser::LoadTarga(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename)
{

	HRESULT hresult;

	ID3D11Texture2D *texture = nullptr;
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;
	int height = 0;
	int width= 0;

	//Open the targa file for reading in binary
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) {
		//return false;
	}

	//Read in the file header
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) {
		//return false;
	}

	//Get the important information from the header
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	//Check that it is 32bit and not 24bit
	if (bpp != 32) {
		//return false;
	}

	//Calculate the size of the 32 bit image data
	imageSize = width * height * 4;

	//Allocate memory for the targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage) {
		//return false;
	}

	//Read in the targa image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) {
		//return false;
	}

	//Close the file
	error = fclose(filePtr);
	if (error != 0) {
		//return false;
	}

	//Allocate memory for the targa destination data
	this->targaData = new unsigned char[imageSize];
	if (!this->targaData) {
		//return false;
	}

	//Initialize the index into the targa destination data
	index = 0;

	//Initialize the index into the targa image data
	k = (width * height * 4) - (width * 4);

	//Now copy the targa imaga data into the targa destionation array int he correct order since the targa format is stored upside down
	for (j = 0; j < height; j++) {

		for (i = 0; i < width; i++) {
			this->targaData[index + 0] = targaImage[k + 2];	//Red
			this->targaData[index + 1] = targaImage[k + 1];	//Green
			this->targaData[index + 2] = targaImage[k + 0];	//Blue
			this->targaData[index + 3] = targaImage[k + 3];	//Alpha

															//Increment the indexes into the targa data
			k += 4;
			index += 4;
		}

		//Set the targa image index back to the preceding row at the begining of the coulum sinceits reding it in upside down
		k -= (width * 8);
	}

	int height2 = height / 2;
	int currPos = 0;
	int swapPos = 0;
	//turns the image to the right side upp
	for (int y = 0; y < height2; y++)
	{
		int q;
		for (int x = 0; x < width; x++)
		{
			currPos = (x * 4) + (width * 4 * y);
			swapPos = (width * 4 * height) -(width * 4 * y)  - (width * 4) + (x * 4);
	
			unsigned char r = this->targaData[swapPos + 0];
			unsigned char g = this->targaData[swapPos + 1];
			unsigned char b = this->targaData[swapPos + 2];
			unsigned char a = this->targaData[swapPos + 3];
			
			unsigned char r2 = this->targaData[currPos + 0];
			unsigned char g2 = this->targaData[currPos + 1];
			unsigned char b2 = this->targaData[currPos + 2];
			unsigned char a2 = this->targaData[currPos + 3];
			
			this->targaData[swapPos + 0] = r2;
			this->targaData[swapPos + 1] = g2;
			this->targaData[swapPos + 2] = b2;
			this->targaData[swapPos + 3] = a2;
			
			this->targaData[currPos + 0] = r;
			this->targaData[currPos + 1] = g;
			this->targaData[currPos + 2] = b;
			this->targaData[currPos + 3] = a;
	
		}
		int z = 0;
	}

	//Release the targa image data now that it was copied into the destination array
	delete[] targaImage;
	targaImage = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1; 
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_R32G32B32A32_FLOAT
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//D3D11_SUBRESOURCE_DATA data;
	//memset(&data, 0, sizeof(data));
	//data.SysMemPitch = NULL;
	//data.SysMemPitch = (width * 4) * sizeof(unsigned char);

	hresult = device->CreateTexture2D(&textureDesc, NULL, &texture);

	int rowPitch = (width * 4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(texture, 0, NULL, this->targaData, rowPitch, 0);

	ID3D11ShaderResourceView *tempTextureView = nullptr;

	hresult = device->CreateShaderResourceView(texture, &srvDesc, &tempTextureView);

	delete[] this->targaData;
	this->targaData = nullptr;

	return tempTextureView;
}
