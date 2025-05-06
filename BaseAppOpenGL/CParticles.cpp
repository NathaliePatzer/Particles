#include "CParticles.h"

CParticles::CParticles(float x, float y, float z,
	float speedx, float speedy, float speedz,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a,
	float size, float life) 
{
	this->pos = glm::vec3(x, y, z);
	this->speed = glm::vec3(speedx, speedy, speedz);
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	this->size = size;
	this->life = life;
}


CParticles::~CParticles(void)
{

}

CParticleSystem::CParticleSystem() 
{
	maxParticles = 0;
}

CParticleSystem::~CParticleSystem(void) 
{
	KillAllParticles();
}

void CParticleSystem::Init(int maxParticles, int particlesPerSecond) 
{
	this->maxParticles = maxParticles;
	//reservar na memória o espaço para as partículas
	ParticlesContainer.reserve(maxParticles);

	//alocar as partículas no vector
	for (int i = 0; i < maxParticles; i++) {
		CreateRandomParticle();
	}
}

void CParticleSystem::Update(float deltaTime)
{
	for (int i = 0; i < ParticlesContainer.size(); i++) {
		CParticles* particle = ParticlesContainer[i];

		//atualiza a posição da partícula
		particle->pos += particle->speed * 0.5f;

		//atualiza a vida da partícula
		particle->life -= 0.01f;

		if (particle->life <= 0.0f) {
			delete particle;
			ParticlesContainer.erase(ParticlesContainer.begin() + i);
			i--;
			CreateRandomParticle();
		}
	}
}

void CParticleSystem::Render() 
{
	for (int i = 0; i < ParticlesContainer.size(); i++) {

		CParticles* particle = ParticlesContainer[i];

		glPushMatrix();
		glTranslatef(particle->pos.x, particle->pos.y, particle->pos.z);
		glColor3ub(particle->r, particle->g, particle->b);
		auxSolidBox(particle->size, particle->size, particle->size);
		glPopMatrix();
	}
}


//void KillParticles(int index);

void CParticleSystem::KillAllParticles()
{
	for (int i = 0; i < ParticlesContainer.size(); i++) {

		delete ParticlesContainer[i];
	}
	
	ParticlesContainer.clear();
}

void CParticleSystem::AddParticle(float x, float y, float z,
	float speedx, float speedy, float speedz,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a,
	float size, float life)
{
	if (ParticlesContainer.size() < this->maxParticles) {
		
		CParticles* particle = new CParticles(x, y, z, speedx, speedy, speedz, r, g, b, a, size, life);
		ParticlesContainer.push_back(particle); //adiciona uma nova partícula no vector
	}
}


void CParticleSystem::CreateRandomParticle()
{
	float x = static_cast<float>(rand() % 200) - 100.0f; //partículas que variam de -100 até 100
	float y = static_cast<float>(rand() % 200) + 200.0f; 
	float z = static_cast<float>(rand() % 200) - 100.0f; 
	float speedx = static_cast<float>(rand() % 100) / 100.0f; 
	float speedy = static_cast<float>(-1.0); //partículas vão sempre para baixo
	float speedz = static_cast<float>(rand() % 100) / 100.0f;
	unsigned char r = static_cast<unsigned char>(rand() % 256);
	unsigned char g = static_cast<unsigned char>(rand() % 256);
	unsigned char b = static_cast<unsigned char>(rand() % 256);
	unsigned char a = static_cast<unsigned char>(rand() % 256);
	float size = static_cast<float>(rand() % 1) + 2;
	float life = static_cast<float>(rand() % 10) + 1;

	AddParticle(x, y, z, speedx, speedy, speedz, r, g, b, a, size, life);
}