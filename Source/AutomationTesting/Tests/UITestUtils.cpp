#include "UITestUtils.h"

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
}
