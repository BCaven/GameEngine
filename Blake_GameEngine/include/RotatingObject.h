#pragma once
#include "GameObject.h"
class RotatingObject :
    public GameObject
{
    glm::vec3 rotation_axis;
    float rotation_speed;
    Quaternion rotation_tick;

public:
    // this class is just a quaternion test

    RotatingObject(std::shared_ptr<Shape> instancedShape, glm::vec3 rot_axis, glm::vec3 pos) : RotatingObject(instancedShape, rot_axis, 1, pos, glm::vec3(1), glm::vec3(0)) {}

    RotatingObject(std::shared_ptr<Shape> instancedShape, glm::vec3 rot_axis = glm::vec3(0, 1, 0), float rot_speed = 1, glm::vec3 pos = glm::vec3(0), glm::vec3 s = glm::vec3(1), glm::vec3 rot = glm::vec3(0)) : GameObject(instancedShape, pos, s, rot), rotation_axis(rot_axis), rotation_speed(rot_speed)
    {
        rotation_tick = Quaternion::createFromAxisAngle(rotation_axis, rotation_speed);
        bCanTick = true;
    }

    void Tick(float deltaTime) override;


};

