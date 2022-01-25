#pragma once

namespace Features {

	struct playersESP {

		inline void InitializeRadar(SDK::UCanvas* Canvas)
		{
			Render::DrawFilledCircle(Canvas, SDK::FVector2D(width - 200, 200), 100, SDK::FLinearColor(1.275, 1.275, 1.275, 1.f), 1);
			Render::DrawCircle(Canvas, SDK::FVector2D(width - 200, 200), 100, SDK::FLinearColor(25.5, 0, 255, 1.f), 1);
			Render::DrawLine(Canvas, SDK::FVector2D(width - 200, 100), SDK::FVector2D(width - 200, 300), SDK::FLinearColor(255, 255, 255, 1.f));
			Render::DrawLine(Canvas, SDK::FVector2D(width - 100, 200), SDK::FVector2D(width - 300, 200), SDK::FLinearColor(255, 255, 255, 1.f));
		}

		inline void Box2D(SDK::UCanvas* Canvas, SDK::APlayerController* PlayerController, SDK::AFortPlayerPawn* actor, SDK::FVector2D Bottom, SDK::FVector2D Top, SDK::FLinearColor col)
		{
			SDK::FVector WorldOrigin, WorldBoxExtent;
			actor->GetActorBounds(bool(PlayerController->AcknowledgedPawn), false, &WorldOrigin, &WorldBoxExtent);

			WorldBoxExtent.X *= 2.f;
			WorldBoxExtent.Y *= 2.f;
			WorldBoxExtent.Z *= 2.f;

			WorldOrigin.X -= WorldBoxExtent.X / 2.f;
			WorldOrigin.Y -= WorldBoxExtent.Y / 2.f;
			WorldOrigin.Z -= WorldBoxExtent.Z / 2.f;

			SDK::FVector one = WorldOrigin;
			SDK::FVector two = WorldOrigin; two.X += WorldBoxExtent.X;
			SDK::FVector three = WorldOrigin; three.X += WorldBoxExtent.X; three.Y += WorldBoxExtent.Y;
			SDK::FVector four = WorldOrigin; four.Y += WorldBoxExtent.Y;

			SDK::FVector five = one; five.Z += WorldBoxExtent.Z;
			SDK::FVector six = two; six.Z += WorldBoxExtent.Z;
			SDK::FVector seven = three; seven.Z += WorldBoxExtent.Z;
			SDK::FVector eight = four; eight.Z += WorldBoxExtent.Z;

			SDK::FVector2D s1, s2, s3, s4, s5, s6, s7, s8;
			PlayerController->ProjectWorldLocationToScreen(one, false, &s1);
			PlayerController->ProjectWorldLocationToScreen(two, false, &s2);
			PlayerController->ProjectWorldLocationToScreen(three, false, &s3);
			PlayerController->ProjectWorldLocationToScreen(four, false, &s4);
			PlayerController->ProjectWorldLocationToScreen(five, false, &s5);
			PlayerController->ProjectWorldLocationToScreen(six, false, &s6);
			PlayerController->ProjectWorldLocationToScreen(seven, false, &s7);
			PlayerController->ProjectWorldLocationToScreen(eight, false, &s8);


			int x_array[8] = { s1.X, s2.X, s3.X, s4.X, s5.X, s6.X, s7.X, s8.X };
			int right = x_array[0], left = x_array[0];

			for (int right_idx = 0; right_idx < 8; right_idx++)
				if (x_array[right_idx] > right)
					right = x_array[right_idx];

			for (int left_idx = 0; left_idx < 8; left_idx++)
				if (x_array[left_idx] < left)
					left = x_array[left_idx];


			float Width = right - left;
			float Height = Bottom.Y - Top.Y;

			SDK::FVector2D min, max, size;
			min.X = left;
			min.Y = Bottom.Y;

			max.X = right;
			max.Y = Top.Y;

			size.X = min.X - max.X;
			size.Y = min.Y - max.Y;

			Render::DrawCorneredBox(Canvas, max, size, 2, col);
		}

		inline void Box3D(SDK::UCanvas* Canvas, SDK::APlayerController* PlayerController, SDK::AFortPlayerPawn* actor, SDK::FLinearColor col)
		{
			SDK::FVector WorldOrigin, WorldBoxExtent;
			actor->GetActorBounds(bool(PlayerController->AcknowledgedPawn), false, &WorldOrigin, &WorldBoxExtent);

			Render::Box3D(PlayerController, Canvas, WorldOrigin, WorldBoxExtent, col);
		}

