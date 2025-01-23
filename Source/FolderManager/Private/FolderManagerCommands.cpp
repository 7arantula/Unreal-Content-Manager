#include "FolderManagerCommands.h"

#define LOCTEXT_NAMESPACE "FFolderManagerCommands"

FFolderManagerCommands::FFolderManagerCommands()
	: TCommands<FFolderManagerCommands>(
		TEXT("FolderManager"),
		NSLOCTEXT("Contexts", "FolderManager", "Folder Manager"),
		NAME_None,
		TEXT("CoreStyle"))
{
}

void FFolderManagerCommands::RegisterCommands()
{
	UI_COMMAND(
		CreateFolders,
		"Create Asset Folders",
		"Creates organized asset folders",
		EUserInterfaceActionType::Button,
		FInputChord());
}

#undef LOCTEXT_NAMESPACE