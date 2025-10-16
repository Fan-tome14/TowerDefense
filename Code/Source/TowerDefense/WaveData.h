#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveData.generated.h"

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Numéro de la vague
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNumber;

	// Type d’alien (Blueprint à spawn)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AAlien> AlienClass;

	// Nombre d'aliens à faire apparaître
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AlienCount;

	// Type d’UFO (Blueprint à spawn)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AUFO> UFOClass;
	
	// Nombre d'UFO à faire apparaître
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UFOCount;

	// Type de TankAlien (Blueprint à spawn)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATankAlien> TankAlienClass;

	// Nombre de TankAlien à faire apparaître
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TankAlienCount;
	
	// Délai entre chaque spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval;

	// Temps avant la vague suivante
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NextWaveDelay;
};
