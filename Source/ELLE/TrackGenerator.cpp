// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "StaticGameSettings.h"
#include "TrackGenerator.h"

//TArray<FTrackPiece> TrackGenerator::Track;

TrackGenerator::TrackGenerator()
{
	LastTrackPiece = nullptr;

	FString GameModeFolder = "";
	switch (StaticGameSettings::gameType)
	{
	case EGameType::ThirdPerson_Perpendicular: GameModeFolder = "TPP";
		break;
	case EGameType::ThirdPerson_OverTheShoulder: GameModeFolder = "TPOS";
		break;
	case EGameType::FirstPerson_VR: GameModeFolder = "FPVR";
		break;
	default: GameModeFolder = "TPP";
	}

	FString s1 = "/Game/Blueprints/" + GameModeFolder + "/NormalTrack" + GameModeFolder;
	ConstructorHelpers::FClassFinder<ATrackPiece> PlatformBPClass(*s1);
	if (PlatformBPClass.Class != NULL)
	{
		NormalTrack = PlatformBPClass.Class;
	}

	FString s2 = "/Game/Blueprints/" + GameModeFolder + "/StartTrack" + GameModeFolder;
	ConstructorHelpers::FClassFinder<ATrackPiece> StartBPClass(*s2);
	if (PlatformBPClass.Class != NULL)
	{
		StartTrack = StartBPClass.Class;
	}

	FString s3 = "/Game/Blueprints/" + GameModeFolder + "/EndTrack" + GameModeFolder;
	ConstructorHelpers::FClassFinder<ATrackPiece> EndBPClass(*s3);
	if (PlatformBPClass.Class != NULL)
	{
		EndTrack = EndBPClass.Class;
	}

	FString s4 = "/Game/Blueprints/" + GameModeFolder + "/ObstacleTrack" + GameModeFolder;
	ConstructorHelpers::FClassFinder<ATrackPiece> ObstacleBPClass(*s4);
	if (ObstacleBPClass.Class != NULL)
	{
		ObstacleTrack = ObstacleBPClass.Class;
	}

	FString s5 = "/Game/Blueprints/" + GameModeFolder + "/PuzzleTrack" + GameModeFolder;
	ConstructorHelpers::FClassFinder<ATrackPiece> TextPlatformBPClass(*s5);
	if (TextPlatformBPClass.Class != NULL)
	{
		PuzzleTrack = TextPlatformBPClass.Class;
	}
}

TrackGenerator::~TrackGenerator()
{}

/// <summary>
/// Spawn the next chunk of track
/// </summary>
/// <param name="world"></param>
void TrackGenerator::SpawnNextTrackChunk(UWorld* world)
{
	if (world == nullptr)
		return;

	if (LastTrackPiece == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(world, StartTrack, FoundActors);

		if (FoundActors.Num() > 0)
		{
			LastTrackPiece = Cast<ATrackPiece>(FoundActors[0]);
		}

		if (LastTrackPiece == nullptr)
			return;
	}

	
	//Array holding Tracks
	TArray<ATrackPiece*> trackChunk;

	//spawn blank piece that will be used as anchor
	FVector vec1 = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
	ATrackPiece* pc1 = (ATrackPiece*)world->SpawnActor(NormalTrack, new FVector(vec1), new FRotator(0, 0, 0));
	LastTrackPiece = pc1;
	trackChunk.Add(pc1);

	bool SpawnedObstacle = false;
	for (int x = 0; x < StaticGameSettings::puzzleInterval; x++)
	{
		//spawn obstacle or normal piece
		if (FMath::RandRange(1, 100) < StaticGameSettings::obstacleSpawnChance * 100 && !SpawnedObstacle)
		{
			FVector vec = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
			ATrackPiece* pc = (ATrackPiece*)world->SpawnActor(ObstacleTrack, new FVector(vec), new FRotator(0, 0, 0));
			LastTrackPiece = pc;
			trackChunk.Add(pc);
			SpawnedObstacle = true;
		}
		else
		{
			FVector vec = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
			ATrackPiece* pc = (ATrackPiece*)world->SpawnActor(NormalTrack, new FVector(vec), new FRotator(0, 0, 0));
			LastTrackPiece = pc;
			trackChunk.Add(pc);
			SpawnedObstacle = false;
		}
	}


	if (StaticGameSettings::isTestMode && StaticGameSettings::selectedLanguage.TestModule.Num() <= 0)
	{
		//spawn puzzle
		FVector vec = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
		ATrackPiece* pc = (ATrackPiece*)world->SpawnActor(EndTrack, new FVector(vec), new FRotator(0, 0, 0));
		LastTrackPiece = pc;
		trackChunk.Add(pc);

		//spawn normal pieces as ending pieces
		for (int x = 0; x < 15; x++)
		{
			FVector vec2 = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
			ATrackPiece* pc2 = (ATrackPiece*)world->SpawnActor(NormalTrack, new FVector(vec2), new FRotator(0, 0, 0));
			LastTrackPiece = pc2;
			trackChunk.Add(pc2);
		}
	}
	else
	{
		//spawn puzzle
		FVector vec = LastTrackPiece->TrackAttachPoint->GetComponentLocation();
		ATrackPiece* pc = (ATrackPiece*)world->SpawnActor(PuzzleTrack, new FVector(vec), new FRotator(0, 0, 0));
		LastTrackPiece = pc;
		trackChunk.Add(pc);
	}

	//add tracks
	AnchorTracks.Add(trackChunk);
}

/// <summary>
/// Destroy the oldest existing track chunk
/// </summary>
void TrackGenerator::DestroyOldestTrackChunk()
{
	if (AnchorTracks.Num() >= 3)
	{
		TArray<ATrackPiece*> pc = AnchorTracks[0];
		AnchorTracks.RemoveAt(0);

		for (int x = 0; x < pc.Num(); x++)
		{
			pc[x]->Destroy();
		}
	}
}
