#include <sstream>
#include <fstream>
#include "ResourceMgr.h"
#include "IOHelper.h"
#include "TimedScope.h"
#include "PropertySetter.h"
#include "PropertyRegisterHelper.h"

#define RegisterProperty(fieldName) \
{\
	constexpr unsigned int fieldNameLength = Length(#fieldName);\
	const char* propertyName = &FormatName<fieldNameLength, CountUppercase(#fieldName, fieldNameLength)>(#fieldName)[0];\
	using FieldType = decltype(std::declval<MaterialModel>().properties.fieldName);\
	materialProperties.emplace(propertyName, MaterialProperty(\
		offsetof(MaterialModelProperties, fieldName),\
		sizeof(FieldType),\
		GetMaterialPropertyType<FieldType>())\
	);\
}

ResourceMgr::ResourceMgr()
{
	initMaterialProperties();
	loadResources();
}

ResourceMgr::~ResourceMgr()
{
	for (auto it = textures.begin(); it != textures.end(); it++)
		it->second.free();

	for (auto it = models.begin(); it != models.end(); it++)
		it->second.free();
}

TextureResource& ResourceMgr::loadTex(const std::string& name, const std::string& texture)
{
	return textures.emplace(name, TextureResource(texture)).first->second;
}

void ResourceMgr::initMaterialProperties()
{
	RegisterProperty(reflectivity);
	RegisterProperty(shineDistanceDamper);
	RegisterProperty(fullyRender);
	RegisterProperty(specularMap);
	RegisterProperty(normalMap);
	RegisterProperty(parallaxMap);
	RegisterProperty(parallaxMapAmplitude);
}

void ResourceMgr::loadTexturesFile()
{
	rapidjson::Document document;
	std::string texturesJson = IOHelper::readFile("res/registries/textures.json");
	document.Parse(&texturesJson.front());
	for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); it++)
	{
		const char* name = it->name.GetString();
		const char* filepath = it->value.GetString();
		textures.emplace(name, filepath);
	}
}

TextureResource& ResourceMgr::parseJsonTexture(const rapidjson::Value& value)
{
	if (value.IsObject())
	{
		return texture(value.GetObject()["registry_name"].GetString());
	}
	else /*if (texIt->value.IsString())*/
	{
		std::string texName = value.GetString();
		texName.append("_mdl_tex");

		return textures.emplace(&texName.front(), value.GetString()).first->second;
	}
}

void ResourceMgr::loadModelsFile()
{
	rapidjson::Document document;
	std::string modelsJson = IOHelper::readFile("res/registries/models.json");
	document.Parse(&modelsJson.front());
	for (const auto& prop : document.GetObject())
	{
		const char* modelName = prop.name.GetString();
		rapidjson::Value::ConstObject obj = prop.value.GetObject();

		TextureResource& modelTexture = parseJsonTexture(obj.FindMember("texture")->value);
		const char* modelFilename = obj["model"].GetString();
		
		ModelResource& resource = models.emplace(modelName, ModelResource(modelFilename, modelTexture))
			.first->second;

		rapidjson::Value::ConstMemberIterator propertiesIt = obj.FindMember("properties");

		if (propertiesIt != obj.MemberEnd())
		{
			for (const auto& prop : propertiesIt->value.GetObject())
			{
				const MaterialProperty& property = materialProperties.find(prop.name.GetString())->second;
				PropertySetterValue propertyValue;
				MaterialPropertyType expectedType = property.getType();
				
				if (expectedType == MaterialPropertyType::INTEGER)
					propertyValue.intVal = prop.value.GetInt();

				else if (expectedType == MaterialPropertyType::UNSIGNED_INTEGER)
					propertyValue.uintVal = prop.value.GetUint();

				else if (expectedType == MaterialPropertyType::FLOATING_DECIMAL)
					propertyValue.floatVal = prop.value.GetFloat();

				else if (expectedType == MaterialPropertyType::SIMPLE_BOOLEAN)
					propertyValue.boolVal = prop.value.GetBool();

				else if (expectedType == MaterialPropertyType::TEXTURE_RESOURCE)
					propertyValue.textureVal = &parseJsonTexture(prop.value);

				resource.addPropertySetter(property, propertyValue);
			}
		}
	}
}

void ResourceMgr::loadResources()
{
	TimedScope timer("load resources");

	loadTexturesFile();
	loadModelsFile();
}
