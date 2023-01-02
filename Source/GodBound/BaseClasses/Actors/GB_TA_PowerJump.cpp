// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_TA_PowerJump.h"

void AGB_TA_PowerJump::Tick(float DeltaTime)
{
	if (MaxRange < MaxRangeLimit)
	{
		MaxRange += MaxRangeAdder*DeltaTime;
	}
	
	Super::Tick(DeltaTime);
}
