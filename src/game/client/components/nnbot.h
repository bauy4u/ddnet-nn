/* Experimental client-side NN model loader.
 * This component only loads and reports .pt models. It does not generate or send player input.
 */
#ifndef GAME_CLIENT_COMPONENTS_NNBOT_H
#define GAME_CLIENT_COMPONENTS_NNBOT_H

#include <base/system.h>
#include <game/client/component.h>

#include <memory>

class CNNBotRuntime;

class CNNBot : public CComponent
{
	std::unique_ptr<CNNBotRuntime> m_pRuntime;
	bool m_ModelLoaded = false;
	char m_aModelPath[IO_MAX_PATH_LENGTH] = {0};

	static void ConLoad(IConsole::IResult *pResult, void *pUserData);
	static void ConUnload(IConsole::IResult *pResult, void *pUserData);
	static void ConStatus(IConsole::IResult *pResult, void *pUserData);

public:
	CNNBot();
	virtual ~CNNBot();
	virtual int Sizeof() const override { return sizeof(*this); }
	virtual void OnConsoleInit() override;
	virtual void OnReset() override;

	bool LoadModel(const char *pPath);
	void UnloadModel();
	void PrintStatus() const;
};

#endif // GAME_CLIENT_COMPONENTS_NNBOT_H
