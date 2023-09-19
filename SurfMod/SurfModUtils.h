#pragma once
// Print SayText Colors
constexpr auto PRINT_TEAM_DEFAULT = 0;
constexpr auto PRINT_TEAM_GREY = -1;
constexpr auto PRINT_TEAM_RED = -2;
constexpr auto PRINT_TEAM_BLUE = -3;

// Client Print
constexpr auto PRINT_NOTIFY = 1;
constexpr auto PRINT_CONSOLE = 2;
constexpr auto PRINT_CHAT = 3;
constexpr auto PRINT_CENTER = 4;
constexpr auto PRINT_RADIO = 5;

constexpr auto MAX_LINE_TXT_MSG = 191;

#define _TXT(TextString, ...) (g_SurfModUtility.FormatString(TextString, ##__VA_ARGS__))

namespace surfmod
{
#define BIT(n) (1 << (n))

	class CUtility
	{
	public:

		// Send say text message
		void SayText(edict_t* pEntity, int Sender, const char* Format, ...);

		 //Get players vector
		std::vector<CBasePlayer*> GetPlayers(bool InGameOnly, bool ReturnBots);

		// Format string returning constant char array result
		const char* FormatString(const char* Format, ...);

		void ServerCommand(const char* Format, ...);

		// Replace all in string
		void ReplaceAll(std::string& String, const std::string& From, const std::string& To);

		// New Lines and colors
		int ParseLinesAndColors(char* Buffer);

		unsigned short FixedUnsigned16(float value, float scale);
		short FixedSigned16(float value, float scale);

		// Set hudmessage parameters
		hudtextparms_t HudParam(int red, int green, int blue, float x, float y, int effects, float fxtime, float holdtime, float fadeintime, float fadeouttime, int channel);

		// Send hudmessage
		void HudMessage(edict_t* pEntity, hudtextparms_t textparms, const char* Format, ...);

		char m_GameDesc[32] = { 0 };

	private:
		// Variables Pointer
		std::map<std::string, cvar_t> m_CvarData;
	};

}; /* namespace surfmod */

extern surfmod::CUtility g_SurfModUtility;
