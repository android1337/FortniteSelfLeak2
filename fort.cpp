// child file
#pragma once
#include "fort.hpp"

// spoofing & utils
#include "xorstr.hpp"
#include "lazy.hpp"
#include "spoofcall.hpp"
#include <fstream>
// sdk
#include "SDK.hpp"

// utilities
#include "utilities.hpp"
#include "render/renderer.hpp"

// features
#include "features.hpp"

#include "minhook/include/MinHook.h"

// menu lib
#include "menu.hpp"


// main variables
void						(*o_post_render)(uintptr_t _this, SDK::UCanvas* Canvas) = nullptr;
bool						bExecuteOnce = true;
bool						Fake_Unload = false;
SDK::UFont*					RobotoFont;


// uworld
inline SDK::UWorld* GetGWorld()
{
	static uintptr_t addr = 0;
	if (!addr)
	{
		addr = utilities::scan(utilities::imageBase(), xorstr("48 89 05 ? ? ? ? 48 8B 4B 78"));
		addr = RELATIVE(addr, 7);
	}
	return (SDK::UWorld*)*(uintptr_t*)addr;
}

SDK::AFortPlayerPawn* storedActor;


namespace exploitsVars {
	SDK::AFortPawn* TargetPawn;
}

void Loop(SDK::UCanvas* Canvas)
{
	Render::DrawCircle(Canvas, SDK::FVector2D(width / 2, height / 2), Settings::Aimbot->FovCircle, SDK::FLinearColor(255, 255, 255, 1.f));





	bool bInLobby = false;

	auto GWorld = GetGWorld();
	if (!GWorld) return;

	auto OwningGameInstance = GWorld->OwningGameInstance;
	if (!OwningGameInstance) return;

	auto PlayerController = OwningGameInstance->LocalPlayers[0]->PlayerController;
	if (!PlayerController) return;

	auto CameraController = PlayerController->PlayerCameraManager;
	if (!CameraController) return;

	auto CameraLocation = CameraController->GetCameraLocation();
	auto CameraRotation = CameraController->GetCameraRotation();

	auto LocalPawn = (SDK::AFortPlayerPawnAthena*)PlayerController->AcknowledgedPawn;
	if (!LocalPawn) bInLobby = true;

	CameraController->DefaultFOV = 60.f;
	//CameraController->CameraCache.POV.DesiredFOV = 60.f;


	auto FortKismetLibrary = (SDK::UFortKismetLibrary*)SDK::UFortKismetLibrary::StaticClass();
	auto KismetStringLibrary = (SDK::UKismetStringLibrary*)SDK::UKismetStringLibrary::StaticClass();


	//Actors
	static SDK::UClass* AFortPlayerPawnAthena;
	if(!AFortPlayerPawnAthena) AFortPlayerPawnAthena = SDK::AFortPlayerPawnAthena::StaticClass();

	static SDK::UClass* AFortAthenaZipline;
	if (!AFortAthenaZipline) AFortAthenaZipline = SDK::AFortAthenaZipline::StaticClass();

	static SDK::UClass* AFortProjectileBase;
	if (!AFortProjectileBase) AFortProjectileBase = SDK::AFortProjectileBase::StaticClass();

	float closestZiplineDist = FLT_MAX;
	SDK::AFortAthenaZipline* closestZipline = nullptr;
	
	float closestActorDist = FLT_MAX;
	SDK::AFortPlayerPawn* closestActor = nullptr;


	//Radar
	if (Settings::PlayersESP->Radar && !bInLobby) Features::PlayersESP->InitializeRadar(Canvas);


	auto levels = GWorld->Levels;

	for (int a = 0; a < levels.Num(); a++)
	{
		auto level = levels[a];
		if (!level) continue;

		auto actors = utilities::read<SDK::TArray<SDK::AActor*>>((uintptr_t)level + 0x98);

		for (int i = 0; i < actors.Num(); i++)
		{
			auto current = actors[i];
			if (!current || current == LocalPawn)
				continue;



			if (LocalPawn && closestActor)
			{
				if (current->IsA(AFortProjectileBase))
				{
					if (utilities::custom_strstr(current->GetName().c_str(), xorstr("B_Prj_Bullet_Sniper")))
					{
						SDK::FHitResult tmp_;
						current->K2_SetActorLocation(SDK::GetBone(closestActor, 68), false, false, &tmp_);
					}
				}
			}

			if (Settings::Exploits->ZiplineTeleport && current->IsA(AFortAthenaZipline))
			{
				auto actor = (SDK::AFortAthenaZipline*)current;
				auto roocomp = actor->RootComponent;
				if (roocomp)
				{
					auto relloc = roocomp->RelativeLocation;

					SDK::FVector2D w2ss; PlayerController->ProjectWorldLocationToScreen(relloc, false, &w2ss);
					Render::DrawStrokeText(Canvas, RobotoFont, w2ss, SDK::FLinearColor(255, 0, 0, 1.f), xorstr(L"Zipline"));


					auto dx = w2ss.X - (width / 2);
					auto dy = w2ss.Y - (height / 2);
					auto dist = utilities::custom_sqrtf(dx * dx + dy * dy);

					if (dist < Settings::Aimbot->FovCircle && dist < closestZiplineDist) {
						closestZiplineDist = dist;
						closestZipline = actor;
					}


				}
			}

			if (current->IsA(AFortPlayerPawnAthena))
			{
				// actor
				auto actor = (SDK::AFortPlayerPawn*)current;
				if (actor->IsDead()) continue;

				// visible check
				auto bIsVisible = actor->WasRecentlyRendered(0.1);

				// team check
				auto bIsFriend = !bInLobby ? FortKismetLibrary->STATIC_OnSameTeam(LocalPawn, actor) : false;

				// world positions
				auto WorldHead = SDK::GetBone(actor, 68);
				auto WorldBottom = SDK::GetBone(actor, 0);

				// screen positions
				SDK::FVector2D head; PlayerController->ProjectWorldLocationToScreen(WorldHead, false, &head);
				SDK::FVector2D Top; PlayerController->ProjectWorldLocationToScreen({ WorldHead.X, WorldHead.Y, WorldHead.Z + 20 }, false, &Top);
				SDK::FVector2D Bottom; PlayerController->ProjectWorldLocationToScreen(WorldBottom, false, &Bottom);
				float TextOffset_Top = Top.Y + 5, TextOffset_Bottom = Bottom.Y;

				// colours
				auto col = SDK::FLinearColor(255, 0, 0, 1.f);
				if (bIsVisible) col = SDK::FLinearColor(255, 60, 0, 1.f);
				if (bIsFriend) col = SDK::FLinearColor(0, 255, 0, 1.f);

				if (LocalPawn)
				{
					auto dx = head.X - (width / 2);
					auto dy = head.Y - (height / 2);
					auto dist = utilities::custom_sqrtf(dx * dx + dy * dy);

					if (dist < Settings::Aimbot->FovCircle && dist < closestActorDist) {
						closestActorDist = dist;
						closestActor = actor;
					}
				}

				// players esp
				if (Settings::PlayersESP->Box2D) Features::PlayersESP->Box2D(Canvas, PlayerController, actor, Bottom, Top, col);

				if (Settings::PlayersESP->Box3D) Features::PlayersESP->Box3D(Canvas, PlayerController, actor, col);

				if (Settings::PlayersESP->Skeleton) Features::PlayersESP->Skeleton(Canvas, PlayerController, actor, col);

				if (Settings::PlayersESP->Lines) Features::PlayersESP->Lines(Canvas, PlayerController, actor, col);

				if (Settings::PlayersESP->Names) Features::PlayersESP->Names(Canvas, RobotoFont, TextOffset_Top, actor, Top, col);

				if (Settings::PlayersESP->Weapons) Features::PlayersESP->Weapons(Canvas, RobotoFont, TextOffset_Bottom, actor, Bottom);

				if (Settings::PlayersESP->Distance) Features::PlayersESP->Distance(Canvas, RobotoFont, TextOffset_Bottom, WorldHead, CameraLocation, Bottom, col);

				if (Settings::PlayersESP->Platform) Features::PlayersESP->Platform(Canvas, RobotoFont, actor, TextOffset_Bottom, Bottom, col);

				if (Settings::PlayersESP->Glow) Features::PlayersESP->Glow(actor);

				if (Settings::PlayersESP->Chams) Features::PlayersESP->Chams(actor);

				if (Settings::PlayersESP->Radar && !bInLobby) Features::PlayersESP->Radar(Canvas, CameraController, WorldHead, col);
			}
		}
	}



	// exploits
	if (!LocalPawn) return;

	// aimbot
	if (closestActor)
		exploitsVars::TargetPawn = closestActor;
	else
		exploitsVars::TargetPawn = 0;

	
	
	if (GetGWorld()->OwningGameInstance->LocalPlayers[0]->PlayerController->IsInputKeyDown(keybinds::Tab))
	{
		SDK::FRotator aimto;
		aimto.Yaw = rand();
		PlayerController->SetControlRotation(aimto);
	}
	

	// ziplines
	Features::Exploits->ZiplineExploit(closestZipline, PlayerController, Canvas, RobotoFont, Settings::Exploits->ZiplineTeleport);

	// vehicles
	auto CurrentVehicle = LocalPawn->GetVehicle();
	Features::Exploits->NoVehicleCollision(CurrentVehicle, Settings::Exploits->NoVehicleCollision);
	Features::Exploits->VehicleFly(CurrentVehicle, PlayerController, Settings::Exploits->VehicleFly);

	if (Settings::Exploits->InfiniteJump)
	{
		auto RootComponent = LocalPawn->RootComponent;

		if (RootComponent)
		{
			SDK::FZiplinePawnState ZiplinePawnState = LocalPawn->ZiplineState;
			ZiplinePawnState.bIsZiplining = true;

			LocalPawn->ZiplineState = ZiplinePawnState;
			LocalPawn->bADSWhileNotOnGround = true;


			if (PlayerController->IsInputKeyDown(keybinds::F6)) { //F6
				//dont mind that shit was just testing :)
				INPUT space = { 0 };
				space.type = INPUT_KEYBOARD;
				space.ki.wVk = VK_SPACE;
				lazy(SendInput).safe_cached()(1, &space, sizeof(INPUT)); // Send KeyDown
				space.ki.dwFlags = KEYEVENTF_KEYUP;
				lazy(SendInput).safe_cached()(1, &space, sizeof(INPUT)); // Send KeyUp
				
			}
		}
	}

	if (Settings::Exploits->Spectate)
	{
		
		SDK::FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 1.f;
		TransitionParams.BlendExp = 2.f;
		TransitionParams.BlendFunction = SDK::EViewTargetBlendFunction::VTBlend_EaseIn;
		TransitionParams.bLockOutgoing = true;

		if (storedActor == LocalPawn)
		{
			PlayerController->ClientSetViewTarget(LocalPawn, TransitionParams);
			PlayerController->ClientSetCameraMode(KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"Default")));
		}
		else if (storedActor)
		{
			PlayerController->ClientSetViewTarget(storedActor, TransitionParams);
			PlayerController->ClientSetCameraMode(KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"ThirdPerson")));
		}
		else
		{
			PlayerController->ClientSetViewTarget(LocalPawn, TransitionParams);
			PlayerController->ClientSetCameraMode(KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"Default")));
		}
		
	}

}


