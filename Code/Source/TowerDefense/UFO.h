#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UEnemyData.h"
#include "Engine/TargetPoint.h"
#include "UFO.generated.h"

UCLASS()
class TOWERDEFENSE_API AUFO : public ACharacter
{
	GENERATED_BODY()

public:
	AUFO();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	/** Target point dans la scène que l'AI doit rejoindre */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	ATargetPoint* TargetPointActor;

	/** Data Asset contenant les stats de l'UFO */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	UUEnemyData* UFOData;

private:
	/** Valeurs internes récupérées depuis l'asset */
	float MovementSpeed;
	float Health;
	FString UFOClass;
	TSubclassOf<ACharacter> UFOClassType;
};
