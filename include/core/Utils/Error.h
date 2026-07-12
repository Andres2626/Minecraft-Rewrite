#pragma once
#include "common.h"

/* simple error handler */

namespace MC 
{
	enum class ErrorType
	{
		NoError = 0,
		WindowLibrary,
		WindowBuild,
		OpenGLInit,
		Noimp,
		AssetLoad = 0x100,
		FileNotFound,
		ShaderCompileError,
		ShaderRegisterError
	};

	struct Error
	{
		ErrorType num;
		mc_str str;
	};

	class MC_API ErrorHandler
	{
	private:
		Error err;
	public:
		ErrorHandler();
		~ErrorHandler();
	public:
		void SetError(const ErrorType& n);
		Error &GetError();
	};
}