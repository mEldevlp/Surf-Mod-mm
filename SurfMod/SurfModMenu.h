#pragma once

namespace surfmod
{
#define MENU_PAGE_OPTION 7

	typedef struct S_MENU_ITEM
	{
		int			Info;
		std::string Text;
		bool		Disabled;
		int			Extra;
	} P_MENU_ITEM, * LP_MENU_ITEM;

	class CMenu
	{
	public:
		void Clear();

		void Create(std::string Title, bool Exit, void* CallbackFunction);

		void AddItem(int Info, std::string Text);
		void AddItem(int Info, std::string Text, bool Disabled);
		void AddItem(int Info, std::string Text, bool Disabled, int Extra);

		void Show(int EntityIndex);
		void Hide(int EntityIndex);

		bool Handle(int EntityIndex, int Key);

		void Display(int EntityIndex, int Page);
		void ShowMenu(int EntityIndex, int Slots, int Time, std::string Text);

	private:
		std::string m_szText;
		std::vector<P_MENU_ITEM> m_Data;
		int m_iPage = 0;
		bool m_bExit = false;
		int m_iPageOption = MENU_PAGE_OPTION;
		void* m_vpFunc = nullptr;
	};

}; /* namespace surfmod */

extern std::array<surfmod::CMenu, MAX_CLIENTS + 1> g_SurfModMenu;