void post_render(uintptr_t _this, SDK::UCanvas* Canvas)
{
	if (!Canvas) return SpoofCall(o_post_render, _this, Canvas);
	width = Canvas->SizeX, height = Canvas->SizeY;

	if (Fake_Unload)
		return SpoofCall(o_post_render, _this, Canvas);

	if (bExecuteOnce)
	{
		RobotoFont = SDK::UObject::FindObject<SDK::UFont>(xorstr("Font Roboto.Roboto"));	
		bExecuteOnce = false;
	}
	
	// actorloop
	Loop(Canvas);


	// handle input
	auto PlayerController = GetGWorld()->OwningGameInstance->LocalPlayers[0]->PlayerController;
	if (PlayerController->WasInputKeyJustPressed(keybinds::Insert)) // insert
		menu::showMenu = !menu::showMenu;

	auto LocalPawn = PlayerController->AcknowledgedPawn;

	// show menu
	if (menu::showMenu)
	{
		if (Settings::Exploits->Spectate && LocalPawn)
		{
			menu2::dragPos = { menu::dragPos.X + 270, menu::dragPos.Y };
			menu2::menuPos = { menu::menuPos.X + 270, menu::menuPos.Y };
			menu2::Initialize(PlayerController, Canvas, RobotoFont, xorstr(L"Spectate Menu"), { 320, 50 });

			auto FortKismetLibrary = (SDK::UFortKismetLibrary*)SDK::UFortKismetLibrary::StaticClass();

			auto AllPlayerPawns = FortKismetLibrary->STATIC_GetFortPlayerPawns(GetGWorld());

			for (int i = 0; i < AllPlayerPawns.Num(); i++)
			{
				auto actor = (SDK::AFortPlayerPawn*)AllPlayerPawns[i];
				if (actor)
				{
					auto PlayerState = actor->PlayerState;
					if (PlayerState)
					{
						auto name = PlayerState->GetPlayerName().c_str();
						if (name)
						{
							if(actor == LocalPawn)
								menu2::CustomSpectate(PlayerController, Canvas, (std::wstring(xorstr(L"LocalPlayer / ")) + std::wstring(name) + xorstr(L": ")).c_str(), 0, storedActor);
							else
								menu2::CustomSpectate(PlayerController, Canvas, (std::wstring(name) + xorstr(L": ")).c_str(), actor, storedActor);
						}
					}
				}
			}

			menu2::End(PlayerController);

		}
		else
			storedActor = 0;

		menu::Initialize(PlayerController, Canvas, RobotoFont, xorstr(L"testinocarino by android#1337"), { 50, 50 });

		// players esp
		menu::CustomSeparator(25.f);
		menu::CustomText(Canvas, xorstr(L"Players ESP"));
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Box2D: "), Settings::PlayersESP->Box2D);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Box3D: "), Settings::PlayersESP->Box3D);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Skeleton: "), Settings::PlayersESP->Skeleton);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Lines: "), Settings::PlayersESP->Lines);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Names: "), Settings::PlayersESP->Names);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Weapons: "), Settings::PlayersESP->Weapons);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Distance: "), Settings::PlayersESP->Distance);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Platform: "), Settings::PlayersESP->Platform);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Chams: "), Settings::PlayersESP->Chams);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Glow: "), Settings::PlayersESP->Glow);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Radar: "), Settings::PlayersESP->Radar);

		// aimbot
		menu::CustomSeparator(25.f);
		menu::CustomText(Canvas, xorstr(L"Aimbot"));
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Memory: "), Settings::Aimbot->Memory);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Sniper Prediction (non l'ho ancora messa)"), Settings::Aimbot->SniperPrediction);
		menu::CustomSlider(PlayerController, Canvas, xorstr(L"Fov Circle:"), Settings::Aimbot->FovCircle, 1, 1000);

		// exploits
		menu::CustomSeparator(25.f);
		menu::CustomText(Canvas, xorstr(L"Exploits"));
		menu::CustomTab(PlayerController, Canvas, xorstr(L"NoVehicleCollision: "), Settings::Exploits->NoVehicleCollision);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"VehicleFly: "), Settings::Exploits->VehicleFly);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"ZiplineTeleport (F5): "), Settings::Exploits->ZiplineTeleport);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Infinite Jump: "), Settings::Exploits->InfiniteJump);
		menu::CustomTab(PlayerController, Canvas, xorstr(L"Sepectate Test: "), Settings::Exploits->Spectate);


		Render::DrawFilledRect(Canvas, SDK::FVector2D(menu::mousePos.X - 3, menu::mousePos.Y - 3), SDK::FVector2D(6, 6), SDK::FLinearColor(255, 255, 255, 1.f), false);
		menu::End(PlayerController);
	}

	if (PlayerController->WasInputKeyJustPressed(keybinds::F7)) { //F7
		Fake_Unload = true;
	}
}



