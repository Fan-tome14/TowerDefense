#include "EndZone.h"
#include "BaseEnemy.h" // pour reconnaître tes ennemis
#include "TowerDefenseGameState.h"

AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// 📦 Création du collider
	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KillBox"));
	RootComponent = KillBox;

	KillBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	KillBox->SetCollisionProfileName(TEXT("Trigger"));

	KillBox->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnOverlapBegin);
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("🟥 KillZone active à %s"), *GetActorLocation().ToString());
}

void AKillZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		ATowerDefenseGameState* GS = GetWorld()->GetGameState<ATowerDefenseGameState>();
		if (GS)
		{
			GS->HandleEnemyRemoved();
		}

		Enemy->Destroy();
	}
}