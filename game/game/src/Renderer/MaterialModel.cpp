#include "MaterialModel.h"

MaterialModel::MaterialModel(TextureResource* texture, const GlModel& model, const std::string& name)
	: model(model), texture(texture),
	objName(name),
	hasSpecularMap(false),
	fullyRender(false),
	shineDistanceDamper(0), reflectivity(0)
{
}