namespace exploitsFunctions {



	void(*o_GetViewPoint)(SDK::ULocalPlayer*, SDK::FMinimalViewInfo*, BYTE) = nullptr;
	void hk_GetViewPoint(SDK::ULocalPlayer* this_LocalPlayer, SDK::FMinimalViewInfo* OutViewInfo, BYTE StereoPass)
	{
		if (Fake_Unload) {
			return o_GetViewPoint(this_LocalPlayer, OutViewInfo, StereoPass);
		}

		o_GetViewPoint(this_LocalPlayer, OutViewInfo, StereoPass);


		//static SDK::UClass* AFortProjectileBase;
		//if (!AFortProjectileBase) AFortProjectileBase = SDK::AFortProjectileBase::StaticClass();


		auto playercontroller = this_LocalPlayer->PlayerController;
		auto local_pawn = playercontroller->AcknowledgedPawn;
		if (!local_pawn) return;
		
		//testing shits
		/*
		auto levels = GetGWorld()->Levels;

		for (int a = 0; a < levels.Num(); a++)
		{
			auto level = levels[a];
			if (!level) continue;

			auto actors = utilities::read<SDK::TArray<SDK::AActor*>>((uintptr_t)level + 0x98);

			for (int i = 0; i < actors.Num(); i++)
			{
				auto current = actors[i];
				if (!current || current == local_pawn)
					continue;

				if (local_pawn)
				{
					if (current->IsA(AFortProjectileBase))
					{
						if (utilities::custom_strstr(current->GetName().c_str(), xorstr("B_Prj_Bullet_Sniper")))
						{
							OutViewInfo->Location = current->RootComponent->RelativeLocation;
						}
					}
				}
			}
		}
		*/
		
		/*
		if (GetAsyncKeyState(VK_F4)) {
			auto rn_loc = local_pawn->RootComponent->RelativeLocation;
			rn_loc.Z += 1000;
			OutViewInfo->Location = rn_loc;

			auto rn_rot = OutViewInfo->Rotation;
			rn_rot.Pitch = -90;
			OutViewInfo->Rotation = rn_rot;
			//OutViewInfo->FOV = 100.f;
		}
		*/
	}


