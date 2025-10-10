#include "EndZone.h"
#include "BaseEnemy.h" // pour reconnaître tes ennemis

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

void AKillZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("💥 %s détruit par KillZone"), *Enemy->GetName());
		Enemy->Destroy();
	}
}
