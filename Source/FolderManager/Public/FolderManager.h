#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/Commands.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "LevelEditor.h"

class FFolderManagerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
	void AddMenuExtension(FMenuBuilder& Builder);
	void CreateAssetFolders();
	
    
private:
	TSharedPtr<FUICommandList> PluginCommands;
};