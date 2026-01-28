// Copyright (c) 2026 GregOrigin. All Rights Reserved.

#include "OmniWalkDemoGenerator.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"                 // new fix
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h" // Added for SpawnMagneSphere
#include "UObject/ConstructorHelpers.h"     // Added for FObjectFinder
#include "Kismet/KismetMathLibrary.h"

AOmniWalkDemoGenerator::AOmniWalkDemoGenerator()
{
	MeshInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshInstances"));
	SetRootComponent(MeshInstances);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		CachedCubeMesh = CubeMeshAsset.Object;
		MeshInstances->SetStaticMesh(CachedCubeMesh);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		CachedSphereMesh = SphereMeshAsset.Object;
	}

	MeshInstances->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AOmniWalkDemoGenerator::BeginPlay()
{
    Super::BeginPlay();
    GenerateSpiralPath();

    // Spawn planetoid at end of spiral
    SpawnMagneSphere(GetActorLocation() + FVector(0, 0, StepCount * 60.0f + 1000.0f), 1500.0f);
}

void AOmniWalkDemoGenerator::SpawnMagneSphere(FVector Location, float Radius)
{
    AActor* SphereActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), Location, FRotator::ZeroRotator);
    if (SphereActor)
    {
        UStaticMeshComponent* SMC = NewObject<UStaticMeshComponent>(SphereActor);

        // 2. USE CACHED POINTER AT RUNTIME (No FObjectFinder here!)
        if (SMC && CachedSphereMesh)
        {
            SMC->SetStaticMesh(CachedSphereMesh);
            SMC->RegisterComponent();
            SphereActor->SetRootComponent(SMC);
            SphereActor->SetActorScale3D(FVector(Radius / 50.0f));
        }

        SphereActor->Tags.Add(FName("OmniWalk.DemoPlanet"));
    }
}

void AOmniWalkDemoGenerator::GenerateSpiralPath()
{
	for (int32 i = 0; i < StepCount; ++i)
	{
		float Angle = i * 0.2f;
		float X = FMath::Cos(Angle) * SpiralRadius;
		float Y = FMath::Sin(Angle) * SpiralRadius;
		float Z = i * 60.0f;

		FVector Location = GetActorLocation() + FVector(X, Y, Z);
		
		// The "Magic": Calculate rotation so the surface 'Up' always faces the center
		// creating a 'wall-run' spiral.
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, GetActorLocation() + FVector(0, 0, Z));
		
		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(Rotation.Quaternion());
		Transform.SetScale3D(StepScale);

		MeshInstances->AddInstance(Transform);
	}
}
