#include "CoreProcess.h"
#include <Platform\Process.h>
#include <Platform\PathUtils.h>
#include <Platform\Guid.h>
#include <Platform\PipeServer.h>
#include <LibRetroServer\Messages.h>

namespace CR
{
	namespace LibRetroServer
	{
		class CoreProcess : public ICoreProcess
		{
		public:
			CoreProcess();
			virtual ~CoreProcess() = default;
			bool InitializeProcess();
			bool WaitForClose(const std::chrono::milliseconds& a_maxWait) override;
			void Initialize(const char* a_coreName, const char* a_gameName) override;
			void Shutdown() override;
		private:
			void OnPipeWriteFinished(void* a_msg, size_t a_msgSize);
			void OnPipeReadFinished(void* a_msg, size_t a_msgSize);

			std::string m_pipeName;
			std::unique_ptr<CR::Platform::IProcess> m_process;
			std::unique_ptr<CR::Platform::IPipeServer> m_pipe;
		};
	}
}

using namespace CR::LibRetroServer;
using namespace CR;

CoreProcess::CoreProcess()
{
}

bool CoreProcess::InitializeProcess()
{
	auto pipeGuid = Platform::CreateGuid();
	std::string pipeName{R"(\\.\pipe\)"};
	pipeName += pipeGuid.ToStringClean();
	m_pipe = Platform::CreatePipeServer(pipeName.c_str(), [this](auto a_msg, auto a_msgSize) {
		this->OnPipeWriteFinished(a_msg, a_msgSize); }, [this](auto a_msg, auto a_msgSize) {
			this->OnPipeReadFinished(a_msg, a_msgSize); });
	if(!m_pipe)
		return false;

	auto corePath = CR::Platform::RelativeToAbsolute("LibRetroServerCore.exe");
	m_process = CR::Platform::CRCreateProcess(corePath.c_str(), pipeGuid.ToStringClean().c_str());
	if(!m_process)
		return false;

	return true;
}

bool CoreProcess::WaitForClose(const std::chrono::milliseconds& a_maxWait)
{
	return m_process->WaitForClose(a_maxWait);
}

void CoreProcess::Initialize(const char* a_coreName, const char* a_gameName)
{
	auto* msg = new LibRetroServer::Messages::InitializeMessage;
	strcpy_s(msg->CorePath, a_coreName);
	strcpy_s(msg->GamePath, a_gameName);
	m_pipe->WriteMsg(msg);
}

void CoreProcess::Shutdown()
{
	auto* msg = new LibRetroServer::Messages::ShutdownMessage;
	m_pipe->WriteMsg(msg);
}

void CoreProcess::OnPipeWriteFinished(void* a_msg, size_t)
{
	delete a_msg;
}

void CoreProcess::OnPipeReadFinished(void* a_msg, size_t a_msgSize)
{
	a_msg; a_msgSize;
}

std::unique_ptr<ICoreProcess> CR::LibRetroServer::CreateCoreProcess()
{
	auto result = std::make_unique<CoreProcess>();
	if(!result->InitializeProcess())
		return nullptr;
	return std::move(result);
}