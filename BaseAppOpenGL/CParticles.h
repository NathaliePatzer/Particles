#pragma once


#include <glew.h>
#include "GLTexture.h"
#include <glm/glm.hpp>
#include <vector>


class CParticles
{

public:
	CParticles(float x, float y, float z,
		float speedx, float speedy, float speedz,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a,
		float size, float life);

	~CParticles(void);

	glm::vec3 pos, speed;
	unsigned char r, g, b, a;
	float size, life;
};

class CParticleSystem 
{
public:
	CParticleSystem();
	~CParticleSystem(void);

	void Init(int maxParticles, int particlesPerSecond);
	void Update(float deltaTime);
	void Render();
	void KillParticles(int index);
	void KillAllParticles();
	void AddParticle(float x, float y, float z,
		float speedx, float speedy, float speedz,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a,
		float size, float life);
	void CreateRandomParticle();

private:
	int maxParticles;
	std::vector<CParticles*> ParticlesContainer;
};