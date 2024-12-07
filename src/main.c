
#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include "console.h"
#include "editor/editor.h"
#include "globals.h"
#include "luaapi.h"
#include "utils.h"


int main() {
	
	Console console;
	console_new(&console);
	luaapi_set_console(&console);

	Editor editor;
	editor_new(&editor, &console);

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH*5, SCREEN_HEIGHT*5, "Nano-Box");
	RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	clock_t t = clock();

	while (!WindowShouldClose()) {

		int scale = min((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);

		Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
		Rectangle destRec = {
			(GetScreenWidth()-(SCREEN_WIDTH*scale))*0.5f, 
			(GetScreenHeight()-(SCREEN_HEIGHT*scale))*0.5f, 
			SCREEN_WIDTH * scale,
			SCREEN_HEIGHT * scale
		};


		if (console.mode == CONSOLE_MODE_EDITOR)
			editor_run(&editor);
		else if (console.mode == CONSOLE_MODE_GAME)
			console_run_update(&console);

		BeginTextureMode(target);
			ClearBackground(BLACK);

			if(console.mode == CONSOLE_MODE_EDITOR)
				console_compose_frame(&editor.ram);
			else if(console.mode == CONSOLE_MODE_GAME)
				console_compose_frame(&console.ram);

		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);
			DrawTexturePro(target.texture, sourceRec, destRec, (Vector2){0.0f,0.0f}, 0.0f, WHITE);
		EndDrawing();

		// Shortcuts
		if(IsKeyPressed(KEY_ESCAPE)) {
			if(console.mode == CONSOLE_MODE_GAME)
				console.mode = CONSOLE_MODE_EDITOR;
			/*else*/
			/*	break;*/
		}
		if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_R) && console.mode == CONSOLE_MODE_EDITOR) {
			sds code_string = sdsjoin(editor.code->data, editor.code->len, "\n");
			console_load_string(&console, code_string);
			console_run_global(&console);
			console_run_boot(&console);
			console.mode = CONSOLE_MODE_GAME;
		}
	}

	console_close(&console);
	editor_close(&editor);

	UnloadRenderTexture(target);
	CloseWindow();

	return 0;
}
