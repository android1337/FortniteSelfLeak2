#pragma once

static float width;
static float height;


#define RELATIVE(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

namespace utilities
{
	inline BOOL IsInScreen(SDK::FVector2D Pos) {
		if (((Pos.X <= 0 or Pos.X > width) and (Pos.Y <= 0 or Pos.Y > height)) or ((Pos.X <= 0 or Pos.X > width) or (Pos.Y <= 0 or Pos.Y > height))) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}

	inline SDK::FLinearColor getColorFromTier(SDK::EFortItemTier Tier) {
		SDK::FLinearColor color = { 255, 255, 224, 1.f };

		if (Tier == SDK::EFortItemTier::I)
			color = { 255, 255, 224, 1.f };
		else if (Tier == SDK::EFortItemTier::II)
			color = { 144, 238, 144, 1.f };
		else if (Tier == SDK::EFortItemTier::III)
			color = { 0, 139, 139, 1.f };
		else if (Tier == SDK::EFortItemTier::IV)
			color = { 128, 0, 128, 1.f };
		else if (Tier == SDK::EFortItemTier::V)
			color = { 255, 165, 0, 1.f };

		return color;
	}

	inline uintptr_t imageBase()
	{
		return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
	}

	inline bool isValidPointer(uintptr_t address)
	{
		if (address)
			return true;
		else
			return false;
	}

	template <typename R>
	inline R read(uintptr_t address)
	{
		R buffer{};

		if (!isValidPointer(address))
			return buffer;

		buffer = *(R*)address;

		return buffer;
	}

	template <typename W>
	inline void write(uintptr_t address, const W& value)
	{
		W buffer{};

		if (!isValidPointer(address))
			return;

		*(W*)address = value;
	}


	inline float custom_sqrtf(float _X)
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X)));
	}

	inline float custom_sinf(float _X)
	{
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X)));
	}

	inline float custom_cosf(float _X)
	{
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X)));
	}

	inline int custom_fabs(int _X)
	{
		int s = _X >> 31;
		return (_X ^ s) - s;
	}

	inline float custom_acosf(float _X)
	{
		return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(_X)));
	}

	inline float custom_tanf(float _X)
	{
		return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(_X)));
	}

	inline float custom_atan2f(float _X, float _Y)
	{
		return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(_X), _mm_set_ss(_Y)));
	}

	inline int custom_compare(const char* X, const char* Y)
	{
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == '\0');
	}

	inline int custom_wcompare(const wchar_t* X, const wchar_t* Y)
	{
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == L'\0');
	}
	
	inline const wchar_t* custom_wcsstr(const wchar_t* X, const wchar_t* Y)
	{
		while (*X != L'\0') {
			if ((*X == *Y) && custom_wcompare(X, Y)) {
				return X;
			}
			X++;
		}
		return NULL;
	}

	inline const char* custom_strstr(const char* X, const char* Y)
	{
		while (*X != '\0') {
			if ((*X == *Y) && custom_compare(X, Y)) {
				return X;
			}
			X++;
		}
		return NULL;
	}

	inline int custom_strlen(const char* string)
	{
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	inline int custom_wcslen(const wchar_t* string)
	{
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	inline int custom_isspace(int c)
	{
		return (c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r' || c == ' ' ? 1 : 0);
	}

	inline int custom_isdigit(int c)
	{
		return (c >= '0' && c <= '9' ? 1 : 0);
	}

	inline int custom_isalpha(int c)
	{
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
	}

	inline char custom_isupper(unsigned char c)
	{
		if (c >= (unsigned char)('A') && c <= (unsigned char)('Z'))
			return 1;
	}


	inline unsigned long custom_strtoul(const char* nptr, char** endptr, register int base)
	{
		register const char* s = nptr;
		register unsigned long acc;
		register int c;
		register unsigned long cutoff;
		register int neg = 0, any, cutlim;
		do {
			c = *s++;
		} while (custom_isspace(c));
		if (c == '-') {
			neg = 1;
			c = *s++;
		}
		else if (c == '+')
			c = *s++;
		if ((base == 0 || base == 16) &&
			c == '0' && (*s == 'x' || *s == 'X')) {
			c = s[1];
			s += 2;
			base = 16;
		}
		if (base == 0)
			base = c == '0' ? 8 : 10;
		cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
		cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
		for (acc = 0, any = 0;; c = *s++) {
			if (custom_isdigit(c))
				c -= '0';
			else if (custom_isalpha(c))
				c -= custom_isupper(c) ? 'A' - 10 : 'a' - 10;
			else
				break;
			if (c >= base)
				break;
			if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
				any = -1;
			else {
				any = 1;
				acc *= base;
				acc += c;
			}
		}
		if (any < 0) {
			acc = ULONG_MAX;
			errno = ERANGE;
		}
		else if (neg)
			acc *= -1.f;
		if (endptr != 0)
			*endptr = (char*)(any ? s - 1 : nptr);
		return (acc);
	}

	inline std::string custom_to_string(int n)
	{
		int m = n;
		char s[100]; //max
		char ss[100]; //max
		int i = 0, j = 0;
		if (n < 0)
		{
			m = 0 - m;
			j = 1;
			ss[0] = '-';
		}
		while (m > 0)
		{
			s[i++] = m % 10 + '0';
			m /= 10;
		}
		s[i] = '\0';
		i = i - 1;
		while (i >= 0)
		{
			ss[j++] = s[i--];
		}
		ss[j] = '\0';
		return ss;
	}

	inline uintptr_t scan(uintptr_t pModuleBaseAddress, const char* sSignature)
	{
		static auto patternToByte = [](const char* pattern)
		{
			auto       bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + custom_strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else
					bytes.push_back(custom_strtoul((const char*)current, &current, 16));
			}
			return bytes;
		};

		const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
		const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

		const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto       patternBytes = patternToByte(sSignature);
		const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

		const auto s = patternBytes.size();
		const auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;

			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			}
		}

		return NULL;
	}
}