	SDK::FVector calculate_prediction(SDK::AFortPawn* LocalPawn, SDK::AFortPawn* Target)
	{
		//Get the enemy location
		auto targetPosition = SDK::GetBone(Target, 68);

		//Get the local location
		auto localPosition = SDK::GetBone(Target, 68);

		//Calculate the difference
		SDK::FVector Difference = targetPosition;
		Difference.X -= localPosition.X;
		Difference.Y -= localPosition.Y;
		Difference.Z -= localPosition.Z;

		//Calculate the distance
		float dist = utilities::custom_sqrtf(Difference.X * Difference.X + Difference.Y * Difference.Y + Difference.Z * Difference.Z);

		//Calculate the Scale Value
		auto PlayerState = LocalPawn->PlayerState;
		if (PlayerState)
		{
			auto Ping = PlayerState->CompressedPing * 4;
			auto ScaleValue = float(Ping) + float(dist * 0.0193f);
			if (ScaleValue < 1.0f) ScaleValue = 1.0;

			//Get the enemy velocity
			auto Velocity = Target->GetVelocity();

			//Calculate the velocity
			float fVelocity = utilities::custom_sqrtf(Velocity.X * Velocity.X + Velocity.Y * Velocity.Y + Velocity.Z * Velocity.Z);

			if (fVelocity > 100.0f)
			{
				Velocity.X *= ScaleValue / fVelocity; Velocity.Y *= ScaleValue / fVelocity; Velocity.Z *= ScaleValue / fVelocity;

				//Add the calculated values to the original target location
				targetPosition.X += Velocity.X;
				targetPosition.Y += Velocity.Y;
				targetPosition.Z += Velocity.Z;
			}
		}

		return targetPosition;

	}



