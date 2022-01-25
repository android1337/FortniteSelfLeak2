#pragma once

namespace Render
{
	void DrawLine(SDK::UCanvas* Canvas, SDK::FVector2D position1, SDK::FVector2D position2, SDK::FLinearColor color, int thickness = 1, bool screenCheck = false)
	{
		if (screenCheck && utilities::IsInScreen(position1) && utilities::IsInScreen(position2))
			Canvas->K2_DrawLine(position1, position2, thickness, color);
		else if (!screenCheck)
			Canvas->K2_DrawLine(position1, position2, thickness, color);
	}

	void DrawRect(SDK::UCanvas* Canvas, SDK::FVector2D position, SDK::FVector2D size, SDK::FLinearColor color, int thickness = 1, bool screenCheck = true)
	{
		if (screenCheck && utilities::IsInScreen(position))
			Canvas->K2_DrawBox(position, size, thickness, color);
		else if (!screenCheck)
			Canvas->K2_DrawBox(position, size, thickness, color);
	}

	void DrawCircle(SDK::UCanvas* Canvas, SDK::FVector2D position, int radius, SDK::FLinearColor color, int thickness = 1, int numSides = 450, bool screenCheck = true)
	{
		if (screenCheck && utilities::IsInScreen(position))
		{
			float Step = 3.14159265358979323846264338327950288419716939937510 * 2.0 / numSides;
			int Count = 0;
			SDK::FVector2D V[128];
			for (float a = 0; a < 3.14159265358979323846264338327950288419716939937510 * 2.0; a += Step) {
				float X1 = radius * cos(a) + position.X;
				float Y1 = radius * sin(a) + position.Y;
				float X2 = radius * cos(a + Step) + position.X;
				float Y2 = radius * sin(a + Step) + position.Y;
				V[Count].X = X1;
				V[Count].Y = Y1;
				V[Count + 1].X = X2;
				V[Count + 1].Y = Y2;
				Render::DrawLine(Canvas, SDK::FVector2D(V[Count].X, V[Count].Y), SDK::FVector2D(X2, Y2), color, thickness);
			}
		}
		else if (!screenCheck)
		{
			float Step = 3.14159265358979323846264338327950288419716939937510 * 2.0 / numSides;
			int Count = 0;
			SDK::FVector2D V[128];
			for (float a = 0; a < 3.14159265358979323846264338327950288419716939937510 * 2.0; a += Step) {
				float X1 = radius * cos(a) + position.X;
				float Y1 = radius * sin(a) + position.Y;
				float X2 = radius * cos(a + Step) + position.X;
				float Y2 = radius * sin(a + Step) + position.Y;
				V[Count].X = X1;
				V[Count].Y = Y1;
				V[Count + 1].X = X2;
				V[Count + 1].Y = Y2;
				Render::DrawLine(Canvas, SDK::FVector2D(V[Count].X, V[Count].Y), SDK::FVector2D(X2, Y2), color, thickness);
			}

		}
	}

	void DrawFilledCircle(SDK::UCanvas* Canvas, SDK::FVector2D position, float radius, SDK::FLinearColor color, bool screenCheck = true)
	{
		if (screenCheck && utilities::IsInScreen(position))
		{
			float smooth = 0.007f;

			double PI = 3.14159265359;
			int size = (int)(2.0f * PI / smooth) + 1;

			float angle = 0.0f;
			int i = 0;

			for (; angle < 2 * PI; angle += smooth, i++)
			{
				Render::DrawLine(Canvas, SDK::FVector2D(position.X, position.Y), SDK::FVector2D(position.X + utilities::custom_cosf(angle) * radius, position.Y + utilities::custom_sinf(angle) * radius), color, 1);
			}
		}
		else if (!screenCheck)
		{
			float smooth = 0.07f;

			double PI = 3.14159265359;
			int size = (int)(2.0f * PI / smooth) + 1;

			float angle = 0.0f;
			int i = 0;

			for (; angle < 2 * PI; angle += smooth, i++)
			{
				Render::DrawLine(Canvas, SDK::FVector2D(position.X, position.Y), SDK::FVector2D(position.X + utilities::custom_cosf(angle) * radius, position.Y + utilities::custom_sinf(angle) * radius), color, 1);
			}
		}
	}

	void DrawFilledRect(SDK::UCanvas* Canvas, SDK::FVector2D position, SDK::FVector2D size, SDK::FLinearColor color, bool screenCheck = true)
	{
		if (screenCheck && utilities::IsInScreen(position))
		{
			for (float i = 0.0f; i < size.Y; i += 1.0f)
				Render::DrawLine(Canvas, SDK::FVector2D(position.X, position.Y + i), SDK::FVector2D(position.X + size.X, position.Y + i), color, 1);
		}
		else if (!screenCheck)
		{
			for (float i = 0.0f; i < size.Y; i += 1.0f)
				Render::DrawLine(Canvas, SDK::FVector2D(position.X, position.Y + i), SDK::FVector2D(position.X + size.X, position.Y + i), color, 1);
		}
	}

