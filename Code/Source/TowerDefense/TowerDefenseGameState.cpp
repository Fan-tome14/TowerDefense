#include "TowerDefenseGameState.h"
#include "WaveData.h"
#include "Engine/DataTable.h"
#include "Alien.h"
#include "TimerManager.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "UFO.h"

ATowerDefenseGameState::ATowerDefenseGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentWaveIndex = 0;
	AliveEnemies = 0;

	SpawnLocation = FVector(-510.f, 120.f, 260.f);
	SpawnRotation = FRotator::ZeroRotator;

}



void ATowerDefenseGameState::BeginPlay()
{
	Super::BeginPlay();
	if (WaveDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ WaveDataTable trouvé, lancement des vagues..."));
		StartNextWave();
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Aucun DataTable assigné dans le GameState !"));
	}
}


void ATowerDefenseGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

	UE_LOG(LogTemp, Warning, TEXT("🔥 Démarrage de la vague %d (%d ennemis)"),
		CurrentWave->WaveNumber, CurrentWave->AlienCount);

	SpawnedEnemies = 0;
	AliveEnemies = 0;

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

	int32 TotalToSpawn = CurrentWave->AlienCount + CurrentWave->UFOCount;

	// Si on a déjà tout spawn
	if (SpawnedEnemies >= TotalToSpawn)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		return;
	}

	// 🧮 Décide quel type spawn
	TSubclassOf<ABaseEnemy> EnemyToSpawnClass = nullptr;

	if (SpawnedEnemies < CurrentWave->UFOCount)
	{
		EnemyToSpawnClass = CurrentWave->UFOClass;
	}
	else
	{
		EnemyToSpawnClass = CurrentWave->AlienClass;
	}

	if (!EnemyToSpawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Classe d’ennemi invalide pour le spawn %d"), SpawnedEnemies + 1);
		SpawnedEnemies++;
		return;
	}

	FVector SpawnLoc = SpawnLocation;
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
	{
		UE_LOG(LogTemp, Error, TEXT("⚠️ DeadEnemy est null dans OnEnemyDied !"));
		return;
	}

	AliveEnemies--;
	UE_LOG(LogTemp, Warning, TEXT("💀 %s est mort. Restants : %d"), *DeadEnemy->GetName(), AliveEnemies);

	if (AliveEnemies <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ Vague %d terminée, préparation de la suivante..."), CurrentWaveIndex + 1);
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
