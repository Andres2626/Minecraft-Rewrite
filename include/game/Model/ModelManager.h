#pragma once

#include <Model/IModel.h>

class ModelManager
{
private:
	static std::vector<std::unique_ptr<IModel>> m_Models;
public:
	static void Init();
	static void Register(ModelType type, std::unique_ptr<IModel> model);
	static IModel &GetModelType(ModelType type) { return *m_Models[(int)type]; };
};