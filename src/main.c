
#include <raylib.h>
#include <rlgl.h>
#include "console.h"
#include "globals.h"
#include "luaapi.h"


int main() {

	Conosole c;
	console_new(&c, "lua_examples/hello.lua");
	set_console(&c);
	register_lua_api(c.L);

	console_run_boot(&c);

	const int screenWidth = SCREEN_WIDTH * 5;
	const int screenHeight = SCREEN_HEIGHT * 5;

	float pixelRatio = (float)screenWidth / (float)SCREEN_WIDTH;

	InitWindow(screenWidth, screenHeight, "Bit-8");

	RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

	Rectangle sourceRec = {0.0f, 0.0f, (float)target.texture.width,
						 -(float)target.texture.height};
	Rectangle destRec = {
	  -pixelRatio, -pixelRatio, 
	  screenWidth + (pixelRatio * 2), screenHeight + (pixelRatio * 2)};

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		/*console_run_update(&console);*/
		console_run_update(&c);

		BeginTextureMode(target);
			/*ClearBackground(RAYWHITE);*/
			console_compose_frame(&c);
			/*DrawRectanglePro((Rectangle){12, 12, 64, 64}, (Vector2){0, 0}, 50.0f, c);*/
		EndTextureMode();

		BeginDrawing();
			DrawTexturePro(target.texture, sourceRec, destRec, (Vector2){0, 0}, 0.0f,WHITE);
		EndDrawing();
	}

	UnloadRenderTexture(target);
	CloseWindow();
	console_close(&c);

	return 0;
}
