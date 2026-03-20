#include "BillboardObject.h"

BillboardObject::BillboardObject() : GameObject()
{
}

BillboardObject::BillboardObject(std::string filePath) : GameObject(filePath)
{
}

BillboardObject::~BillboardObject()
{
}

void BillboardObject::Tick(float deltaTime)
{
	// rotate to look at parent
	glm::vec3 parentPos = Parent->GetPosition();
	glm::vec3 toParent = parentPos - GetPosition();

	// TODO: need to write quaternion first lol
}