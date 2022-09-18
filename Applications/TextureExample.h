#pragma once
#include "..\Interfaces/IApplication.h"
class TextureExample : public IApplication
{
public:
	void PreLoopInit() override;
	void Draw() override;
	void EventLoop() override;
	void Tick() override;
};

