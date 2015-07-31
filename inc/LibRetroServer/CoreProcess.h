#pragma once
#include <memory>
#include <chrono>

namespace CR
{
	namespace LibRetroServer
	{
		struct ICoreProcess
		{
			virtual ~ICoreProcess() = default;
			//Returns false if timed out waiting for process to close, or if some other error occured
			virtual bool WaitForClose(const std::chrono::milliseconds& a_maxWait) = 0;
		};

		std::unique_ptr<ICoreProcess> CreateCoreProcess();
	}
}