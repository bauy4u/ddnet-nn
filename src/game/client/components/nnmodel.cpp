#include <base/system.h>
#include <engine/console.h>
#include <game/client/component.h>
#include <game/client/gameclient.h>

#if defined(CONF_CLIENT_NNMODEL)
#include <engine/server/NN/Models.h>
#include <torch/torch.h>
#endif

class CNNModelLoader : public CComponent
{
public:
	virtual int Sizeof() const override { return sizeof(*this); }
	virtual void OnConsoleInit() override
	{
		Console()->Register("cl_nnmodel_load", "r[path]", CFGFLAG_CLIENT, ConLoad, this, "Load a .pt neural model");
		Console()->Register("cl_nnmodel_unload", "", CFGFLAG_CLIENT, ConUnload, this, "Unload neural model");
		Console()->Register("cl_nnmodel_status", "", CFGFLAG_CLIENT, ConStatus, this, "Print neural model status");
	}

	bool Load(const char *pPath)
	{
#if !defined(CONF_CLIENT_NNMODEL)
		(void)pPath;
		dbg_msg("client-nnmodel", "not compiled with CONF_CLIENT_NNMODEL");
		return false;
#else
		try
		{
			m_Model = ActorCritic(3345, 9, 2e-2);
			torch::load(m_Model, pPath);
			m_Device = torch::cuda::is_available() ? torch::Device(torch::kCUDA) : torch::Device(torch::kCPU);
			m_Model->to(m_Device);
			m_Model->eval();
			str_copy(m_aPath, pPath, sizeof(m_aPath));
			m_Loaded = true;
			dbg_msg("client-nnmodel", "loaded on %s: %s", m_Device.is_cuda() ? "CUDA" : "CPU", pPath);
			return true;
		}
		catch(const std::exception &e)
		{
			dbg_msg("client-nnmodel", "load failed: %s", e.what());
			m_Loaded = false;
			m_aPath[0] = 0;
			m_Model = nullptr;
			return false;
		}
#endif
	}

	void Unload()
	{
#if defined(CONF_CLIENT_NNMODEL)
		m_Model = nullptr;
#endif
		m_Loaded = false;
		m_aPath[0] = 0;
	}

	void Status() const
	{
		dbg_msg("client-nnmodel", "compiled=%s loaded=%d path='%s'",
#if defined(CONF_CLIENT_NNMODEL)
			"yes",
#else
			"no",
#endif
			m_Loaded, m_aPath);
	}

private:
	static void ConLoad(IConsole::IResult *pResult, void *pUserData) { ((CNNModelLoader *)pUserData)->Load(pResult->GetString(0)); }
	static void ConUnload(IConsole::IResult *pResult, void *pUserData) { (void)pResult; ((CNNModelLoader *)pUserData)->Unload(); }
	static void ConStatus(IConsole::IResult *pResult, void *pUserData) { (void)pResult; ((CNNModelLoader *)pUserData)->Status(); }

	bool m_Loaded = false;
	char m_aPath[IO_MAX_PATH_LENGTH] = {0};
#if defined(CONF_CLIENT_NNMODEL)
	ActorCritic m_Model{nullptr};
	torch::Device m_Device = torch::kCPU;
#endif
};
