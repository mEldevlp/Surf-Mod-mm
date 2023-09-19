#include "precompiled.h"

surfmod::CBox g_SurfModBox;

void surfmod::CBox::BoxMainMenu(int EntityIndex)
{
	std::string title = g_SurfModUtility.FormatString("^yBox menu\n\n");

	g_SurfModMenu[EntityIndex].Create(title, true, reinterpret_cast<void*>(this->BoxMainMenu_Handle));
	g_SurfModMenu[EntityIndex].AddItem(0, "Create");

	g_SurfModMenu[EntityIndex].Show(EntityIndex);
}

void surfmod::CBox::BoxMainMenu_Handle(int EntityIndex, P_MENU_ITEM Item)
{
	switch (Item.Info)
	{
	case 0:
	{
		Vector origin = INDEXENT(EntityIndex)->v.origin;
		const float fMins[3] = { DEFAULT_MINSIZE };
		const float fMax[3] = { DEFAULT_MAXSIZE };

		g_SurfModBox.box_Create("box", "", origin, fMins, fMax, EntityIndex);

		break;
	}

	default:
		break;
	}
}

edict_t* surfmod::CBox::box_Create(std::string szClass, std::string szId, const Vector fOrigin, const float fMins[3], const float fMaxs[3], int editor)
{
	edict_t* pBox = g_engfuncs.pfnCreateNamedEntity(MAKE_STRING("info_target"));

	if (!pBox || !pBox->pvPrivateData)
	{
		if (pBox)
			g_engfuncs.pfnRemoveEntity(pBox);

		return nullptr;
	}

	pBox->v.classname = MAKE_STRING(szClass.c_str());
	pBox->v.model = MAKE_STRING("sprites/cnt1.spr");
	pBox->v.effects = EF_NODRAW;
	pBox->v.solid = SOLID_TRIGGER;
	pBox->v.movetype = MOVETYPE_NONE;
	//pBox->v.enemy = INDEXENT(1);
	pBox->v.nextthink = gpGlobals->time + 0.1;
	pBox->v.origin = fOrigin;

	SET_SIZE(pBox, fMins, fMaxs);

	return pBox;
}
