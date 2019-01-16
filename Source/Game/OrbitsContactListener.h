#include <Box2D/Box2D.h>
#include "Game/entities/GameObject.h"

class OrbitsContactListener : public b2ContactListener
{
public:
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};