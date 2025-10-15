// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TourBase.h"
#include "TourMagique.generated.h"

UCLASS()
class TOWERDEFENSE_API ATourMagique : public ATourBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATourMagique();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
