#include "BillboardObject.h"

BillboardObject::BillboardObject() : GameObject()
{

}

BillboardObject::BillboardObject(std::shared_ptr<GameObject> target) : GameObject()
{
	bCanTick = true;
}

BillboardObject::BillboardObject(std::string filePath) : GameObject(filePath)
{
	bCanTick = true;
}

BillboardObject::~BillboardObject()
{
}

void BillboardObject::Tick(float deltaTime)
{
	if (!Parent)
	{
		logger->warn("No parent selected for the billboard object");
		return;
	}
	// rotate to look at parent
	glm::vec3 parentPos = Parent->GetPosition();
	if (parentPos == position)
	{
		return;
	}
	rotation = Quaternion::lookAt(position, parentPos);	
	//logger->info("Rotation: {} + {}i + {}j + {}k", rotation.w, rotation.x, rotation.y, rotation.z);
}