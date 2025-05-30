#ifndef __TERRENO_H__
#define __TERRENO_H__



#include <windows.h>	// Header File For Windows
#include <fstream>

#include <glew.h>
#include <gl.h>										// Header File For The OpenGL32 Library
#include <glut.h>
#include <glext.h>
#include <glm/glm.hpp>
#include <vector>

using namespace std;

#define MAP_SIZE		1025	// Este � o tamanho do arquivo .RAW (tamanho em pixels)
#define STEP_SIZE		5.0f	// Esta � a largura e altura de cada QUAD (quadrado do terreno) (quantos pixels ele vai pegar para formar cada quadrado na renderiz���o)
#define	HEIGHT_RATIO	1.0f    // Altura m�xima que o Y pode alcan�ar (1 = 255) (2 = 510)

class CTerreno
{
public:
	CTerreno();
	~CTerreno();

	

	// Fun��o que retorna a altura (0 a 255) para o heightmap pegando X e Y
	int Height(int X, int Y);

	// Seta uma cor para cada index em particular, dependendo da altura do index
	void SetVertexColor(int x, int y);

	// Fun��o que carrega arquivo .RAW com o tamanho certo do arquivo
	void LoadRawFile(LPSTR strName);

	// Fun��o que p�e os dados do heightmap nas primitivas e desenha na tela
	void RenderHeightMap(bool isTexTile);
	void RenderHeightMapMultiTex(bool isTexTile);
	void RenderHeightMapVA();

	void CreateHeightMapDisplayList(bool isTexTile = false, bool isMultiTex = false);
	void RenderHeightMapDisplayList();


	glm::vec3 CalculateQuadNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
	glm::vec3 CalculateTriangleNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);


private:
	BYTE pHeightMap[MAP_SIZE * MAP_SIZE];
	GLuint displayListId;

	vector<glm::vec3> vVertexData;
	vector<glm::vec2> vTexCoordData;
	vector<glm::vec3> vNormalData;

	bool isTexTile;
	unsigned int iNumFaces;
};


#endif
