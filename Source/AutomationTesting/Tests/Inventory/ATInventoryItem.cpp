#include "ATInventoryItem.h"
#include "ATInventoryComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AATInventoryItem::AATInventoryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(CollisionComponent);

	CollisionComponent->InitSphereRadius(30.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);
}

void AATInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (const APawn* Pawn = Cast<APawn>(OtherActor))
	{
		if (const auto InvComp = Pawn->FindComponentByClass<UATInventoryComponent>())
		{
			if (InvComp->TryToAddItem(InventoryData))
			{
				Destroy();
			}
		}
	}
}

