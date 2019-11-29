// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChronoSphere.generated.h"

UCLASS()
class TOPDOWNARPG_API AChronoSphere : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float SlowDownTimeInSeconds;

	UPROPERTY(EditAnywhere)
		float SpeedCoefficient = 0.5;

private:
	FTimerManager* TimerManager = nullptr;
	FTimerHandle TimerHandle;
	AActor* LastTrappedActor;

public:
	// Sets default values for this actor's properties
	AChronoSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = Visual)
		class UParticleSystemComponent* OnOverlapParticle;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void StartTimer(AActor* OtherActor);

	UFUNCTION()
		void SpeedUpActor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};