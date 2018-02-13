// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrackPiece.h"
#include "StaticGameSettings.h"


/**
 *
 */
class ELLE_API TrackGenerator
{
public:
	TrackGenerator();
	~TrackGenerator();

	TSubclassOf<ATrackPiece> NormalTrack;
	TSubclassOf<ATrackPiece> StartTrack;
	TSubclassOf<ATrackPiece> EndTrack;
	TSubclassOf<ATrackPiece> PuzzleTrack;
	TSubclassOf<ATrackPiece> ObstacleTrack;

	TArray<TArray<ATrackPiece*>> AnchorTracks;
	ATrackPiece* LastTrackPiece;
	void SpawnNextTrackChunk(UWorld* world);
	void DestroyOldestTrackChunk();
};
