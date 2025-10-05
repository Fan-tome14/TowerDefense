#include "UFO.h"
#include "EnnemyAIController.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AUFO::AUFO()
{
	PrimaryActorTick.bCanEverTick = true;

	// Utiliser l'AIController dédié
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnnemyAIController::StaticClass();
}

void AUFO::BeginPlay()
{
	Super::BeginPlay();

	// Récupération des stats depuis le Data Asset
	if (UFOData)
	{
		MovementSpeed = UFOData->Vitesse;
		Health = UFOData->Vie;
		UFOClass = UFOData->Nom.ToString();
		UFOClassType = UFOData->EnemyClass;

		// Appliquer la vitesse au CharacterMovementComponent
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UFOData non assigné pour %s"), *GetName());
	}

	// Déplacement vers le TargetPoint via NavMesh
	if (TargetPointActor)
	{
		AEnnemyAIController* AICont = Cast<AEnnemyAIController>(GetController());
		if (AICont)
		{
			AICont->MoveToActor(TargetPointActor, 5.0f); // 5.f = tolerance distance
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController non trouvé pour %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPoint non assigné pour %s"), *GetName());
	}
}

void AUFO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotation vers la cible
	if (TargetPointActor)
	{
		FVector Direction = (TargetPointActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
		SetActorRotation(NewRotation);
	}
}
