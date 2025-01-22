#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FFolderManagerCommands : public TCommands<FFolderManagerCommands>
{
public:
	FFolderManagerCommands();
	virtual void RegisterCommands() override;
    
	TSharedPtr<FUICommandInfo> CreateFolders;
};