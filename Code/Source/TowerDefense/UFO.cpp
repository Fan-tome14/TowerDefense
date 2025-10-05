#include "UFO.h"
#include "EnnemyAIController.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h" // 🔹 Nécessaire pour GetActorOfClass
#include "Engine/TargetPoint.h"     // 🔹 Pour le type ATargetPoint

AUFO::AUFO()
{
	PrimaryActorTick.bCanEverTick = true;

	// Utiliser l'AIController dédié
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnnemyAIController::StaticClass();

	// Laisser le CharacterMovement gérer la rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AUFO::BeginPlay()
{
	Super::BeginPlay();

	// ✅ Récupération automatique du premier TargetPoint trouvé dans la scène
	if (!TargetPointActor)
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATargetPoint::StaticClass());
		if (FoundActor)
		{
			TargetPointActor = Cast<ATargetPoint>(FoundActor);
			UE_LOG(LogTemp, Warning, TEXT("TargetPoint trouvé : %s"), *TargetPointActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Aucun TargetPoint trouvé dans la scène pour %s"), *GetName());
		}
	}

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

	// ✅ Déplacement vers le TargetPoint via NavMesh (si trouvé)
	if (TargetPointActor)
	{
		AEnnemyAIController* AICont = Cast<AEnnemyAIController>(GetController());
		if (AICont)
		{
			AICont->MoveToActor(TargetPointActor, 5.0f); // 5.f = tolérance distance
			UE_LOG(LogTemp, Warning, TEXT("%s se déplace vers %s"), *GetName(), *TargetPointActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController non trouvé pour %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPoint non trouvé pour %s"), *GetName());
	}
}

void AUFO::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Optionnel : Rotation vers la cible (visuel uniquement)
	if (TargetPointActor)
	{
		FVector Direction = (TargetPointActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(Direction);
		SetActorRotation(NewRotation);
	}
}
