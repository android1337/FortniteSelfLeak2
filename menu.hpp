#pragma once

namespace menu
{
	inline SDK::UFont*				font;
	inline SDK::FVector2D			mousePos;
	inline SDK::FVector2D	menuPos;
	inline static SDK::FVector2D	dragPos;

	inline bool						showMenu = true;
	inline float					width = 260;
	inline float					height = 910;
	inline float					tmp_y;
	inline bool						isSliding = false;

	inline bool inArea(SDK::FVector2D area1, SDK::FVector2D area2) 
	{
		if (mousePos.X >= area1.X && mousePos.X <= area2.X && mousePos.Y >= area1.Y && mousePos.Y <= area2.Y)
			return true;
		else
			return false;
	}

	inline void Initialize(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, SDK::UFont* Font, const wchar_t* title, SDK::FVector2D pos) 
	{
		PlayerController->GetMousePosition(&mousePos.X, &mousePos.Y);
		
		if(!menuPos.X && !menuPos.Y)
			menuPos = pos;

		font = Font;
		Render::DrawFilledRect(Canvas, SDK::FVector2D(menuPos.X - 15, menuPos.Y), SDK::FVector2D(width, height), SDK::FLinearColor(1.275, 1.275, 1.275, 1.f), false);
		Render::DrawRect(Canvas, SDK::FVector2D(menuPos.X - 15, menuPos.Y), SDK::FVector2D(width, height), SDK::FLinearColor(25.5, 0.f, 255, 1.f), 1, false);
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, menuPos.Y + 5), SDK::FLinearColor(255, 255, 0, 1.f), title, false, false, false);
	
		tmp_y = menuPos.Y;
	}

	inline void End(SDK::APlayerController* PlayerController)
	{
		// input handle
		if (!isSliding && inArea(SDK::FVector2D(menuPos.X - 15, menuPos.Y), SDK::FVector2D(menuPos.X + width, menuPos.Y + height)))
		{
			if (PlayerController->IsInputKeyDown(keybinds::LeftClick)) //LeftMouseButton
			{
				if (dragPos.X == 0)
				{
					dragPos.X = (mousePos.X - width - menuPos.X);
					dragPos.Y = (mousePos.Y - height - menuPos.Y);
				}
				menuPos.X = mousePos.X - width - dragPos.X;
				menuPos.Y = mousePos.Y - height - dragPos.Y;
			}
			else
				dragPos = SDK::FVector2D(0, 0);
		}
		else
			dragPos = SDK::FVector2D(0, 0);
	}

	inline void CustomText(SDK::UCanvas* Canvas, const wchar_t* content) 
	{
		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(25.5, 0, 255, 1.f), content, false, false, false);
	}

	inline void CustomSeparator(float value = 30.f) 
	{
		tmp_y += value;
	}

	inline void CustomTab(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, const wchar_t* content, bool& config) 
	{

		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(255, 255, 255, 1.f), content, false, false, false);

		if (inArea(SDK::FVector2D(menuPos.X + 195 - 2, tmp_y - 2), SDK::FVector2D(menuPos.X + 195 + 19, tmp_y + 19)))
			if (PlayerController->WasInputKeyJustPressed(keybinds::LeftClick)) //LeftMouseButton
				config = !config;

		if (config)
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(0, 102, 0, 1.f), xorstr(L"On"), false, false, false);
		else
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(255, 25.5, 25.5, 1.f), xorstr(L"Off"), false, false, false);
	}

	inline void CustomSlider(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, const wchar_t* content, int& value, int min, int max) 
	{

		isSliding = false;

		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(255, 255, 255, 1.f), content, false, false, false);
		tmp_y += 30.f;

		SDK::FVector2D slider_size = SDK::FVector2D{ 180, 5 };

		if (inArea(SDK::FVector2D{ menuPos.X - 3.f, tmp_y - slider_size.Y * 2 }, SDK::FVector2D(menuPos.X + slider_size.X + 3.f, tmp_y + slider_size.Y * 2)))
		{
			if (PlayerController->IsInputKeyDown(keybinds::LeftClick))
			{
				value = ((mousePos.X - menuPos.X) * ((max - min) / slider_size.X)) + min;
				if (value < min) value = min;
				if (value > max) value = max;
				isSliding = true;
			}
		}

		float oneP = slider_size.X / (max - min);
		Render::DrawFilledRect(Canvas, SDK::FVector2D{ menuPos.X, tmp_y - slider_size.Y }, SDK::FVector2D(oneP * (value - min), slider_size.Y), SDK::FLinearColor(25.5, 0, 255, 1.f));
		Render::DrawFilledCircle(Canvas, SDK::FVector2D{ menuPos.X + oneP * (value - min), tmp_y - slider_size.Y + 2.5f }, 6.f, SDK::FLinearColor(255, 255, 255, 1.f));

		auto converted = utilities::custom_to_string(value);
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y - slider_size.Y - 5.f), SDK::FLinearColor(255, 255, 255, 1.f), std::wstring(converted.begin(), converted.end()).c_str(), false, false, false);
		tmp_y -= 10.f;
	}
}

