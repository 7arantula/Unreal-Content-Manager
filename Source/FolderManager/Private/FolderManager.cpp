#include "FolderManager.h"
#include "FolderManagerCommands.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Widgets/Input/SEditableTextBox.h"

#define LOCTEXT_NAMESPACE "FFolderManagerModule"

void FFolderManagerModule::StartupModule()
{
    FFolderManagerCommands::Register();
    PluginCommands = MakeShareable(new FUICommandList);
    
    PluginCommands->MapAction(
        FFolderManagerCommands::Get().CreateFolders,
        FExecuteAction::CreateRaw(this, &FFolderManagerModule::CreateAssetFolders),
        FCanExecuteAction());
        
    FLevelEditorModule& LevelEditorModule = 
        FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
        
    TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
    MenuExtender->AddMenuExtension(
        "WindowLayout",
        EExtensionHook::After,
        PluginCommands,
        FMenuExtensionDelegate::CreateRaw(this, &FFolderManagerModule::AddMenuExtension));
        
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FFolderManagerModule::AddMenuExtension(FMenuBuilder& Builder)
{
    Builder.AddMenuEntry(FFolderManagerCommands::Get().CreateFolders);
}

void FFolderManagerModule::CreateAssetFolders()
{
    bool bUserConfirmed = false;
    FString UserEnteredName;

    TSharedRef<SWindow> InputWindow = SNew(SWindow)
        .Title(LOCTEXT("CreateFoldersDialogTitle", "Give it a Name"))
        .ClientSize(FVector2D(400, 150))
        .SupportsMaximize(false)
        .SupportsMinimize(false);

    TSharedPtr<SEditableTextBox> FolderNameTextBox;
    InputWindow->SetContent(
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .Padding(10)
        [
            SNew(STextBlock)
            .Text(LOCTEXT("CreateFoldersDialogText", "Don't think, Just give it a name:"))
        ]
        + SVerticalBox::Slot()
        .Padding(10)
        [
            SAssignNew(FolderNameTextBox, SEditableTextBox)
        ]
        + SVerticalBox::Slot()
        .Padding(167,20,167,10)
        [
            SNew(SButton)
            .ButtonColorAndOpacity(FLinearColor(150,150,150,.1))
            .Text(LOCTEXT("CreateFoldersDialogButton", "OKAY"))
            .OnClicked_Lambda([&bUserConfirmed, &UserEnteredName, FolderNameTextBox, Window = InputWindow]() {
                bUserConfirmed = true;
                UserEnteredName = FolderNameTextBox->GetText().ToString();
                Window->RequestDestroyWindow();
                return FReply::Handled();
            })
        ]
    );

    GEditor->EditorAddModalWindow(InputWindow);

    if (!bUserConfirmed || UserEnteredName.IsEmpty())
    {
        return;
    }

    FString BasePath = TEXT("All/Content/") + UserEnteredName;
    TArray<FString> SubFolders = {
        TEXT("Blueprint"),
        TEXT("Meshes"),
        TEXT("Textures"),
        TEXT("Materials")
    };
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    for (const FString& SubFolder : SubFolders)
    {
        FString FullPath = BasePath + TEXT("/") + SubFolder;
        FString FullPathOnDisk = FPaths::ProjectContentDir() + UserEnteredName + TEXT("/") + SubFolder;
        FPaths::NormalizeDirectoryName(FullPathOnDisk);
        PlatformFile.CreateDirectoryTree(*FullPathOnDisk);
    }

    FMessageDialog::Open(EAppMsgType::Ok, 
        FText::Format(LOCTEXT("FoldersCreated", "Created folders inside > {0}"), 
        FText::FromString(BasePath)));
}

void FFolderManagerModule::ShutdownModule()
{
    FFolderManagerCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFolderManagerModule, FolderManager)
