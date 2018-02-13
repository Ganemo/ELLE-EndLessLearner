// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <iostream>
#include <fstream>

using namespace std;

/**
 * 
 */
class ELLE_API RunRecorderHandler
{
public:
	RunRecorderHandler();
	~RunRecorderHandler();

	ofstream* runFile;

	void OpenRunFile(FString fileName, FString gameType, FString difficulty, FString language);
	void RecordLine(FString line);
	void CloseRunFile();

	TArray<struct FPuzzleObject> puzzles;
	void AddPuzzle(bool correctlysolved, FString focus, TArray<FString> options, enum EPuzzleType type);

	int score = 0;
private:
	bool fileOpen = false;

	FString path;
};