		inline void Skeleton(SDK::UCanvas* Canvas, SDK::APlayerController* PlayerController, SDK::AFortPlayerPawn* actor, SDK::FLinearColor col)
		{
			SDK::FVector2D neck, rightChest, leftChest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftWrist,
				rightWrist, pelvis, leftAss, rightAss, leftKnee, rightKnee, leftAnkle, rightAnkle;

			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 66), false, &neck);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 8), false, &rightChest);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 37), false, &leftChest);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 38), false, &leftShoulder);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 9), false, &rightShoulder);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 94), false, &leftElbow);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 10), false, &rightElbow);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 62), false, &leftWrist);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 33), false, &rightWrist);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 2), false, &pelvis);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 76), false, &leftAss);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 69), false, &rightAss);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 77), false, &leftKnee);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 70), false, &rightKnee);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 78), false, &leftAnkle);
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 71), false, &rightAnkle);

			Render::DrawLine(Canvas, neck, leftChest, col, 1, true);
			Render::DrawLine(Canvas, neck, rightChest, col, 1, true);
			Render::DrawLine(Canvas, leftChest, leftShoulder, col, 1, true);
			Render::DrawLine(Canvas, rightChest, rightShoulder, col, 1, true);
			Render::DrawLine(Canvas, leftShoulder, leftElbow, col, 1, true);
			Render::DrawLine(Canvas, rightShoulder, rightElbow, col, 1, true);
			Render::DrawLine(Canvas, leftElbow, leftWrist, col, 1, true);
			Render::DrawLine(Canvas, rightElbow, rightWrist, col, 1, true);
			Render::DrawLine(Canvas, neck, pelvis, col, 1, true);
			Render::DrawLine(Canvas, pelvis, leftAss, col, 1, true);
			Render::DrawLine(Canvas, pelvis, rightAss, col, 1, true);
			Render::DrawLine(Canvas, leftAss, leftKnee, col, 1, true);
			Render::DrawLine(Canvas, rightAss, rightKnee, col, 1, true);
			Render::DrawLine(Canvas, leftKnee, leftAnkle, col, 1, true);
			Render::DrawLine(Canvas, rightKnee, rightAnkle, col, 1, true);
		}

		inline void Lines(SDK::UCanvas* Canvas, SDK::APlayerController* PlayerController, SDK::AFortPlayerPawn* actor, SDK::FLinearColor col)
		{
			SDK::FVector2D enemyScreen;
			PlayerController->ProjectWorldLocationToScreen(SDK::GetBone(actor, 2), false, &enemyScreen);

			Render::DrawLine(Canvas, SDK::FVector2D(width / 2, height), enemyScreen, col, 1, true);
		}

		inline void Names(SDK::UCanvas* Canvas, SDK::UFont* Font, float& TextOffset_Top, SDK::AFortPlayerPawn* actor, SDK::FVector2D Top, SDK::FLinearColor col)
		{
			auto PlayerState = actor->PlayerState;
			if (!PlayerState) return;

			auto name = PlayerState->GetPlayerName().c_str();
			if (!name) return;

			TextOffset_Top -= Canvas->K2_TextSize(Font, name, SDK::FVector2D(1.1f, 1.1f)).Y;
			SDK::FVector2D calculation = SDK::FVector2D(Top.X, TextOffset_Top);

			Render::DrawStrokeText(Canvas, Font, calculation, col, name);
		}

		inline void Weapons(SDK::UCanvas* Canvas, SDK::UFont* Font, float& TextOffset_Bottom, SDK::AFortPlayerPawn* actor, SDK::FVector2D Bottom)
		{
			auto weapon = actor->CurrentWeapon;
			if (!weapon) return;

			auto weapon_data = weapon->WeaponData;
			if (!weapon_data) return;

			auto name = weapon_data->DisplayName;
			if (!name.GetText()) return;

			auto tier = weapon_data->Tier;

			TextOffset_Bottom += Canvas->K2_TextSize(Font, name.GetText(), SDK::FVector2D(1.1f, 1.1f)).Y;
			auto calculation = SDK::FVector2D(Bottom.X, TextOffset_Bottom);

			Render::DrawStrokeText(Canvas, Font, calculation, utilities::getColorFromTier(tier), name.GetText());
		}

		inline void Distance(SDK::UCanvas* Canvas, SDK::UFont* Font, float& TextOffset_Bottom, SDK::FVector WorldHead, SDK::FVector CameraLocation, SDK::FVector2D Bottom, SDK::FLinearColor col)
		{
			int dist = (int)(utilities::custom_sqrtf(CameraLocation.Distance(WorldHead)) / 100.f);

			auto converted = utilities::custom_to_string(dist);
			auto text = std::wstring(xorstr(L"[")) + std::wstring(converted.begin(), converted.end()) + std::wstring(xorstr(L"m]"));

			TextOffset_Bottom += Canvas->K2_TextSize(Font, text.c_str(), SDK::FVector2D(1.1f, 1.1f)).Y;
			auto calculation = SDK::FVector2D(Bottom.X, TextOffset_Bottom);

			Render::DrawStrokeText(Canvas, Font, calculation, col, text.c_str());
		}

		inline void Platform(SDK::UCanvas* Canvas, SDK::UFont* Font, SDK::AFortPlayerPawn* actor, float& TextOffset_Bottom, SDK::FVector2D Bottom, SDK::FLinearColor col)
		{
			auto player_state = (SDK::AFortPlayerState*)actor->PlayerState;
			if (!player_state) return;

			auto platform = player_state->GetPlatform();
			if (!platform.IsValid()) return;

			const wchar_t* text = platform.c_str();

			TextOffset_Bottom += Canvas->K2_TextSize(Font, text, SDK::FVector2D(1.1f, 1.1f)).Y;
			auto calculation = SDK::FVector2D(Bottom.X, TextOffset_Bottom);

			if (utilities::custom_wcsstr(text, xorstr(L"PSN"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"PlayStation"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"XBL"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"Xbox"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"WIN"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"Windows"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"MAC"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"MacOS"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"LNX"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"Linux"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"IOS"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"Ios"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"AND"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"Android"));
			}
			else if (utilities::custom_wcsstr(text, xorstr(L"SWT"))) {
				Render::DrawStrokeText(Canvas, Font, calculation, col, xorstr(L"NintendoSwitch"));
			}
		}

		inline void Glow(SDK::AFortPlayerPawn* actor)
		{
			SDK::FPawnHighlight HitGlow;
			HitGlow.UsesPulse = 0;
			HitGlow.Inner = SDK::FLinearColor(60000.f, 0.f, 0.f, 255.f);
			HitGlow.Outer = SDK::FLinearColor(60000.f, 0.f, 0.f, 255.f);

			((SDK::APlayerPawn_Athena_C*)actor)->ApplyPawnHighlight(actor, HitGlow);
		}

		inline void Chams(SDK::AFortPlayerPawn* actor)
		{
			SDK::FPawnHighlight HitGlow;
			HitGlow.UsesPulse = 0;
			HitGlow.Inner = SDK::FLinearColor(255.f, 0.f, 0.f, 1.f);
			HitGlow.Outer = SDK::FLinearColor(0.f, 255.f, 0.f, 1.f);

			((SDK::APlayerPawn_Athena_C*)actor)->ApplyPawnHighlight(actor, HitGlow);
		}

		inline void Radar(SDK::UCanvas* Canvas, SDK::APlayerCameraManager* CameraManager, SDK::FVector Head, SDK::FLinearColor col)
		{
			auto localPlayerCamRot = CameraManager->GetCameraRotation();
			auto localPlayerCamLoc = CameraManager->GetCameraLocation();

			SDK::FVector2D DrawPos = SDK::FVector2D(width - 300, 100);
			SDK::FVector2D DrawSize = SDK::FVector2D(200, 200);

			auto fYaw = localPlayerCamRot.Yaw * 3.141592653589793f / 180.0f;
			float dx = Head.X - localPlayerCamLoc.X;
			float dy = Head.Y - localPlayerCamLoc.Y;

			float fsin_yaw = utilities::custom_sinf(fYaw);
			float fminus_cos_yaw = -utilities::custom_cosf(fYaw);

			float x = dy * fminus_cos_yaw + dx * fsin_yaw;
			x = -x;
			float y = dx * fminus_cos_yaw - dy * fsin_yaw;

			float range = (float)7000; //7000.f <-> 50000.f


			if (utilities::custom_fabs((x)) > range || utilities::custom_fabs((y)) > range) {
				if ((y) > (x)) {
					if ((y) > -(x)) {
						(x) = range * (x) / (y);
						(y) = range;
					}
					else {
						(y) = -range * (y) / (x);
						(x) = -range;
					}
				}
				else {
					if ((y) > -(x)) {
						(y) = range * (y) / (x);
						(x) = range;
					}
					else {
						(x) = -range * (x) / (y);
						(y) = -range;
					}
				}
			}

			int rad_x = (int)DrawPos.X;
			int rad_y = (int)DrawPos.Y;

			float r_siz_x = DrawSize.X;
			float r_siz_y = DrawSize.Y;

			int x_max = (int)r_siz_x + rad_x - 2;
			int y_max = (int)r_siz_y + rad_y - 2;

			SDK::FVector2D out;
			out.X = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
			out.Y = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

			auto difference_x = out.X - (width - 200);
			auto difference_y = out.Y - 200;
			auto distance_xy = utilities::custom_sqrtf(difference_x * difference_x + difference_y * difference_y);


			if (distance_xy <= 100)
				Render::DrawFilledRect(Canvas, SDK::FVector2D(out.X - 2, out.Y - 2), SDK::FVector2D(4, 4), col);
		}

	};


	struct exploits {

		inline void ZiplineExploit(SDK::AFortAthenaZipline* Zipline, SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, SDK::UFont* Font, bool bExploit)
		{
			if (!bExploit) return;

			if (!Zipline) return;

			auto localPawn = (SDK::AFortPlayerPawn*)PlayerController->AcknowledgedPawn;
			if (!localPawn) return;

			auto localPawnRoot = localPawn->RootComponent;
			if (!localPawnRoot) return;

			auto ziplineRoot = Zipline->RootComponent;
			if (!ziplineRoot) return;
			
			auto ziplineLoc = ziplineRoot->RelativeLocation;

			SDK::FVector2D w2ss; PlayerController->ProjectWorldLocationToScreen(ziplineLoc, false, &w2ss);
			Render::DrawStrokeText(Canvas, Font, w2ss, SDK::FLinearColor(0, 255, 0, 1.f), xorstr(L"Zipline"));

			if (PlayerController->WasInputKeyJustPressed(keybinds::F5)) //F5
			{
				localPawn->K2_TeleportTo(ziplineLoc, localPawnRoot->RelativeRotation);
				localPawn->BeginZiplining(Zipline, 0, 0);
			}
			
		}

		inline void NoVehicleCollision(SDK::AFortAthenaVehicle* Vehicle, bool bNoCollision)
		{
			if (!Vehicle) return;

			if (bNoCollision)
			{
				Vehicle->SetActorEnableCollision(false);
				DisabledSettings::Exploits->NoVehicleCollision = false;
			}
			else
			{
				if (!DisabledSettings::Exploits->NoVehicleCollision)
				{
					Vehicle->SetActorEnableCollision(true);
					DisabledSettings::Exploits->NoVehicleCollision = true;
				}
			}
		}

		inline void VehicleFly(SDK::AFortAthenaVehicle* Vehicle, SDK::APlayerController* PlayerController, bool bFly)
		{
			if (!Vehicle) return;
			
			auto RootComponent = Vehicle->RootComponent;
			if (RootComponent)
			{
				static SDK::FVector TpPos = RootComponent->RelativeLocation;

				if (bFly)
				{
					//W
					if (PlayerController->IsInputKeyDown(keybinds::W)) TpPos.X += 100;

					//S
					if (PlayerController->IsInputKeyDown(keybinds::S)) TpPos.X -= 100;

					//D
					if (PlayerController->IsInputKeyDown(keybinds::D)) TpPos.Y += 100;

					//A
					if (PlayerController->IsInputKeyDown(keybinds::A)) TpPos.Y -= 100;

					//SpaceBar
					if (PlayerController->IsInputKeyDown(keybinds::SpaceBar)) TpPos.Z += 75;

					//LeftShift
					if (PlayerController->IsInputKeyDown(keybinds::LeftShift)) TpPos.Z -= 75;

					Vehicle->K2_TeleportTo(TpPos, { 0, 0, 0 });
				}
				else
					TpPos = RootComponent->RelativeLocation;
			}
		}

	};


	inline playersESP* PlayersESP = new playersESP();
	inline exploits* Exploits = new exploits();
}