#pragma once
#include "..\Interfaces/IApplication.h"
class TriangleExampleTwo : public IApplication
{
public:
	void Init();
	virtual void Loop() override;
};

