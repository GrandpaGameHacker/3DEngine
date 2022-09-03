#pragma once
#include "..\Interfaces/IApplication.h"
class TriangleExample : public IApplication
{
public:
	void Init();
	virtual void Loop() override;
};

