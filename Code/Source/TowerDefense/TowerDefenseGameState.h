#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TowerDefenseGameState.generated.h"

class UDataTable;
class ABaseEnemy;

UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATowerDefenseGameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
	UDataTable* WaveDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave System")
	FRotator SpawnRotation;

	void StartWaves();
	void StartNextWave();
	void SpawnEnemy();

	UFUNCTION()
	void OnEnemyDied(ABaseEnemy* DeadEnemy);

private:
	int32 CurrentWaveIndex;
	int32 SpawnedEnemies;
	int32 AliveEnemies;
	float CurrentNextWaveDelay;

	FTimerHandle SpawnTimer;
	FTimerHandle NextWaveTimer;
};
