
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EndZone.generated.h"

UCLASS()
class TOWERDEFENSE_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AKillZone();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision")
	UBoxComponent* KillBox;

	virtual void BeginPlay() override;
	void NotifyActorBeginOverlap(AActor* OtherActor);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
};
