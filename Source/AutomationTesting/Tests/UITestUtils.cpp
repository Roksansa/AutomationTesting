#include "UITestUtils.h"

#include "AutomationBlueprintFunctionLibrary.h"
#include "BufferVisualizationData.h"
#include "TestUtils.h"
#include "Blueprint/WidgetTree.h"


namespace UE::TEST
{
UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
{
	if (!Widget || !Widget->WidgetTree) return nullptr;

	UWidget* FoundWidget = nullptr;
	UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
		[&](UWidget* Child)
		{
			if (Child && Child->GetFName().IsEqual(WidgetName))
			{
				FoundWidget = Child;
			}
		});
	return FoundWidget;
}


FTakeScreenshotLatentCommand::FTakeScreenshotLatentCommand(const FString& InScreenshotName)
	: ScreenshotName(InScreenshotName)
{
	FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
}

FTakeScreenshotLatentCommand::~FTakeScreenshotLatentCommand()
{
	FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
}

void FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
	CommandCompleted = true;
}

FTakeGameScreenshotLatentCommand::FTakeGameScreenshotLatentCommand(const FString& InScreenshotName)
	: FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeGameScreenshotLatentCommand::Update()
{
	if (!ScreenshotRequested)
	{
		const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
		UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetAnyGameWorld(), ScreenshotName, FString{}, Options);
		ScreenshotRequested = true;
	}
	return CommandCompleted;
}

FTakeUIScreenshotLatentCommand::FTakeUIScreenshotLatentCommand(const FString& InScreenshotName)
	: FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeUIScreenshotLatentCommand::Update()
{
	if (!ScreenshotSetupDone)
	{
		ScreenshotSetupDone = true;
		SetBufferVisualization("Opacity");
		return false;
	}

	if (!ScreenshotRequested)
	{
		const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
		UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetAnyGameWorld(), ScreenshotName, Options);
		ScreenshotRequested = true;
	}
	return CommandCompleted;
}

void FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
	FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
	SetBufferVisualization(NAME_None);
}

void FTakeUIScreenshotLatentCommand::SetBufferVisualization(const FName& VisualizeBuffer)
{
	if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
	{
		static IConsoleVariable* ICVar =
			IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
		if (ICVar)
		{
			if (ViewportClient->GetEngineShowFlags())
			{
				ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(VisualizeBuffer == NAME_None ? false : true);
				ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer == NAME_None ? true : false);
				ICVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
			}
		}
	}
}
}
