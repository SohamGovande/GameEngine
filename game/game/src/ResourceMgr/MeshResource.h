#pragma once

#include "Resource.h"
#include "Resource.t.h"
#include "Mesh.h"

class MeshResource
	: public Resource<Mesh>
{
private:
	std::string objFile;
	std::string texFile;

protected:
	void generate() override;

public:
	MeshResource(const std::string& objFile, const std::string& texFile);
	~MeshResource();

	static void free(void* _thisPtr);

	inline const std::string& getFilepath() const { return objFile; }
};

