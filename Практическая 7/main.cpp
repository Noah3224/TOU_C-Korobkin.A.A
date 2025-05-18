#include "raylib.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PLAYER_SPEED 5
#define MONSTER_SPEED 2
#define MAX_MONSTERS 3
#define ATTACK_COOLDOWN 0.5f

struct Entity {
	Vector2 position;
	int health;
	int maxHealth;
	int damage;
	Texture2D texture;
	bool isAlive;
};

struct Bonus {
	Vector2 position;
	Texture2D texture;
	bool active;
	float duration;
};

float lastPlayerAttackTime = 0.0f;
float lastMonsterAttackTime = 0.0f;
bool showDamageEffect = false;
float damageTimer = 0;
float damageEffectDuration = 0.1f;

int monstersKilled = 0;  

Texture2D LoadTextureSafe(const char* path) {
	Texture2D tex = LoadTexture(path);
	if (!tex.id) DrawText("Texture not found", 50, 50, 30, RED);
	return tex;
}

void DrawHealthBar(Vector2 pos, int health, int maxHealth) {
	DrawRectangle(pos.x - 2, pos.y - 20, 104, 14, DARKGRAY);
	DrawRectangle(pos.x, pos.y - 18, (float)health / maxHealth * 100, 10, GREEN);
	DrawRectangleLines(pos.x, pos.y - 18, 100, 10, BLACK);
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Битва с монстром");
	InitAudioDevice();
	SetTargetFPS(60);
	
	Music music = LoadMusicStream("assets/music.ogg");
	Sound attackSound = LoadSound("assets/attack.wav");
	PlayMusicStream(music);
	
	Texture2D playerTexture = LoadTexture("assets/player.png");
	Texture2D monsterTexture = LoadTexture("assets/monster.png");
	Texture2D background = LoadTexture("assets/background.png");
	Texture2D bonusTexture = LoadTexture("assets/bonus.png");
	
	Entity player = { { 200, 200 }, 100, 100, 20, playerTexture, true };
	std::vector<Entity> monsters;
	for (int i = 0; i < MAX_MONSTERS; i++) {
		Entity m = { {(float)(rand()% (SCREEN_WIDTH - 64) + 32), (float)(rand()% (SCREEN_HEIGHT - 64) + 32)}, 50 + rand()%50, 50 + rand()%50, 10 + rand()%10, monsterTexture, true };
		monsters.push_back(m);
	}
	
	Bonus bonus = { {600, 400}, bonusTexture, true, 0.0f };
	
	bool gameOver = false;
	bool gamePaused = false;
	
	while (!WindowShouldClose()) {
		UpdateMusicStream(music);
		float deltaTime = GetFrameTime();
		
		if (IsKeyPressed(KEY_P) && !gameOver) gamePaused = !gamePaused;
		
		if (!gameOver && !gamePaused) {
			
			if (IsKeyDown(KEY_RIGHT)) player.position.x += PLAYER_SPEED;
			if (IsKeyDown(KEY_LEFT)) player.position.x -= PLAYER_SPEED;
			if (IsKeyDown(KEY_UP)) player.position.y -= PLAYER_SPEED;
			if (IsKeyDown(KEY_DOWN)) player.position.y += PLAYER_SPEED;
			
			
			if (player.position.x < 32) player.position.x = 32;
			if (player.position.x > SCREEN_WIDTH - 32) player.position.x = SCREEN_WIDTH - 32;
			if (player.position.y < 32) player.position.y = 32;
			if (player.position.y > SCREEN_HEIGHT - 32) player.position.y = SCREEN_HEIGHT - 32;
			
		
			if (bonus.active && CheckCollisionCircles(player.position, 30, bonus.position, 20)) {
				bonus.active = false;
				player.health = (player.health + 30 > player.maxHealth) ? player.maxHealth : player.health + 30;
			}
			
		
			for (auto& monster : monsters) {
				if (!monster.isAlive) continue;
				
				
				Vector2 dir;
				dir.x = player.position.x - monster.position.x;
				dir.y = player.position.y - monster.position.y;
				
				
				float length = sqrt(dir.x * dir.x + dir.y * dir.y);
				if (length != 0) {
					dir.x /= length;
					dir.y /= length;
				}
				
				if (monster.health > 20) {
					monster.position.x += dir.x * MONSTER_SPEED;
					monster.position.y += dir.y * MONSTER_SPEED;
				} else {
					monster.position.x -= dir.x * MONSTER_SPEED;
					monster.position.y -= dir.y * MONSTER_SPEED;
				}
				
				
				if (monster.position.x < 32) monster.position.x = 32;
				if (monster.position.x > SCREEN_WIDTH - 32) monster.position.x = SCREEN_WIDTH - 32;
				if (monster.position.y < 32) monster.position.y = 32;
				if (monster.position.y > SCREEN_HEIGHT - 32) monster.position.y = SCREEN_HEIGHT - 32;
				
			
				if (CheckCollisionCircles(player.position, 30, monster.position, 30)) {
				
					if (GetTime() - lastMonsterAttackTime > ATTACK_COOLDOWN) {
						player.health -= monster.damage;
						lastMonsterAttackTime = GetTime();
						PlaySound(attackSound);
						showDamageEffect = true;
						damageTimer = 0;
						if (player.health <= 0) {
							player.health = 0;
							gameOver = true;
							gamePaused = false; 
						}
					}
					
					if (IsKeyDown(KEY_SPACE)) {
						if (GetTime() - lastPlayerAttackTime > ATTACK_COOLDOWN) {
							monster.health -= player.damage;
							lastPlayerAttackTime = GetTime();
							PlaySound(attackSound);
							if (monster.health <= 0) {
								monster.health = 0;
								monster.isAlive = false;
								monstersKilled++; 
							}
						}
					}
				}
			}
		}
		else if (gameOver) {
			
			if (IsKeyPressed(KEY_ENTER)) {
			
				player.health = player.maxHealth;
				player.position = { 200, 200 };
				monsters.clear();
				for (int i = 0; i < MAX_MONSTERS; i++) {
					Entity m = { {(float)(rand()% (SCREEN_WIDTH - 64) + 32), (float)(rand()% (SCREEN_HEIGHT - 64) + 32)}, 50 + rand()%50, 50 + rand()%50, 10 + rand()%10, monsterTexture, true };
					monsters.push_back(m);
				}
				bonus.active = true;
				monstersKilled = 0;
				gameOver = false;
			}
		}
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		DrawTexture(background, 0, 0, WHITE);
		
		if (gamePaused && !gameOver) {
			DrawText("PAUSE", 400, 340, 20, YELLOW);
		} else {
			DrawTexture(player.texture, player.position.x - 32, player.position.y - 32, WHITE);
			DrawHealthBar(player.position, player.health, player.maxHealth);
			
			for (const auto& monster : monsters) {
				if (!monster.isAlive) continue;
				DrawTexture(monster.texture, monster.position.x - 32, monster.position.y - 32, WHITE);
				DrawHealthBar(monster.position, monster.health, monster.maxHealth);
			}
			
			if (bonus.active)
				DrawTexture(bonus.texture, bonus.position.x - 16, bonus.position.y - 16, WHITE);
			
			if (showDamageEffect) {
				damageTimer += deltaTime;
				DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RED, 0.2f));
				if (damageTimer > damageEffectDuration) showDamageEffect = false;
			}
			
		
			DrawText(("Monster Death: " + std::to_string(monstersKilled)).c_str(), 10, 10, 20, DARKGRAY);
			
			if (gameOver) {
				DrawText("DEATH! PRESS ENTER", 400, 360, 30, RED);
			}
		}
		
		EndDrawing();
	}
	
	UnloadTexture(playerTexture);
	UnloadTexture(monsterTexture);
	UnloadTexture(background);
	UnloadTexture(bonusTexture);
	UnloadMusicStream(music);
	UnloadSound(attackSound);
	CloseAudioDevice();
	CloseWindow();
	
	return 0;
}

