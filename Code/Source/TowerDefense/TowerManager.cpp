#include "TowerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreGameInstance.h"
#include "EngineUtils.h" // pour TActorIterator

ATowerManager::ATowerManager()
{
    PrimaryActorTick.bCanEverTick = false;
    CurrentTowerLevel = 1;
    Prix = 100;
}

void ATowerManager::BeginPlay()
{
    Super::BeginPlay();

    // Récupérer les tours déjà placées dans la scène (niveau 1)
    SpawnedTowers.Empty();
    if (TowerLevel1BP)
    {
        for (TActorIterator<AActor> It(GetWorld(), TowerLevel1BP); It; ++It)
        {
            AActor* Tower = *It;
            if (Tower && Tower->IsValidLowLevel())
            {
                SpawnedTowers.Add(Tower);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ %d tours existantes récupérées au début du jeu."), SpawnedTowers.Num());

    // On ne spawn rien pour le niveau 1
    if (CurrentTowerLevel > 1)
    {
        SpawnTowers();
    }
}

void ATowerManager::SpawnTowers()
{
    // Supprimer toutes les tours existantes avant de spawn le niveau actuel
    for (AActor* Tower : SpawnedTowers)
    {
        if (Tower && Tower->IsValidLowLevel())
        {
            Tower->Destroy();
        }
    }
    SpawnedTowers.Empty();

    // Déterminer le Blueprint du niveau actuel
    TSubclassOf<AActor> TowerBP = nullptr;
    if (CurrentTowerLevel == 1)
        TowerBP = TowerLevel1BP;
    else if (CurrentTowerLevel == 2)
        TowerBP = TowerLevel2BP;
    else if (CurrentTowerLevel == 3)
        TowerBP = TowerLevel3BP;

    if (!TowerBP)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Aucun Blueprint défini pour le niveau %d !"), CurrentTowerLevel);
        return;
    }

    // Spawn des tourelles aux positions définies
    for (const FVector& Loc : TowerSpawnLocations)
    {
        AActor* NewTower = GetWorld()->SpawnActor<AActor>(TowerBP, Loc, FRotator::ZeroRotator);
        if (NewTower)
        {
            SpawnedTowers.Add(NewTower);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ %d tourelles de niveau %d spawnées."), SpawnedTowers.Num(), CurrentTowerLevel);
}

void ATowerManager::UpgradeTowers()
{
    UScoreGameInstance* MyGI = Cast<UScoreGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!MyGI)
        return;

    int32 NextCost = 0;
    int32 NextPrix = Prix;

    if (CurrentTowerLevel == 1)
    {
        NextCost = 100;
        NextPrix = 200;
    }
    else if (CurrentTowerLevel == 2)
    {
        NextCost = 200;
        NextPrix = 300;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ Les tourelles sont déjà au niveau maximum."));
        return;
    }

    // Vérifier si le joueur a assez de score
    if (MyGI->Score < NextCost)
    {
        UE_LOG(LogTemp, Warning, TEXT("💸 Score insuffisant pour upgrade. (%d requis, %d actuel)"), NextCost, MyGI->Score);
        return;
    }

    // Appliquer le coût et passer au niveau supérieur
    Cost = NextCost;
    Prix = NextPrix;
    MyGI->Score -= Cost;
    CurrentTowerLevel++;

    // Spawn seulement si niveau > 1
    if (CurrentTowerLevel > 1)
    {
        SpawnTowers(); // détruit les tours existantes (niveau 1) et spawn les nouvelles
    }

    UE_LOG(LogTemp, Warning, TEXT("⬆️ Tourelles améliorées au niveau %d. Score restant : %d"), CurrentTowerLevel, MyGI->Score);
}
