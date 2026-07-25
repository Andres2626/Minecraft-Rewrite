#include "Layers/Default.h"

namespace MC 
{
	namespace Layers 
	{

		Default::Default()
		{
			SetVisible(true);
		}

		Default::~Default()
		{

		}

		bool Default::Init()
		{
			return true;
		}

		void Default::OnUpdate(Utils::Timestep &ts)
		{

		}

		void Default::OnRender(float alpha)
		{

		}

		void Default::OnTick()
		{

		}

		void Default::OnSuspended()
		{

		}
	}
}