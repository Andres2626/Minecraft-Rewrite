#pragma once
#include "common.h"

/* simple error handler */

namespace MC 
{
	enum class ErrorType : u32t
	{
		NoError = 0,
		NoImplemented,
		NoMemory,
		WindowLibrary,
		WindowBuild,
		ContextInit,
		AssetLoad = 0x100,
		FileNotFound,
		ShaderCompileError,
		ShaderRegisterError
	};

#if 0
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
#endif /* deprecated */

}