namespace keybinds
{
	SDK::FKey RightClick;
	SDK::FKey LeftClick;
	SDK::FKey Insert;
	SDK::FKey Tab;

	SDK::FKey F1;
	SDK::FKey F2;
	SDK::FKey F3;
	SDK::FKey F4;
	SDK::FKey F5;
	SDK::FKey F6;
	SDK::FKey F7;
	SDK::FKey F8;
	SDK::FKey F9;

	SDK::FKey W;
	SDK::FKey A;
	SDK::FKey S;
	SDK::FKey D;
	SDK::FKey SpaceBar;
	SDK::FKey LeftShift;
}


namespace SDK
{
	inline uintptr_t Address_GetBoneMatrix = 0;
	
	namespace useless {
		struct FMatrix {
			float M[4][4];
		};
		static FMatrix* myMatrix = new FMatrix();
	}

	inline SDK::FVector GetBone(SDK::AFortPawn* actor, int Id)
	{
		if (!actor) return { 0, 0, 0 };

		auto mesh = actor->Mesh;
		if (!mesh) return { 0, 0, 0 };

		auto function = ((useless::FMatrix * (__fastcall*)(uintptr_t, useless::FMatrix*, int))(Address_GetBoneMatrix));
		SpoofCall(function, uintptr_t(mesh), useless::myMatrix, Id);

		return { useless::myMatrix->M[3][0], useless::myMatrix->M[3][1], useless::myMatrix->M[3][2] };
	}
}

//Settings

namespace Settings {

	struct playersESP {
		bool Box2D;
		bool Box3D;
		bool Skeleton;
		bool Lines;
		bool Names;
		bool Weapons;
		bool Distance;
		bool Platform;
		bool TargetLine;
		bool Glow;
		bool Chams;
		bool Radar;
	};

	struct worldESP {
		bool Loot;
		bool Chest;
		bool AmmoBox;
	};

	struct aimbot {
		bool Memory;
		bool SniperPrediction;

		int FovCircle = 300;
	};

	struct exploits {
		bool DayTime;
		bool FirstCamera;
		bool NoVehicleCollision = false;
		bool VehicleFly;
		bool ZiplineTeleport;
		bool Spectate;
		bool InfiniteJump;
	};

	inline playersESP* PlayersESP = new playersESP();
	inline worldESP* WorldESP = new worldESP();
	inline aimbot* Aimbot = new aimbot();
	inline exploits* Exploits = new exploits();
}

namespace DisabledSettings {

	struct exploits {
		bool NoVehicleCollision = !Settings::Exploits->NoVehicleCollision;
	};

	inline exploits* Exploits = new exploits();
}