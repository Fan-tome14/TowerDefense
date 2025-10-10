#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

// ✅ Délégué pour signaler la mort de l’ennemi
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, ABaseEnemy*, DeadEnemy);

UCLASS()
class TOWERDEFENSE_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	// ✅ Événement appelé quand l’ennemi est détruit
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDeath OnEnemyDeath;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ✅ On override Destroyed() pour déclencher notre événement
	virtual void Destroyed() override;
};
