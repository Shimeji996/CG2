#pragma once
#include "Engine.h"

class TextureManager
{
public:

	void Initialize(MyEngine* engine);

	void Finalize();

private:

	MyEngine* engine_;

};

