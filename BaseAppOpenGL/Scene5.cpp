#include "Scene5.h"

CScene5::CScene5()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	//pTextures = new CTexture();	

	fAngle = 0.0f;

	// Enemy (Lâmpada) Initial Position
	enemyPos = glm::vec3(0.0f, 0.0f, 0.0f);
	playerPos = glm::vec3(pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);
}


CScene5::~CScene5(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}	
}




int CScene5::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter
	
	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Ângulo em Radianos 0 a 6.28 (360 Graus)
	fAngle += 0.1f;
	if ((fAngle >= 6.28f) || (fAngle <= -6.28f))
		fAngle = 0.0f;



	// Objeto com translação
	float mTranslate[16] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
	   -5, 5, 0, 1
	};

	glColor3ub(255, 255, 255);
	glPushMatrix();
	glMultMatrixf(mTranslate);
	glutSolidCube(1);
	glPopMatrix();


	// Objeto com Rotação em X
	float mRotateX[16] =
	{
		1,			 0,			   0, 0,
		0,  cos(fAngle), sin(fAngle), 0,
		0, -sin(fAngle), cos(fAngle), 0,
		0,            0,           0, 1
	};

	glColor3ub(255, 0, 0);
	glPushMatrix();
	glTranslatef(-5.0f, 0.0f, 0.0f);
	glMultMatrixf(mRotateX);
	glutSolidCube(1);
	glPopMatrix();


	// Objeto com Rotação em Y
	float mRotateY[16] =
	{
		cos(fAngle),  0, -sin(fAngle), 0,
				  0,  1,            0, 0,
		sin(fAngle),  0,  cos(fAngle), 0,
				  0,  0,            0, 1
	};

	glColor3ub(0, 255, 0);
	glPushMatrix();
	glTranslatef(0.0f, 5.0f, 0.0f);
	glMultMatrixf(mRotateY);
	glutSolidCube(1);
	glPopMatrix();


	// Objeto com Rotação em Z
	float mRotateZ[16] =
	{
		cos(fAngle), sin(fAngle), 0, 0,
	   -sin(fAngle), cos(fAngle), 0, 0,
				  0,           0, 1, 0,
				  0,           0, 0, 1
	};

	glColor3ub(0, 0, 255);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 5.0f);
	glMultMatrixf(mRotateZ);
	glutSolidCube(1);
	glPopMatrix();



	// Objeto com Escala
	float mScale[16] =
	{
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 2, 0,
		0, 0, 0, 1
	};

	glColor3ub(255, 255, 0);
	glPushMatrix();
	glTranslatef(5.0f, 0.0f, 0.0f);
	glMultMatrixf(mScale);
	glutSolidCube(1);
	glPopMatrix();



	///////////////////////////////////////////////
	// Objeto com translação, rotação e escala
	float mTranslate1[16] =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
	   0, 15, 0, 1
	};

	// Desenha um torus
	glColor3ub(174, 174, 235);
	glPushMatrix();
	glMultMatrixf(mTranslate1);
	glMultMatrixf(mRotateY);
	glMultMatrixf(mScale);
	glutSolidTorus(1, 3, 10, 10);
	glPopMatrix();



	// Atualiza a Player Position
	playerPos = glm::vec3(pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	// Acha o vetor de direção entre o Player <--- Enemy através da subtração de suas
	// respectivas posições
	glm::vec3 dirVectorPE(playerPos - enemyPos);

	// Normaliza o vetor de direção
	//glm::normalize(dirVectorPE);		
	
	double magnitude =  sqrt(
		pow(dirVectorPE.x, 2) +
		pow(dirVectorPE.y, 2) +
		pow(dirVectorPE.z, 2));

	glm::vec3 normaldirVectorPE;
	normaldirVectorPE.x = (dirVectorPE.x / magnitude);
	normaldirVectorPE.y = (dirVectorPE.y / magnitude);
	normaldirVectorPE.z = (dirVectorPE.z / magnitude);
	

	// Incrementa a nova posição do Enemy (Lâmpada) baseada no vetor de direção
	enemyPos += (normaldirVectorPE * 0.1f);

	// Calcula  a distância entre o Player e o Enemy (Lâmpada)
	double distance = sqrt(
		pow(enemyPos.x - playerPos.x, 2) +
		pow(enemyPos.y - playerPos.y, 2) +
		pow(enemyPos.z - playerPos.z, 2));


	if (distance >= 0.5)
		glColor3ub(255, 255, 255);
	else
		glColor3ub(255, 0, 0);

	// Desenhar a Lâmpada
	glPushMatrix();
	glTranslatef(enemyPos.x, enemyPos.y, enemyPos.z);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();


	



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));

	//// Imprime a distância entre o Inimigo e o Player
	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("Distance (Enemy -> Player): %.1f", distance);

	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene5::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene5::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}



}

void CScene5::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene5::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene5::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