	void(*o_GetPlayerViewPoint)(SDK::APlayerController*, SDK::FVector*, SDK::FRotator*) = nullptr;
	void hk_GetPlayerViewPoint(SDK::APlayerController* player_controller, SDK::FVector* Location, SDK::FRotator* Rotation)
	{
		o_GetPlayerViewPoint(player_controller, Location, Rotation);
		
		if (Fake_Unload)
			return;

		if (Settings::Aimbot->Memory)
		{
			//player_controller->FOV(120.f);

			auto LocalPawn = player_controller->AcknowledgedPawn;
			if (LocalPawn)
			{
				if (player_controller->IsInputKeyDown(keybinds::RightClick)) //RightMouseButton
				{
					if (exploitsVars::TargetPawn)
					{
						auto rootHead = SDK::GetBone(exploitsVars::TargetPawn, 68);
						SDK::FVector camloc = *Location;

						if (Settings::Aimbot->SniperPrediction)
						{
							rootHead = calculate_prediction((SDK::AFortPawn*)LocalPawn, exploitsVars::TargetPawn);
						}

						SDK::FVector VectorPos;
						VectorPos.X = rootHead.X - camloc.X;
						VectorPos.Y = rootHead.Y - camloc.Y;
						VectorPos.Z = rootHead.Z - camloc.Z;

						float distance = (double)(utilities::custom_sqrtf(VectorPos.X * VectorPos.X + VectorPos.Y * VectorPos.Y + VectorPos.Z * VectorPos.Z));

						SDK::FRotator rot;
						rot.Pitch = -((utilities::custom_acosf(VectorPos.Z / distance) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510)) - 90.f);
						rot.Yaw = utilities::custom_atan2f(VectorPos.Y, VectorPos.X) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510);
						rot.Roll = 0;

						*Rotation = rot;
					}
				}
			}
		}
	}
}

