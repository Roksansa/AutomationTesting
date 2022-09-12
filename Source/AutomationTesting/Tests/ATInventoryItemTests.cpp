// Automation testing project copyright

#if WITH_AUTOMATION_TESTS
#include "AutomationTesting/Tests/ATInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "TestUtils.h"
#include "Inventory/ATInventoryItem.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "AutomationTesting/AutomationTestingCharacter.h"
#include "Inventory/ATInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

//fix warning - enable streaming in world settings
//map close automation after ended test -- upd 5.0 ue automationControllerManager - 501 line - GUnrealEd->RequestEndPlayMap();

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "OriginGame.Inventory.Item.CppActorCantBeCreated",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "OriginGame.Inventory.Item.BlueprintShouldBeSetupCorrectly",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "OriginGame.Inventory.Item.InventoryDataShouldBeSetupCorrectly",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTaken, "OriginGame.Inventory.Item.InventoryItemCanBeTaken",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMeshForEveryInventoryItemShouldExist, "OriginGame.Inventory.Item.MeshForEveryInventoryItemShouldExist",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

constexpr char* NewMapName = "/Game/AutomationTesting/Test/EmptyTestLevel";
constexpr char* InventoryItemBPName = "Blueprint'/Game/AutomationTesting/Actors/BP_InventoryItem.BP_InventoryItem'";
constexpr char* InventoryItemBPTestName = "Blueprint'/Game/AutomationTesting/Test/BP_InventoryItem_Test.BP_InventoryItem_Test'";

using namespace UE::TEST;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
	const FString ExpectedWarnMsg =
		FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *AATInventoryItem::StaticClass()->GetName());
	AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{100.f}};
	const AATInventoryItem* InventoryItem = World->SpawnActor<AATInventoryItem>(AATInventoryItem::StaticClass(), InitialTransform);
	if (!TestNull("Inventory item exist", InventoryItem)) { return false; }

	return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{60.f}};
	const AATInventoryItem* InventoryItem = UE::TEST::CreateBlueprint<AATInventoryItem>(World, InventoryItemBPName, InitialTransform);
	if (!TestNotNull("Inventory item exist", InventoryItem)) { return false; }

	const auto CollisionComp = InventoryItem->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Sphere component exists", CollisionComp)) { return false; }

	TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	TestTrueExpr(InventoryItem->GetRootComponent() == CollisionComp);

	UE::TEST::ForEach<ECollisionChannel>([&](const ECollisionChannel EnumValue, const FName& Name)
	{
		if (EnumValue != ECollisionChannel::ECC_OverlapAll_Deprecated)
		{
			TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EnumValue) == ECollisionResponse::ECR_Overlap);
		}
	});

	const auto TextRenderComp = InventoryItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text render component exists", TextRenderComp)) { return false; }

	const auto StaticMeshComp = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Static mesh component exists", StaticMeshComp)) { return false; }

	TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f));
	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{60.f}};
	AATInventoryItem* InventoryItem = UE::TEST::CreateBlueprint<AATInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
	if (!TestNotNull("Inventory item exist", InventoryItem)) { return false; }

	constexpr FInventoryData InventoryData{EInventoryItemType::CUBE, 20};
	const FLinearColor Color = FLinearColor::Yellow;
	UE::TEST::CallFuncByNameWithParams(InventoryItem, "SetInventoryData", {InventoryData.ToString(), Color.ToString()});

	const auto TextRenderComp = InventoryItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text render component exists", TextRenderComp)) { return false; }

	TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InventoryData.Score)));
	TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

	const auto StaticMeshComp = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Static mesh component exists", StaticMeshComp)) { return false; }

	const auto Material = StaticMeshComp->GetMaterial(0);
	if (!TestNotNull("Material exist", Material)) { return false; }

	FLinearColor MaterialColor;
	Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"BaseColor"}, MaterialColor);
	TestTrueExpr(MaterialColor == Color);

	return true;
}

bool FInventoryItemCanBeTaken::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	const FTransform InitialTransform{FVector{60.f}};
	AATInventoryItem* InventoryItem = UE::TEST::CreateBlueprint<AATInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
	if (!TestNotNull("Inventory item exist", InventoryItem)) { return false; }

	constexpr FInventoryData InventoryData{EInventoryItemType::CUBE, 20};
	const FLinearColor Color = FLinearColor::Yellow;
	UE::TEST::CallFuncByNameWithParams(InventoryItem, "SetInventoryData", {InventoryData.ToString(), Color.ToString()});

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, AAutomationTestingCharacter::StaticClass(), Actors);
	if (!TestTrueExpr(Actors.Num() == 1)) { return false; }

	const auto Character = Cast<AAutomationTestingCharacter>(Actors[0]);
	if (!TestNotNull("Character exist", Character)) { return false; }

	const auto InventoryComponent = Character->FindComponentByClass<UATInventoryComponent>();
	if (!TestNotNull("Inventory component exist", InventoryComponent)) { return false; }

	const int32 CurrentCount = InventoryComponent->GetInventoryAmountByType(InventoryData.Type);
	if (!TestTrueExpr(CurrentCount >= 0)) { return false; }

	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), Actors);
	const int32 CountActors = Actors.Num();
	Character->SetActorLocation(InitialTransform.GetLocation());

	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(InventoryData.Type) - CurrentCount == InventoryData.Score);
	TestTrueExpr(!IsValid(InventoryItem));

	UGameplayStatics::GetAllActorsOfClass(World, AATInventoryItem::StaticClass(), Actors);
	TestTrueExpr(CountActors == Actors.Num() + 1);

	return true;
}

bool FMeshForEveryInventoryItemShouldExist::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f));
	LevelScope{NewMapName};
	UWorld* World = UE::TEST::GetAnyGameWorld();
	if (!TestNotNull("World exist", World)) { return false; }

	UE::TEST::ForEach<EInventoryItemType>([&](const EInventoryItemType EnumValue, const FName& Name)
	{
		const FTransform InitialTransform{FVector{60.f * (static_cast<int>(EnumValue) + 1)}};
		AATInventoryItem* InventoryItem = UE::TEST::CreateBlueprint<AATInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
		TestNotNull("Inventory item exist", InventoryItem);
		const FInventoryData InventoryData{EnumValue, 20};
		const FLinearColor Color = FLinearColor::Green;
		UE::TEST::CallFuncByNameWithParams(InventoryItem, "SetInventoryData", {InventoryData.ToString(), Color.ToString()});
		const auto StaticMeshComp = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
		TestNotNull("Static mesh component exists", StaticMeshComp);
		const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exists"), *UEnum::GetValueAsString(EnumValue));
		TestNotNull(*MeshMsg, StaticMeshComp->GetStaticMesh().Get());
	});
	return true;
}

#endif
