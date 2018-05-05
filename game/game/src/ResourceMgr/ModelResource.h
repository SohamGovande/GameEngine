#pragma once

#include "Renderer/MaterialModel.h"
#include "Resource.h"
#include "Resource.t.h"

class ModelResource
	: public Resource<MaterialModel>
{
private:
	std::string modelFile, texFile;

protected:
	void generate() override;

public:
	ModelResource(const std::string& modelFile, const std::string& texFile);
	~ModelResource();

	static void free(void* thisPtr);
};
