#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "UEnemyData.h"
#include "Alien.generated.h"

UCLASS()
class TOWERDEFENSE_API AAlien : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AAlien();

protected:
	virtual void BeginPlay() override;
	void StartMovingToTarget();
public:
	virtual void Tick(float DeltaTime) override;

	ATargetPoint* TargetPointActor;

	float MovementSpeed;

	float Health;

	FString AlienClass;

	TSubclassOf<ACharacter> AlienClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UUEnemyData* AlienData;
};
