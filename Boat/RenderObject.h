#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"	

#include "AssetManager.h"
#include "Utilities.h"
#include "Camera.h"
using namespace Utilities;

class RenderObject
{
public:
	RenderObject();
	RenderObject(string, string, string); //ok for now
	RenderObject(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up); //ok for now
	virtual ~RenderObject();

	virtual int update(long elapsedTime);
	virtual int render(long totalElapsedTime);
	virtual int shaderlessRender(long totalElapsedTime, bool useOwnTexture = false);
	virtual int updateMatrices();

	virtual void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }

	static void setStatics(glm::mat4 * view, glm::mat4 * proj, AssetManager * ass) 
		{ viewMatrix = view; projMatrix = proj; assetManager = ass; }

protected:
	string modelPath;
	string texturePath;
	string shaderPath;

	Model * getModel(string path) { return assetManager->getModel(path); }
	Texture * getTexture(string path) { return assetManager->getTexture(path); }
	Shader * getShader(string path) { return assetManager->getShader(path); }
	
	Model * model;
	Texture * texture;
	Shader * shader;

	bool lighting;

	glm::mat4 worldMatrix, normalMatrix;
	glm::vec3 position, direction, up;
	
	#define activeCamera Camera::getActiveCamera()
	#define activeShader Shader::getActiveShader()

	static glm::mat4 * viewMatrix;
	static glm::mat4 * projMatrix;
private:
	void init();

	static AssetManager * assetManager;
};

#endif