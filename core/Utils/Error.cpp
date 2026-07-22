#include "Utils/Error.h"

namespace MC 
{
	ErrorHandler::ErrorHandler()
	{
		err.num = ErrorType::NoError;
		err.str = "";
	}

	ErrorHandler::~ErrorHandler()
	{

	}

	void ErrorHandler::SetError(const ErrorType &n)
	{
		err.num = n;
		
		switch (n)
		{
		case ErrorType::WindowLibrary:
			err.str = "Window library error";
			break;
		case ErrorType::WindowBuild:
			err.str = "Window build error";
			break;
		case ErrorType::ContextInit:
			err.str = "Context init error";
			break;
		case ErrorType::AssetLoad:
			err.str = "Resource loading error";
			break;
		case ErrorType::Noimp:
			err.str = "Not implemented";
			break;
		case ErrorType::FileNotFound:
			err.str = "File not found error";
			break;
		case ErrorType::ShaderCompileError:
			err.str = "Shader compile error";
			break;
		default:
			break;
		}
	}

	Error &ErrorHandler::GetError()
	{
		return err;
	}
}


