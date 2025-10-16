#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreGameInstance.h"

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

void ABaseEnemy::SubirDegats(float Quantite)
{
    Vie -= Quantite;

    if (Vie <= 0.f)
    {
        // Déclencher l’événement de mort
        OnEnemyDeath.Broadcast(this);
        Destroy();
    }
}


