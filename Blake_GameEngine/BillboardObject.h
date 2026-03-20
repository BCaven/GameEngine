#pragma once
#include "GameObject.h"
#include <memory>

class BillboardObject :
    public GameObject
{
    std::shared_ptr<GameObject> Parent;

    public:
    BillboardObject();
    ~BillboardObject();
    BillboardObject(std::string filePath);
	void Tick(float deltaTime) override;

	void SetParent(std::shared_ptr<GameObject> parent) { Parent = parent; }
};

