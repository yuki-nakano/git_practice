#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

int start = 0;				//ENTERを押されることにより+1されゲームが始まる			
DWORD gStartTime = 0;		//ゲームの始まった時間を保存
DWORD gElapsedTime = 0;		//ゲーム開始からの経過時間
int gPushKeyCount = 0;		//連打回数を保存

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	while (true)
	{
		bool message_ret = false;
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{

			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
	{
		gStartTime = timeGetTime();
		start++;
	}

	if (3000 <= gElapsedTime && gElapsedTime <=13000)
	{
		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
		{
			gPushKeyCount++;
		}
	}
/*	交互に連打
	if (3000 <= gElapsedTime && gElapsedTime <=13000)
	{
		if (Engine::IsKeyboardKeyPushed(DIK_A) == true && gPushKeyCount % 2 == 1)
		{
			gPushKeyCount++;
		}

		if (Engine::IsKeyboardKeyPushed(DIK_S) == true && gPushKeyCount % 2 == 0)
		{
			gPushKeyCount++;
		}
	}
*/

	//========================================================
	// キーボードの入力取得
	//========================================================

	//if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
	//{
	//	// キーが押された瞬間の処理
	//}

	//if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	//{
	//	// キーが押されている間の処理
	//}

	//if (Engine::IsKeyboardKeyReleased(DIK_A))
	//{
	//	// キーが離された瞬間の処理
	//}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	if (start == 0)
	{
		Engine::DrawFont(80.0f, 230.0f, "ENTERを押して開始してください", FontSize::Large, FontColor::White);
	}
	else if(start >= 1)
	{

		gElapsedTime = timeGetTime() - gStartTime;

		//初めのカウントダウン
		if (gElapsedTime <= 3000)
		{
			char countDown[2];
			int countTime = 3 - gElapsedTime / 1000;
			sprintf_s(countDown, 2, "%d", countTime);
			Engine::DrawFont(300.0f, 220.0f, countDown, FontSize::Large, FontColor::White);
		}
		else if (3000 < gElapsedTime && gElapsedTime <= 10000)
		{
			Engine::DrawFont(240.0f, 220.0f, "スタート", FontSize::Large, FontColor::White);
		}
		//終わりのカウントダウン
		else if (10000 < gElapsedTime && gElapsedTime <= 13000)
		{
			char countDown[2];
			int countTime = 13 - gElapsedTime / 1000;
			sprintf_s(countDown, 2, "%d", countTime);
			Engine::DrawFont(300.0f, 220.0f, countDown, FontSize::Large, FontColor::White);
		}
		else if (13000 < gElapsedTime && gElapsedTime <= 15000)
		{
			Engine::DrawFont(280.0f, 220.0f, "終了", FontSize::Large, FontColor::White);
		}

		//連打中の連打回数の表示
		if (3000 <= gElapsedTime && gElapsedTime <= 8000)
		{
			char counter[4];
			sprintf_s(counter, 4, "%d", gPushKeyCount);
			Engine::DrawFont(290.0f, 260.0f, counter, FontSize::Large, FontColor::White);
		}
		else if (8000 <= gElapsedTime && gElapsedTime <= 13000)
		{
			Engine::DrawFont(290.0f, 260.0f, "--", FontSize::Large, FontColor::White);
		}

		//結果表示
		if (16000 < gElapsedTime)
		{
			Engine::DrawFont(210.0f, 220.0f, "あなたの結果は", FontSize::Large, FontColor::White);
		}
		if (17000 <= gElapsedTime)
		{
			char counter[4];
			sprintf_s(counter, 4, "%d", gPushKeyCount);
			Engine::DrawFont(290.0f, 260.0f, counter, FontSize::Large, FontColor::White);
			Engine::DrawFont(320.0f, 260.0f, "回", FontSize::Large, FontColor::White);
		}
	}


	// フォント描画
//	Engine::DrawFont(0.0f, 0.0f, "FontSize:Small", FontSize::Small, FontColor::White);
//	Engine::DrawFont(0.0f, 30.0f, "FontSize:Regular", FontSize::Regular, FontColor::White);
//	Engine::DrawFont(0.0f, 60.0f, "FontSize:Large", FontSize::Large, FontColor::White);

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
