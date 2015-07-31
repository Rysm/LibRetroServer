#pragma once
#include <memory>

namespace CR
{
	namespace LibRetroServer
	{
		struct ICoreProcess
		{
			virtual ~ICoreProcess() = default;
		};

		std::unique_ptr<ICoreProcess> CreateCoreProcess();
	}
}