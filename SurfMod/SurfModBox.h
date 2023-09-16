#pragma once

#define DEFAULT_MINSIZE  -32.0, -32.0, -32.0
#define DEFAULT_MAXSIZE  32.0, 32.0, 32.0

namespace surfmod {

	class CBox
	{
	public:
		void BoxMainMenu(int EntityIndex);
		static void BoxMainMenu_Handle(int EntityIndex, P_MENU_ITEM Item);


	private:
		edict_t* box_Create(std::string szClass, std::string szId, const Vector fOrigin, const float fMins[3], const float fMaxs[3], int editor = 0);
	};
};

extern surfmod::CBox g_SurfModBox;