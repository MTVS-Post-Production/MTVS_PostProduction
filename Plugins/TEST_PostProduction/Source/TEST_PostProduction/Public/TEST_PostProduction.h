// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"

class FToolBarBuilder;
class FMenuBuilder;

class FTEST_PostProductionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	inline static bool isLoginSuccess = false;
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;


#pragma region CustomEditorTab

	void RegisterCustomEditorTab();

	TSharedRef<SDockTab> OnSpawnPostProductionTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnMainMenuTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnWebTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnSoundTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnLoginTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnMemoTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnMemoCommentDetailTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnMemoWriteTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnDetailTab(const FSpawnTabArgs& spawnArgs);

	TSharedRef<SDockTab> OnSpawnImageTab(const FSpawnTabArgs& spawnArgs);


	class UEditorUtilityWidgetBlueprint* LoadedEditorWidget;

#pragma endregion 

#pragma region ContentBrowserMenuExtention

	void InitCBMenuExtension();

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SeletedPaths);

	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);

	void OnDeleteUnusedAssetButtonClicked();
#pragma endregion
	

};
