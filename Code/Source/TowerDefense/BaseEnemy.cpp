#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreGameInstance.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    Vie = 100.f;
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
     if (UScoreGameInstance* GI = Cast<UScoreGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
     {
        GI->AjouterScore();
     }
        

        // Déclencher l’événement de mort
        OnEnemyDeath.Broadcast(this);

        Destroy();
    }
}


