#include "TowerDefenseGameState.h"
#include "WaveData.h"
#include "Engine/DataTable.h"
#include "Alien.h"
#include "UFO.h"
#include "TankAlien.h"
#include "BaseEnemy.h"
#include "ScoreGameInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ATowerDefenseGameState::ATowerDefenseGameState()
{
    PrimaryActorTick.bCanEverTick = true;

    CurrentWaveIndex = 0;
    SpawnedEnemies = 0;
    AliveEnemies = 0;
    TotalEnemiesThisWave = 0;

    SpawnLocation = FVector(-510.f, 120.f, 260.f);
    SpawnRotation = FRotator::ZeroRotator;

    // Exemple de positions multiples
    SpawnLocations.Add(FVector(-510.f, 120.f, 260.f));
    SpawnLocations.Add(FVector(1290.f, 1350.f, 240.f));
}

void ATowerDefenseGameState::BeginPlay()
{
    Super::BeginPlay();

    if (WaveDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ WaveDataTable trouvé, lancement des vagues..."));
        StartNextWave();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Aucun DataTable assigné dans le GameState !"));
    }
}

void ATowerDefenseGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // 🔹 Mettre à jour le temps restant seulement si timer actif
    if (GetWorldTimerManager().IsTimerActive(NextWaveTimer))
    {
        TempsAvantProchaineVague = GetWorldTimerManager().GetTimerRemaining(NextWaveTimer);
    }
    else
    {
        TempsAvantProchaineVague = 0.f;
    }
}

void ATowerDefenseGameState::StartWaves()
{
    if (WaveDataTable)
    {
        StartNextWave();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("⚠️ Aucun DataTable assigné dans le GameState"));
    }
}

void ATowerDefenseGameState::StartNextWave()
{
    static const FString Context(TEXT("Wave Context"));
    TArray<FWaveData*> AllWaves;
    WaveDataTable->GetAllRows(Context, AllWaves);

    if (CurrentWaveIndex >= AllWaves.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ Toutes les vagues terminées !"));
        return;
    }

    FWaveData* CurrentWave = AllWaves[CurrentWaveIndex];
    if (!CurrentWave)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Données de vague introuvables pour index %d"), CurrentWaveIndex);
        return;
    }

    // Total d'ennemis pour cette vague
    TotalEnemiesThisWave = CurrentWave->AlienCount + CurrentWave->UFOCount + CurrentWave->TankAlienCount;
    SpawnedEnemies = 0;
    AliveEnemies = 0;

    UE_LOG(LogTemp, Warning, TEXT("🔥 Démarrage de la vague %d (%d ennemis)"),
        CurrentWave->WaveNumber, TotalEnemiesThisWave);

    // Démarrage du timer de spawn
    GetWorldTimerManager().SetTimer(
        SpawnTimer,
        this,
        &ATowerDefenseGameState::SpawnEnemy,
        CurrentWave->SpawnInterval,
        true
    );

    CurrentNextWaveDelay = CurrentWave->NextWaveDelay;
}

