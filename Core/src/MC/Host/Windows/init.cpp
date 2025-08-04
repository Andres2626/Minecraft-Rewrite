#include "..\init.h"
#include "..\console.h"

#include <string.h>
#include <locale>

namespace MC 
{

	void Host::Init(int* argc, char*** argv) 
	{
		Console::Init();
		setlocale(LC_ALL, "");
	}

	void Host::End()
	{

	}

}