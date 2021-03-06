// Fill out your copyright notice in the Description page of Project Settings.

#include "Aechoes.h"
#include "OverworldController.h"


AOverworldController::AOverworldController()
{
	//ShowCursor(true);
	this->bShowMouseCursor = true;
	UE_LOG(LogTemp, Warning, TEXT("Starting overworld controller"));
}

void AOverworldController::SetControl(ACombatableCharacter * character)
{
	this->CCharacter = character;
}

ACombatableCharacter *AOverworldController::GetControl()
{
	return this->CCharacter;
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

	// handle clicks
	InputComponent->BindAction("SelectClick", IE_Pressed, this, &AOverworldController::OnSelectClick);
	InputComponent->BindAction("ActionClick", IE_Pressed, this, &AOverworldController::OnActionClick);

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
void AOverworldController::OnSelectClick()
{

}

/** Handles right clicks **/
void AOverworldController::OnActionClick()
{

	if (CCharacter == nullptr || !CCharacter->IsValidLowLevel())
		return;

	//UE_LOG(LogTemp, Warning, TEXT("Clicked at pos: %f, %f, %f"), location.X, location.Y, location.Z);
	FVector loc, dir;
	FHitResult result;
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);
	loc = result.Location;
	/*this->DeprojectMousePositionToWorld(loc, dir);*/
	
	UWorldGrid *grid = ((AAechoesGameMode *)GetWorld()->GetAuthGameMode())->getGrid();
	AObstacle *out = grid->get(loc.X, loc.Y);
	if (out != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *out->GetName());
	} 
	else if (CCharacter->isCommandReady()) {

		UE_LOG(LogTemp, Warning, TEXT("Is Ready and Willing!"));

		ULrid *lrid = CCharacter->GetLrid();
		TArray<GridPosition> lridPath = lrid->GetPath(grid->ToGridPos(loc));
		if (lridPath.Num() == 0)
			return; //can't move that far

		loc = grid->snapTo(loc, true);

		FVector oldPos = CCharacter->GetActorLocation();
		grid->clear(oldPos.X, oldPos.Y);
		grid->place(loc.X, loc.Y, CCharacter);

		TArray<FVector> navPoints;
		FVector vect;
		GridPosition p;
		for (int i = lridPath.Num(); i > 0; i--) {
			p = lridPath[i-1];
			vect = grid->ToWorldPos(p, true, true);
			//vect.Z = CCharacter->GetActorLocation().Z;
			navPoints.Add(vect);
		}



		CCharacter->SetMovementPath(navPoints, true);
		//UNavigationSystem::SimpleMoveToLocation(CCharacter->GetController(), loc);
		CCharacter->SetEffectiveLocation(loc, true);

		if (NavArrow != nullptr)
			NavArrow->DestroyArrow();
	}
}

void AOverworldController::BeginPlay()
{
	NavArrow = GetWorld()->SpawnActor<ANavArrow>(ANavArrow::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
}

void AOverworldController::TickActor(float DeltaTime,
	enum ELevelTick TickType,
	FActorTickFunction & ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	static GridPosition LastMousePosition(-9999, -9999);
	static UWorldGrid *grid = nullptr;

	//if game in focus
	FVector2D pos, size;


	if (GEngine->GameViewport && GEngine->GameViewport->GetMousePosition(pos)) {

		GEngine->GameViewport->GetViewportSize(size);
		
		if (pos.X > 0 && (pos.X < 100 || pos.X > size.X - 100))
			MoveRight(pos.X < 100 ? -1 + (pos.X / 100) : 1 - ( (size.X - pos.X) / 100));

		if (pos.Y > 0 && (pos.Y < 100 || pos.Y > size.Y - 100))
			MoveUp(pos.Y < 100 ? 1 - (pos.Y / 100) : -1 + ((size.Y - pos.Y) / 100));

		if (grid == nullptr || !grid->IsValidLowLevel()) {
			grid = ((AAechoesGameMode *) this->GetWorld()->GetAuthGameMode())->getGrid(); //try to fetch
			UE_LOG(LogTemp, Warning, TEXT("Grid is null"));
		}

		if (grid != nullptr && grid->IsValidLowLevel())
		if (CCharacter != nullptr && CCharacter->IsValidLowLevelFast() && CCharacter->isCommandReady()) {
			FVector loc, dir;
			FHitResult result;
			this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, result);
			//if (result.)


			loc = result.Location;

			if (!(grid->ToGridPos(FVector(loc.X, loc.Y, 0)) == LastMousePosition)) {
				LastMousePosition = grid->ToGridPos(FVector(loc.X, loc.Y, 0));
				FVector cloc = CCharacter->GetActorLocation();
				
				int gDist = grid->GetGridDistance(loc, cloc);

				if (gDist <= CCharacter->getMP()) {
          TArray<GridPosition> points = CCharacter->GetLrid()->GetPath(LastMousePosition);
					NavArrow->UpdateTarget(points);
				}
				else {
					NavArrow->DestroyArrow();
				}
			}
		}

	}

}