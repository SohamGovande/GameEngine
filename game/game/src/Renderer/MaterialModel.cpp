#include "MaterialModel.h"

MaterialModel::MaterialModel(TextureResource& texture, const std::string& name)
	: texture(texture), objName(name), properties()
{
}

RenderableMaterialModel::RenderableMaterialModel(GlModel&& model, TextureResource& texture, const std::string& name)
	: MaterialModel(texture, name), model(std::move(model))
{
}