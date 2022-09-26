#pragma once
#include "TestUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

namespace UE::TEST
{
template <class T>
T* FindWidgetByClass()
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetAnyGameWorld(), Widgets, T::StaticClass(), false);
	return Widgets.Num() != 0 ? Cast<T>(Widgets[0]) : nullptr;
}

UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName);

class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
{
public:
	FTakeScreenshotLatentCommand(const FString& InScreenshotName);
	virtual ~FTakeScreenshotLatentCommand() override;

protected:
	const FString ScreenshotName;
	bool ScreenshotRequested = false;
	bool CommandCompleted = false;

	virtual void OnScreenshotTakenAndCompared();
};

class FTakeGameScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
	FTakeGameScreenshotLatentCommand(const FString& InScreenshotName);

	virtual bool Update() override;
};

class FTakeUIScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
	FTakeUIScreenshotLatentCommand(const FString& InScreenshotName);
	virtual bool Update() override;

private:
	bool ScreenshotSetupDone = false;

	virtual void OnScreenshotTakenAndCompared() override;
	void SetBufferVisualization(const FName& VisualizeBuffer);
};
}
