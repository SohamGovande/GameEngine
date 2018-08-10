#include "MaterialModel.h"
#include "Comparator.h"

#define Compare(fieldName) if (auto result = Comparators::compareValues(fieldName, other.fieldName)) return result

MaterialModel::MaterialModel(TextureResource& texture, const std::string& name)
	: texture(texture), objName(name), properties()
{
}

RenderableMaterialModel::RenderableMaterialModel(GlModel&& model, TextureResource& texture, const std::string& name)
	: MaterialModel(texture, name), model(std::move(model))
{
}

MaterialModelProperties::MaterialModelProperties()
	: specularMap(nullptr), normalMap(nullptr), parallaxMap(nullptr),
	fullyRender(false), shineDistanceDamper(0), reflectivity(0), parallaxMapAmplitude(0)
{
}

bool MaterialModelProperties::operator==(const MaterialModelProperties& other) const
{
	return specularMap == other.specularMap 
		&& fullyRender == other.fullyRender
		&& shineDistanceDamper == other.shineDistanceDamper
		&& reflectivity == other.reflectivity
		&& normalMap == other.normalMap
		&& parallaxMap == other.parallaxMap
		&& parallaxMapAmplitude == other.parallaxMapAmplitude;
}

void MaterialModelProperties::loadAllTextures()
{
	if (hasSpecularMap())
		specularMap->load();
	if (hasNormalMap())
		normalMap->load();
	if (hasParallaxMap())
		parallaxMap->load();
}

void MaterialModel::loadAllTextures()
{
	texture.load();
	properties.loadAllTextures();
}

int MaterialModelProperties::compare(const MaterialModelProperties& other) const
{
	Compare(hasParallaxMap());
	if (hasParallaxMap())
	{
		Compare(parallaxMap->getFilepath());
		Compare(parallaxMapAmplitude);
	}

	Compare(hasNormalMap());
	if (hasNormalMap())
		Compare(normalMap->getFilepath());

	Compare(fullyRender);

	Compare(hasSpecularMap());
	if (hasSpecularMap())
		Compare(specularMap->getFilepath());

	Compare(reflectivity);
	Compare(shineDistanceDamper);

	return 0; //The material properties are the same
}

int MaterialModel::compare(const MaterialModel& other) const
{
	Compare(texture.getFilepath());
	if (int propertiesComparison = properties.compare(other.properties)) return propertiesComparison;

	return 0; //The two models are the same
}

int RenderableMaterialModel::compareAgainstRenderable(const RenderableMaterialModel& other) const
{
	Compare(model.vao.getVaoID());
	return this->compare((const MaterialModel&)other);
}

bool MaterialModel::operator==(const MaterialModel & model) const
{
	return model.objName == objName &&
		model.texture.getFilepath() == texture.getFilepath() &&
		model.properties == properties;
}

size_t std::hash<MaterialModelProperties>::operator()(const MaterialModelProperties& obj) const
{
	size_t i = hash<bool>()(obj.fullyRender) ^
		hash<float>()(obj.shineDistanceDamper) ^
		hash<float>()(obj.reflectivity) ^
		hash<float>()(obj.parallaxMapAmplitude);

	if (obj.hasSpecularMap())
		i ^= hash<std::string>()(obj.specularMap->getFilepath());
	if (obj.hasNormalMap())
		i ^= hash<std::string>()(obj.normalMap->getFilepath());
	if (obj.hasParallaxMap())
		i ^= hash<std::string>()(obj.parallaxMap->getFilepath());
	return i;
}

std::size_t std::hash<MaterialModel>::operator()(const MaterialModel& model) const
{
	std::size_t h = std::hash<std::string>()(model.getObjName()) ^
		std::hash<std::string>()(model.getTexture().getFilepath()) ^
		std::hash<MaterialModelProperties>()(model.properties);

	return h;
}