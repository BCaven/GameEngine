#pragma once
#include "SceneGraph.h"
#include "cpuBVH.h"

class SceneGraphBVH_cpu :
    public SceneGraph
{
    cpuBVH bvh;

public:
    SceneGraphBVH_cpu() : SceneGraph() 
    {
        activeCamera = nullptr;
    }

    SceneGraphBVH_cpu(std::shared_ptr<Camera> ActiveCam) : SceneGraph() 
    {
        activeCamera = ActiveCam;
    };
    ~SceneGraphBVH_cpu() {};

    void setActiveCamera(std::shared_ptr<Camera> activeCam) { activeCamera = activeCam; }

    void AddObjectToSceneGraph(std::shared_ptr<GameObject> newObj) override;
    std::vector<std::shared_ptr<GameObject>> GetVisibleObjects() override;
};

