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
}
