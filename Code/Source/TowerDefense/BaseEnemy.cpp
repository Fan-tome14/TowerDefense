#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::Destroyed()
{
	Super::Destroyed();

	// ⚡ Notifie tous les abonnés que cet ennemi vient d’être détruit
	OnEnemyDeath.Broadcast(this);

	UE_LOG(LogTemp, Warning, TEXT("☠️ %s a été détruit (broadcast envoyé)"), *GetName());
}
