#ifndef GAME_CLIENT_COMPONENTS_NNMODEL_H
#define GAME_CLIENT_COMPONENTS_NNMODEL_H

#include <base/system.h>
#include <game/client/component.h>

#if defined(CONF_CLIENT_NNMODEL)
#include <engine/server/NN/Models.h>
#include <torch/torch.h>
#endif

class CNNModelLoader : public CComponent
{
public:
	virtual int Sizeof() const override { return sizeof(*this); }
	virtual void OnConsoleInit() override;

	bool Load(const char *pPath);
	void Unload();
	void Status() const;

private:
	static void ConLoad(IConsole::IResult *pResult, void *pUserData);
	static void ConUnload(IConsole::IResult *pResult, void *pUserData);
	static void ConStatus(IConsole::IResult *pResult, void *pUserData);

	bool m_Loaded = false;
	char m_aPath[IO_MAX_PATH_LENGTH] = {0};
#if defined(CONF_CLIENT_NNMODEL)
	ActorCritic m_Model{nullptr};
	torch::Device m_Device = torch::kCPU;
#endif
};

#endif // GAME_CLIENT_COMPONENTS_NNMODEL_H