//detectable softaim
static uintptr_t SpreadCaller = 0;
static BOOL(*SpreadOriginal)(uintptr_t a1, float* a2, float* a3);
static BOOL SpreadHook(uintptr_t a1, float* a2, float* a3)
{
	if (Settings::Aimbot->Memory && _ReturnAddress() == (void*)SpreadCaller && GetGWorld()->OwningGameInstance->LocalPlayers[0]->PlayerController->IsInputKeyDown(keybinds::RightClick)) {
		return 0;
	}

	return SpreadOriginal(a1, a2, a3);
}

void fort::init()
{
	MessageBoxA(0, "DINGUS", 0, 0);
	// getnamebyindex
	SDK::Address_GetNameByIndex = utilities::scan(utilities::imageBase(), xorstr("48 89 5C 24 20 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 30 08 00 00"));
	MessageBoxA(0, std::to_string(SDK::Address_GetNameByIndex).c_str(), 0, 0);

	// gobjects
	auto GObject_address = utilities::scan(utilities::imageBase(), xorstr("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1"));
	SDK::Address_GObjects = decltype(SDK::Address_GObjects)(RELATIVE(GObject_address, 7));

	// bonematrix
	auto GetBoneMatrix_address = utilities::scan(utilities::imageBase(), xorstr("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"));
	SDK::Address_GetBoneMatrix = decltype(SDK::Address_GetBoneMatrix)(RELATIVE(GetBoneMatrix_address, 5));


	auto KismetStringLibrary = (SDK::UKismetStringLibrary*)SDK::UKismetStringLibrary::StaticClass();
	keybinds::Insert = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"Insert")) }, 0 };
	keybinds::Tab = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"Tab")) }, 0 };
	keybinds::LeftClick = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"LeftMouseButton")) }, 0 };
	keybinds::RightClick = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"RightMouseButton")) }, 0 };
	keybinds::F1 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F1")) }, 0 };
	keybinds::F2 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F2")) }, 0 };
	keybinds::F3 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F3")) }, 0 };
	keybinds::F4 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F4")) }, 0 };
	keybinds::F5 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F5")) }, 0 };
	keybinds::F6 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F6")) }, 0 };
	keybinds::F7 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F7")) }, 0 };
	keybinds::F8 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F8")) }, 0 };
	keybinds::F9 = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"F9")) }, 0 };
	keybinds::W = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"W")) }, 0 };
	keybinds::A = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"A")) }, 0 };
	keybinds::S = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"S")) }, 0 };
	keybinds::D = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"D")) }, 0 };
	keybinds::SpaceBar = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"SpaceBar")) }, 0 };
	keybinds::LeftShift = SDK::FKey{ SDK::FName{ KismetStringLibrary->STATIC_Conv_StringToName(xorstr(L"LeftShift")) }, 0 };

	SpreadCaller = utilities::scan(utilities::imageBase(), xorstr("0F 57 D2 48 8D 4C 24 ? 41 0F 28 CC E8 ? ? ? ? 48 8B 4D B0 0F 28 F0 48 85 C9"));
	auto SpreadAddr = utilities::scan(utilities::imageBase(), xorstr("E8 ? ? ? ? 48 8D 4B 28 E8 ? ? ? ? 48 8B C8"));
	SpreadAddr = RELATIVE(SpreadAddr, 5);
	MH_CreateHook((PVOID)SpreadAddr, SpreadHook, reinterpret_cast<PVOID*>(&SpreadOriginal));
	MH_EnableHook((PVOID)SpreadAddr);



	MH_Initialize();
	auto LocalPlayer = GetGWorld()->OwningGameInstance->LocalPlayers[0];

	//yes I wont release my hook way. ps you can swap vtable I just release it with minhook so if you try to sell it have fun :)
	//GetViewPoint
	auto VFTable = *(uintptr_t*)(uintptr_t(LocalPlayer));
	MH_CreateHook(*(PVOID*)(VFTable + 0x2B0), exploitsFunctions::hk_GetViewPoint, reinterpret_cast<PVOID*>(&exploitsFunctions::o_GetViewPoint));
	MH_EnableHook(*(PVOID*)(VFTable + 0x2B0));

	//GetPlayerViewPoint
	VFTable = *(uintptr_t*)(uintptr_t(LocalPlayer->PlayerController));
	MH_CreateHook(*(PVOID*)(VFTable + 0x788), exploitsFunctions::hk_GetPlayerViewPoint, reinterpret_cast<PVOID*>(&exploitsFunctions::o_GetPlayerViewPoint));
	MH_EnableHook(*(PVOID*)(VFTable + 0x788));


	// viewport
	auto ViewportClient = LocalPlayer->ViewportClient;
	if (!ViewportClient) return;

	// postrender table (detectable softaim)
	void** PostRender_VTable = *(void***)(ViewportClient);
	DWORD OldProtection;
	lazy(VirtualProtect)(&PostRender_VTable[107], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
	o_post_render = decltype(o_post_render)(PostRender_VTable[107]);
	PostRender_VTable[107] = &post_render;
	lazy(VirtualProtect)(&PostRender_VTable[107], 8, OldProtection, &OldProtection);

	return;
}