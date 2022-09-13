#pragma once
#include "..\Interfaces/IApplication.h"
class ExampleTwo : public IApplication
{
public:
	void PreLoopInit();
	void Draw() override;
	void EventLoop() override;
	void Tick() override;
};

