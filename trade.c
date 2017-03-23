
#include "trade.h"
#include "gui.h"

int trade_setup()
{
	printf("Loading trade...\n");
	main_scene = SCENE_TRADE;
	frame_skip=0;
	
	npc_faction = 0;
	trade_scroll_offset = 0;

	trade_setup_gui();

	music_stop(500);
	music_loop_group(MUSIC_ROLE_TRADE, npc_faction, 0, -1);

	//Populate npc with some random inventory

	return 0;
}

void trade_set_faction(int f)
{
	//TODO: Get rid of this and replace it with some kind of structure that handles/tracks NPCs
	npc_faction = f;
}

void trade_setup_gui()
{
	gui_clear();
	trade_button_scroll_down = gui_add_symbol_button(SYMBOL_ARROW_DOWN, 1, 54, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_down, -1, NULL, -1, NULL, -1);
	trade_button_scroll_up = gui_add_symbol_button(SYMBOL_ARROW_UP, 9, 54, -1, BUTTON_STATE_DISABLED, BUTTON_STYLE_GUI, -1, &trade_scroll_up, -1, NULL, -1, NULL, -1);
	gui_add_text_button("?", 64-12, 38, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_query, -1, NULL, -1, NULL, -1);
	gui_add_text_button("Okay", 64-47, 54, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

	int default_button;
	default_button = gui_add_text_button("Back", 64-24, 54, 21, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_cancel, -1, NULL, -1, NULL, -1);

	//TODO: combine NPC and player inventories, then loop through and render buttons for each.
	//TODO: Clip and scroll
		gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 8, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_CARGO_LIFEFORM, 28, 8, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 8, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_sell, 0, NULL, -1, NULL, -1);

		gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 16, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_CARGO_LIFEFORM, 28, 16, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 16, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_sell, 0, NULL, -1, NULL, -1);

		gui_add_symbol_button(SYMBOL_ARROW_LEFT, 18, 24, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_CARGO_LIFEFORM, 28, 24, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_buy, 0, NULL, -1, NULL, -1);
		gui_add_symbol_button(SYMBOL_ARROW_RIGHT, 64 - 26, 24, -1, BUTTON_STATE_ENABLED, BUTTON_STYLE_GUI, -1, &trade_sell, 0, NULL, -1, NULL, -1);


	update_button_state(default_button, BUTTON_STATE_SELECTED);
}

void trade_draw()
{
	SDL_RenderClear(main_renderer);
	draw_text(1, 2, "Creds", strlen("Creds"), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	char* creds = "34987306";
	draw_text(64 - strlen(creds) * 4, 2, creds, strlen(creds), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);


	char* itemString = "Han Solos";
	draw_text(2, 40, itemString, strlen(itemString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

	char* buyString = "B: 105";
	draw_text(2, 48, buyString, strlen(buyString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);
	char* sellString = "S: 257";
	draw_text(64 - strlen(sellString) * 4, 48, sellString, strlen(sellString), FONT_EARTH, -1, -1, GUI_DEFAULT_COLOR);

}


int trade_scroll_up(int v)
{
	return 0;
}

int trade_scroll_down(int v)
{
	return 0;
}

int trade_query(int v)
{
	return 0;
}

int trade_buy(int v)
{
	return 0;
}

int trade_sell(int v)
{
	return 0;
}

int trade_cancel(int v)
{
	return 0;
}

int trade_apply(int v)
{
	return 0;
}