#include "CoreProcess.h"
#include <Platform\Process.h>

namespace CR
{
	namespace LibRetroServer
	{
		class CoreProcess : public ICoreProcess
		{
		public:
			CoreProcess(std::unique_ptr<CR::Platform::IProcess> a_process);
			virtual ~CoreProcess() = default;
		private:
			std::unique_ptr<CR::Platform::IProcess> m_process;
		};
	}
}

using namespace CR::LibRetroServer;

CoreProcess::CoreProcess(std::unique_ptr<CR::Platform::IProcess> a_process) :
	m_process(std::move(a_process))
{

}

std::unique_ptr<ICoreProcess> CR::LibRetroServer::CreateCoreProcess()
{
	auto process = CR::Platform::CRCreateProcess("LibRetroServerCore", "");
	if (!process)
		return nullptr;
	return std::make_unique<CoreProcess>(std::move(process));
}