namespace menu2
{
	inline SDK::UFont* font;
	inline SDK::FVector2D			mousePos;
	inline SDK::FVector2D	menuPos;
	inline static SDK::FVector2D	dragPos;

	inline bool						showMenu = true;
	inline float					width = 260;
	inline float					height = 910;
	inline float					tmp_y;
	inline bool						isSliding = false;

	inline bool inArea(SDK::FVector2D area1, SDK::FVector2D area2)
	{
		if (mousePos.X >= area1.X && mousePos.X <= area2.X && mousePos.Y >= area1.Y && mousePos.Y <= area2.Y)
			return true;
		else
			return false;
	}

	inline void Initialize(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, SDK::UFont* Font, const wchar_t* title, SDK::FVector2D pos)
	{
		PlayerController->GetMousePosition(&mousePos.X, &mousePos.Y);

		if (!menuPos.X && !menuPos.Y)
			menuPos = pos;

		font = Font;
		Render::DrawFilledRect(Canvas, SDK::FVector2D(menuPos.X - 15, menuPos.Y), SDK::FVector2D(width, height), SDK::FLinearColor(1.275, 1.275, 1.275, 1.f), false);
		Render::DrawRect(Canvas, SDK::FVector2D(menuPos.X - 15, menuPos.Y), SDK::FVector2D(width, height), SDK::FLinearColor(25.5, 0.f, 255, 1.f), 1, false);
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, menuPos.Y + 5), SDK::FLinearColor(255, 255, 0, 1.f), title, false, false, false);

		tmp_y = menuPos.Y;
	}

	inline void End(SDK::APlayerController* PlayerController)
	{

	}

	inline void CustomText(SDK::UCanvas* Canvas, const wchar_t* content)
	{
		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(25.5, 0, 255, 1.f), content, false, false, false);
	}

	inline void CustomSeparator(float value = 30.f)
	{
		tmp_y += value;
	}

	inline void CustomTab(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, const wchar_t* content, bool& config)
	{

		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(255, 255, 255, 1.f), content, false, false, false);

		if (inArea(SDK::FVector2D(menuPos.X + 195 - 2, tmp_y - 2), SDK::FVector2D(menuPos.X + 195 + 19, tmp_y + 19)))
			if (PlayerController->WasInputKeyJustPressed(keybinds::LeftClick)) //LeftMouseButton
				config = !config;

		if (config)
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(0, 102, 0, 1.f), xorstr(L"On"), false, false, false);
		else
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(255, 25.5, 25.5, 1.f), xorstr(L"Off"), false, false, false);
	}

	inline void CustomSpectate(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, const wchar_t* content, SDK::AFortPlayerPawn* current_actor, SDK::AFortPlayerPawn*& target_actor)
	{

		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(255, 255, 255, 1.f), content, false, false, false);

		if (inArea(SDK::FVector2D(menuPos.X + 195 - 2, tmp_y - 2), SDK::FVector2D(menuPos.X + 195 + 19, tmp_y + 19)))
			if (PlayerController->WasInputKeyJustPressed(keybinds::LeftClick)) //LeftMouseButton
				target_actor = current_actor;

		if (current_actor == target_actor)
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(0, 102, 0, 1.f), xorstr(L"Yes"), false, false, false);
		else
			Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y), SDK::FLinearColor(255, 25.5, 25.5, 1.f), xorstr(L"No"), false, false, false);
	}

	inline void CustomSlider(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, const wchar_t* content, int& value, int min, int max)
	{

		isSliding = false;

		tmp_y += 30.f;
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X, tmp_y), SDK::FLinearColor(255, 255, 255, 1.f), content, false, false, false);
		tmp_y += 30.f;

		SDK::FVector2D slider_size = SDK::FVector2D{ 180, 5 };

		if (inArea(SDK::FVector2D{ menuPos.X - 3.f, tmp_y - slider_size.Y * 2 }, SDK::FVector2D(menuPos.X + slider_size.X + 3.f, tmp_y + slider_size.Y * 2)))
		{
			if (PlayerController->IsInputKeyDown(keybinds::LeftClick))
			{
				value = ((mousePos.X - menuPos.X) * ((max - min) / slider_size.X)) + min;
				if (value < min) value = min;
				if (value > max) value = max;
				isSliding = true;
			}
		}

		float oneP = slider_size.X / (max - min);
		Render::DrawFilledRect(Canvas, SDK::FVector2D{ menuPos.X, tmp_y - slider_size.Y }, SDK::FVector2D(oneP * (value - min), slider_size.Y), SDK::FLinearColor(25.5, 0, 255, 1.f));
		Render::DrawFilledCircle(Canvas, SDK::FVector2D{ menuPos.X + oneP * (value - min), tmp_y - slider_size.Y + 2.5f }, 6.f, SDK::FLinearColor(255, 255, 255, 1.f));

		auto converted = utilities::custom_to_string(value);
		Render::DrawStrokeText(Canvas, font, SDK::FVector2D(menuPos.X + 195, tmp_y - slider_size.Y - 5.f), SDK::FLinearColor(255, 255, 255, 1.f), std::wstring(converted.begin(), converted.end()).c_str(), false, false, false);
		tmp_y -= 10.f;
	}
}