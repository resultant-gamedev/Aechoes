// Fill out your copyright notice in the Description page of Project Settings.

#include "Aechoes.h"
#include "OverworldController.h"


AOverworldController::AOverworldController()
{
	ShowCursor(true);
}


void AOverworldController::SetupInputComponent()
{

	Super::SetupInputComponent();

	// Set up gameplay key bindings
	check(InputComponent);
	//InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindAxis("MoveForward", this, &AAechoesCharacter::MoveForward);
	//InputComponent->BindAxis("MoveRight", this, &AAechoesCharacter::MoveRight);

	//// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	//// "turn" handles devices that provide an absolute delta, such as a mouse.
	//// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &AAechoesCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &AAechoesCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AOverworldController::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AOverworldController::TouchStopped);

	//MoveUp, MoveRight, *Rate
	//Also SelectClick, ActionClick
	//InputComponent->BindAction("MoveUp", IE_Pressed, this, &AOverworldCamera::MoveUp);
	//InputComponent->BindAction("MoveRight", IE_Pressed, this, &AOverworldCamera::MoveRight);

	InputComponent->BindAxis("MoveUpRate", this, &AOverworldController::MoveUpRate);
	InputComponent->BindAxis("MoveRightRate", this, &AOverworldController::MoveRightRate);
	InputComponent->BindAxis("MoveUp", this, &AOverworldController::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AOverworldController::MoveRight);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &AOverworldController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &AOverworldController::ZoomOut);
}

void AOverworldController::MoveUp(float Rate)
{

	if (Rate == 0.0f)
		return;

	((AAechoesGameMode *) this->GetWorld()->GetAuthGameMode())->getCamera()
		->MoveUp(Rate);
	


}

void AOverworldController::MoveRightRate(float Rate)
{
	MoveRight(Rate);
}

void AOverworldController::MoveRight(float Rate)
{


	if (Rate == 0.0f)
		return;

	((AAechoesGameMode *) this->GetWorld()->GetAuthGameMode())->getCamera()
		->MoveRight(Rate);

}

void AOverworldController::MoveUpRate(float Rate)
{
	if (Rate != 0.0f) {
		MoveUp(Rate);
	}
}

void AOverworldController::ZoomIn()
{
	((AAechoesGameMode *) this->GetWorld()->GetAuthGameMode())->getCamera()
		->ZoomIn();
}

void AOverworldController::ZoomOut()
{
	((AAechoesGameMode *) this->GetWorld()->GetAuthGameMode())->getCamera()
		->ZoomOut();
}

/** Handler for when a touch input begins. */
void AOverworldController::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{

}

/** Handler for when a touch input stops. */
void AOverworldController::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{

}

/** Handles left clicks **/
void AOverworldController::OnSelectClick(FVector location)
{

}

/** Handles right clicks **/
void AOverworldController::OnActionClick(FVector location)
{

}