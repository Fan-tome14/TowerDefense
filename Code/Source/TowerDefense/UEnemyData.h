// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UEnemyData.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UUEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Info")FName  Nom;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Stats")int Vie;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Stats")float Vitesse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")TSubclassOf<ACharacter> EnemyClass;
};
