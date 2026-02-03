#pragma once
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class GameObject
{
	bool bCanTick;
	bool bDynamic;
	bool bVisible;
	glm::mat4x4 transform;
	std::shared_ptr<spdlog::logger> logger;
	

public:
	GameObject()
		: bCanTick(true), bDynamic(false), bVisible(true), transform(
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f)
	{
		logger = spdlog::get("shape");
		if (!logger)
		{
			logger = spdlog::stdout_color_mt("shape");
		}
	}
	virtual void Tick(float deltaTime);
	inline void SetCanTick(bool canTick) { bCanTick = canTick; }
	inline bool CanTick() const { return bCanTick; }
	inline void SetDynamic(bool isDynamic) { bDynamic = isDynamic; }
	inline bool IsDynamic() const { return bDynamic; }
	inline void SetVisible(bool isVisible) { bVisible = isVisible; }
	inline bool IsVisible() const { return bVisible; }
	inline void SetTransform(const glm::mat4x4& newTransform) { transform = newTransform; }
	inline const glm::mat4x4& GetTransform() const { return transform; }


};

