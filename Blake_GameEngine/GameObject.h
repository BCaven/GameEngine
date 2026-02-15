#pragma once
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Quaternion.h"

class GameObject
{
	bool bCanTick;
	bool bDynamic;
	bool bVisible;
	glm::vec3 position;
	Quaternion rotation;
	int renderElement;
	glm::vec3 linearV; // TBD: make a subclass that can move and have the base class unable to move.
	glm::vec3 angularV;
	std::shared_ptr<spdlog::logger> logger;
	

public:
	GameObject()
		: bCanTick(false), bDynamic(false), bVisible(true)
	{
		logger = spdlog::get("shape");
		if (!logger)
		{
			logger = spdlog::stdout_color_mt("shape");
		}
	}
	glm::mat4 getModel();
	int getRenderElement() { return renderElement; }

	virtual void Tick(float deltaTime);
	inline void SetCanTick(bool canTick) { bCanTick = canTick; }
	inline bool CanTick() const { return bCanTick; }
	inline void SetDynamic(bool isDynamic) { bDynamic = isDynamic; }
	inline bool IsDynamic() const { return bDynamic; }
	inline void SetVisible(bool isVisible) { bVisible = isVisible; }
	inline bool IsVisible() const { return bVisible; }


};