	void DrawStrokeText(SDK::UCanvas* Canvas, SDK::UFont* Font, SDK::FVector2D position, SDK::FLinearColor color, const wchar_t* content, bool bCentered = true, bool screenCheck = true, bool bOutlined = true)
	{
		if (screenCheck && utilities::IsInScreen(position))
			Canvas->K2_DrawText(Font, content, position, SDK::FVector2D(1.1f, 1.1f), color, 0.0f, SDK::FLinearColor(), position, bCentered, bCentered, bOutlined, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		else if (!screenCheck)
			Canvas->K2_DrawText(Font, content, position, SDK::FVector2D(1.1f, 1.1f), color, 0.0f, SDK::FLinearColor(), position, bCentered, bCentered, bOutlined, SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void DrawCorneredBox(SDK::UCanvas* Canvas, SDK::FVector2D position, SDK::FVector2D size, int thickness, SDK::FLinearColor color) {

		if (!utilities::IsInScreen(position)) return;

		float lineW = (size.X / 5);
		float lineH = (size.Y / 4);

		//corners
		DrawLine(Canvas, SDK::FVector2D(position.X, position.Y), SDK::FVector2D(position.X, position.Y + lineH), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X, position.Y), SDK::FVector2D(position.X + lineW, position.Y), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X + size.X - lineW, position.Y), SDK::FVector2D(position.X + size.X, position.Y), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X + size.X, position.Y), SDK::FVector2D(position.X + size.X, position.Y + lineH), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X, position.Y + size.Y - lineH), SDK::FVector2D(position.X, position.Y + size.Y), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X, position.Y + size.Y), SDK::FVector2D(position.X + lineW, position.Y + size.Y), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X + size.X - lineW, position.Y + size.Y), SDK::FVector2D(position.X + size.X, position.Y + size.Y), color, thickness);
		DrawLine(Canvas, SDK::FVector2D(position.X + size.X, position.Y + size.Y - lineH), SDK::FVector2D(position.X + size.X, position.Y + size.Y), color, thickness);
	}

	void Box3D(SDK::APlayerController* PlayerController, SDK::UCanvas* Canvas, SDK::FVector origin, SDK::FVector extends, SDK::FLinearColor col)
	{
		extends.X *= 2.f;
		extends.Y *= 2.f;
		extends.Z *= 2.f;

		origin.X -= extends.X / 2.f;
		origin.Y -= extends.Y / 2.f;
		origin.Z -= extends.Z / 2.f;

		SDK::FVector one = origin;
		SDK::FVector two = origin; two.X += extends.X;
		SDK::FVector three = origin; three.X += extends.X; three.Y += extends.Y;
		SDK::FVector four = origin; four.Y += extends.Y;

		SDK::FVector five = one; five.Z += extends.Z;
		SDK::FVector six = two; six.Z += extends.Z;
		SDK::FVector seven = three; seven.Z += extends.Z;
		SDK::FVector eight = four; eight.Z += extends.Z;

		SDK::FVector2D s1, s2, s3, s4, s5, s6, s7, s8;

		PlayerController->ProjectWorldLocationToScreen(one, false, &s1);
		PlayerController->ProjectWorldLocationToScreen(two, false, &s2);
		PlayerController->ProjectWorldLocationToScreen(three, false, &s3);
		PlayerController->ProjectWorldLocationToScreen(four, false, &s4);
		PlayerController->ProjectWorldLocationToScreen(five, false, &s5);
		PlayerController->ProjectWorldLocationToScreen(six, false, &s6);
		PlayerController->ProjectWorldLocationToScreen(seven, false, &s7);
		PlayerController->ProjectWorldLocationToScreen(eight, false, &s8);


		if (utilities::IsInScreen(s1) && utilities::IsInScreen(s1))
			DrawLine(Canvas, s1, s2, col);
		if (utilities::IsInScreen(s2) && utilities::IsInScreen(s3))
			DrawLine(Canvas, s2, s3, col);
		if (utilities::IsInScreen(s3) && utilities::IsInScreen(s4))
			DrawLine(Canvas, s3, s4, col);
		if (utilities::IsInScreen(s4) && utilities::IsInScreen(s1))
			DrawLine(Canvas, s4, s1, col);


		if (utilities::IsInScreen(s5) && utilities::IsInScreen(s6))
			DrawLine(Canvas, s5, s6, col);
		if (utilities::IsInScreen(s6) && utilities::IsInScreen(s7))
			DrawLine(Canvas, s6, s7, col);
		if (utilities::IsInScreen(s7) && utilities::IsInScreen(s8))
			DrawLine(Canvas, s7, s8, col);
		if (utilities::IsInScreen(s8) && utilities::IsInScreen(s5))
			DrawLine(Canvas, s8, s5, col);


		if (utilities::IsInScreen(s1) && utilities::IsInScreen(s5))
			DrawLine(Canvas, s1, s5, col);
		if (utilities::IsInScreen(s2) && utilities::IsInScreen(s6))
			DrawLine(Canvas, s2, s6, col);
		if (utilities::IsInScreen(s3) && utilities::IsInScreen(s7))
			DrawLine(Canvas, s3, s7, col);
		if (utilities::IsInScreen(s4) && utilities::IsInScreen(s8))
			DrawLine(Canvas, s4, s8, col);

	}

	void DrawHealthBar(SDK::UCanvas* Canvas, float health, int x, int y, float height, SDK::FLinearColor color)
	{
		if (!utilities::IsInScreen(SDK::FVector2D(x, y))) return;

		if (health > 100) health = 100;

		float flBoxes = std::ceil(health / 10.f);
		float flX = x - 7 - height / 4.f; float flY = y - 1;
		float flHeight = height / 10.f;

		DrawFilledRect(Canvas, { flX, flY }, { 4, height + 2 }, { 80, 80, 80, 125 });
		DrawRect(Canvas, { flX, flY }, { 4, height + 2 }, { 0, 0, 0, 255 });
		DrawFilledRect(Canvas, { flX + 1, flY }, { 2, flHeight * flBoxes + 1 }, color);
	}
}