void ATowerDefenseGameState::SpawnEnemy()
{
    static const FString Context(TEXT("Wave Context"));
    TArray<FWaveData*> AllWaves;
    WaveDataTable->GetAllRows(Context, AllWaves);

    if (CurrentWaveIndex >= AllWaves.Num())
        return;

    FWaveData* CurrentWave = AllWaves[CurrentWaveIndex];
    if (!CurrentWave)
        return;

    if (SpawnedEnemies >= TotalEnemiesThisWave)
    {
        GetWorldTimerManager().ClearTimer(SpawnTimer);
        return;
    }

    // Détermination du type d'ennemi
    TSubclassOf<ABaseEnemy> EnemyToSpawnClass = nullptr;
    int32 Index = SpawnedEnemies;

    if (Index < CurrentWave->UFOCount)
        EnemyToSpawnClass = CurrentWave->UFOClass;
    else if (Index < CurrentWave->UFOCount + CurrentWave->AlienCount)
        EnemyToSpawnClass = CurrentWave->AlienClass;
    else
        EnemyToSpawnClass = CurrentWave->TankAlienClass;

    if (!EnemyToSpawnClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Classe d’ennemi invalide pour le spawn %d"), SpawnedEnemies + 1);
        SpawnedEnemies++;
        return;
    }

    // Choix aléatoire de spawn
    int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
    FVector SpawnLoc = SpawnLocations.IsValidIndex(RandomIndex) ? SpawnLocations[RandomIndex] : SpawnLocation;
    FRotator SpawnRot = SpawnRotation;

    ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyToSpawnClass, SpawnLoc, SpawnRot);
    if (SpawnedEnemy)
    {
        AliveEnemies++;
        SpawnedEnemy->OnEnemyDeath.AddDynamic(this, &ATowerDefenseGameState::OnEnemyDied);

        UE_LOG(LogTemp, Warning, TEXT("👾 Ennemi %d (%s) spawné à %s (vivants : %d)"),
            SpawnedEnemies + 1,
            *SpawnedEnemy->GetName(),
            *SpawnLoc.ToString(),
            AliveEnemies);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ Échec du spawn de l’ennemi %d pour la vague %d"),
            SpawnedEnemies + 1, CurrentWaveIndex + 1);
    }

    SpawnedEnemies++;
}

void ATowerDefenseGameState::OnEnemyDied(ABaseEnemy* DeadEnemy)
{
    if (!DeadEnemy)
        return;

    AliveEnemies--;

    int32 PointsEarned = 0;

    // Déterminer les points selon la classe
    if (DeadEnemy->IsA(AAlien::StaticClass()))
        PointsEarned = 50;
    else if (DeadEnemy->IsA(AUFO::StaticClass()))
        PointsEarned = 20;
    else if (DeadEnemy->IsA(ATankAlien::StaticClass()))
        PointsEarned = 50;

    // Ajouter au score dans GameInstance
    UScoreGameInstance* MyGI = Cast<UScoreGameInstance>(GetGameInstance());
    if (MyGI)
    {
        MyGI->AjouterScore(PointsEarned);
        UE_LOG(LogTemp, Warning, TEXT("+%d points ajoutés. Total: %d"), PointsEarned, MyGI->Score);
    }

    UE_LOG(LogTemp, Warning, TEXT("💀 %s est mort. Restants : %d"), *DeadEnemy->GetName(), AliveEnemies);

    // Passage à la prochaine vague seulement si tous les ennemis spawnés et morts
    if (AliveEnemies <= 0 && SpawnedEnemies >= TotalEnemiesThisWave)
    {
        static const FString Context(TEXT("Wave Context"));
        TArray<FWaveData*> AllWaves;
        WaveDataTable->GetAllRows(Context, AllWaves);

        if (CurrentWaveIndex + 1 >= AllWaves.Num())
        {
            UE_LOG(LogTemp, Warning, TEXT("🎉 Toutes les vagues sont terminées, pas de timer lancé."));
            return;
        }

        CurrentWaveIndex++;
        GetWorldTimerManager().SetTimer(
            NextWaveTimer,
            this,
            &ATowerDefenseGameState::StartNextWave,
            CurrentNextWaveDelay,
            false
        );
    }
}

void ATowerDefenseGameState::DecrementAliveEnemies()
{
    AliveEnemies--;
    UE_LOG(LogTemp, Warning, TEXT("💀 Ennemi tué. Restants : %d"), AliveEnemies);

    if (AliveEnemies <= 0 && SpawnedEnemies >= TotalEnemiesThisWave)
    {
        CurrentWaveIndex++;
        GetWorldTimerManager().SetTimer(
            NextWaveTimer,
            this,
            &ATowerDefenseGameState::StartNextWave,
            CurrentNextWaveDelay,
            false
        );
    }
}
