#include "pch.h"
#include "ZeroGUI.h"

#include "Menu.h"

int TestNum = 50;
float HandPower = -1.f;

void Menu::Tick(CG::UCanvas* canvas, CG::UFont* font)
{
	ZeroGUI::SetupCanvas(canvas, font);
	ZeroGUI::Input::Handle();

	static bool menuOpened = false;
	static CG::FVector2D pos = { 100.f, 200.f };
	if (GetAsyncKeyState(VK_INSERT) & 1)
		menuOpened = !menuOpened; // Our menu key

	if (ZeroGUI::Window("Superior UE4 GUI", &pos, CG::FVector2D{ 500.0f, 200.0f }, menuOpened))
	{
		//Simple Tabs
		static int tab = 0;
		if (ZeroGUI::ButtonTab("Tab 1", CG::FVector2D{ 110, 25 }, tab == 0))
			tab = 0;
		if (ZeroGUI::ButtonTab("Tab 2", CG::FVector2D{ 110, 25 }, tab == 1))
			tab = 1;
		ZeroGUI::NextColumn(130.0f);
		//

		// Some Elements
		static int predectHotkey = VK_NUMPAD5;
		ZeroGUI::Hotkey("Predect Hotkey", CG::FVector2D{ 80, 25 }, &predectHotkey);

		ZeroGUI::Text("Hand Power: ");
		ZeroGUI::SameLine();
		ZeroGUI::Text(std::to_string(ceil(HandPower * 100.0f) / 100.0f).c_str());
		ZeroGUI::SameLine();
		if (ZeroGUI::Button("It's a Button!", CG::FVector2D{ 100, 25 }) || GetAsyncKeyState(predectHotkey) & 1)
		{
			//HandPower = cheats->PredictHandPower();
		}
	}

	ZeroGUI::Render(); // Custom Render. I use it for drawing Combobox and ColorPicker over the menu
	ZeroGUI::Draw_Cursor(menuOpened);
}