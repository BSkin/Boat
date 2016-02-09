#ifndef AI_H
#define AI_H

#include "Character.h"

class AI : public Character
{
public:
	AI();
	AI(string, string, string, glm::vec3 position, glm::vec3 direction, glm::vec3 up, string path); //ok for now
	virtual ~AI();
	
	virtual int update(long elapsedTime);
	virtual int render(long totalElapsedTime);
};

#endif