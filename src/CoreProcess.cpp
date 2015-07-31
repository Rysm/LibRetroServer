#include "CoreProcess.h"
#include <Platform\Process.h>
#include <Platform\PathUtils.h>

namespace CR
{
	namespace LibRetroServer
	{
		class CoreProcess : public ICoreProcess
		{
		public:
			CoreProcess(std::unique_ptr<CR::Platform::IProcess> a_process);
			virtual ~CoreProcess() = default;
			bool WaitForClose(const std::chrono::milliseconds& a_maxWait) override;
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

bool CoreProcess::WaitForClose(const std::chrono::milliseconds& a_maxWait)
{
	return m_process->WaitForClose(a_maxWait);
}

std::unique_ptr<ICoreProcess> CR::LibRetroServer::CreateCoreProcess()
{
	auto corePath = CR::Platform::RelativeToAbsolute("LibRetroServerCore.exe");
	auto process = CR::Platform::CRCreateProcess(corePath.c_str(), "");
	if (!process)
		return nullptr;
	return std::make_unique<CoreProcess>(std::move(process));
}