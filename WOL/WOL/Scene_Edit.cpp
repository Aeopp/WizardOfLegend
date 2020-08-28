#include "pch.h"
#include "Scene_Edit.h"
#include "timer.h"
#include "sound_mgr.h"
#include "object_mgr.h"
#include "Input_mgr.h"
#include "collision_mgr.h"
#include "Bmp_mgr.h"
#include "Tile_mgr.h"
#include "Font.h"
#include "game.h"
#include "helper.h"
#include "Bmp.h"
#include "Color.h"
#include "Tile.h"
#include "Debuger.h"



void Scene_Edit::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc, size_factor);

	object_mgr& _object_mgr = object_mgr::instance();
	vec cp = _object_mgr.camera_pos;

	{
		auto FontOn = Font(hdc, L"", 20, RGB(117, 245, 255));

		Timer::instance().render(hdc);

		/*auto p =Bmp_mgr::instance().Insert_Bmp(L"MAPTILE_#.bmp", L"MAPTILE_#");
		auto wp = p.lock();
		;
		GdiTransparentBlt(hdc,0,0,438,511, wp->Get_MemDC(),
			0, 0, 438, 511, RGB(255,0,255));*/

		// 원활한 에디팅을 위해 타일 좌표만큼 사각형을 그린다.

		if (bDebug)
		{
			Debuger _Debug;

			for (int i = 0; i < game::TileNumY; ++i)
			{
				for (int j = 0; j < game::TileNumX; ++j)
				{
					int left, top, right, bottom;

					left = j * game::TileWorldX;
					top = i * game::TileWorldY;
					right = left + game::TileWorldX;
					bottom = top + game::TileWorldY;

					left -= cp.x;
					right -= cp.x;
					top -= cp.y;
					bottom -= cp.y;

					Rectangle(hdc, left, top, right, bottom);
				}
			}
		}

		Tile_mgr::instance().render(hdc, size_factor);

		object_mgr::instance().render(hdc, size_factor);

		collision_mgr::instance().render(hdc, size_factor);

		if (bDebug)
		{
			auto oMouseWorldPos = Input_mgr::instance().GetWorldMousePos();
			if (!oMouseWorldPos.has_value())return;
			std::wstringstream wss;
			wss << L" X : " << oMouseWorldPos->x << L" Y : " << oMouseWorldPos->y << std::endl;
			std::wstring wstr = wss.str();
			TextOut(hdc, 150, 150, wstr.c_str(), wstr.size());
		}
	};

	if(bSelect)
	{
		auto SP_CurrentSelectImage = ImageSelectMap[CurrentSelectImage].first.lock();
		if (!SP_CurrentSelectImage)return;

		HDC hMemDC = SP_CurrentSelectImage->Get_MemDC();

		vec dl = select_image_start_pos;
		vec ds, ss;
		ss =ds = { 916,620 };
		vec sl = { 0,0 };

		if (CurrentSelectImage == ETileSelect::Info_4) {ds = ss = { 438,511 };}

		GdiTransparentBlt(hdc
			, dl.x, dl.y
			, ds.x, ds.y
			, hMemDC
			, sl.x, sl.y
			, ss.x, ss.y
			, COLOR::MEGENTA());
	}
};

void Scene_Edit::update(float dt)
{
	Scene::update(dt);

	Timer& _Timer = Timer::instance();

	_Timer.update();

	sound_mgr::instance().Frame(dt);

	Camera_Scroll();

	object_mgr::instance().update();

	collision_mgr::instance().update();

	Tile_mgr::instance().update(dt);

	Input_Check_Scroll();

	Input_mgr::instance().update();
};

void Scene_Edit::initialize()
{
	// TOOD :: Scene Dependent Init
	Scene::initialize();

	Tile_mgr::instance().initialize();

	// 이미지들 대신 이니셜라이즈
	ImageSelectMap[ETileSelect::Info_1] = { Bmp_mgr::instance().Insert_Bmp(L"MAPTILE_1.bmp", L"MAPTILE_1"),L"MAPTILE_1" };
	ImageSelectMap[ETileSelect::Info_2] = { Bmp_mgr::instance().Insert_Bmp(L"MAPTILE_2.bmp", L"MAPTILE_2"),L"MAPTILE_2"};
	ImageSelectMap[ETileSelect::Info_3] = { Bmp_mgr::instance().Insert_Bmp(L"MAPTILE_3.bmp", L"MAPTILE_3"),L"MAPTILE_3"};
	ImageSelectMap[ETileSelect::Info_4] = { Bmp_mgr::instance().Insert_Bmp(L"MAPTILE_#.bmp", L"MAPTILE_#"),L"MAPTILE_#"};
}

void Scene_Edit::release()
{
	Scene::release();

	Tile_mgr::instance().release();

	collision_mgr::instance().collision_tile_clear();

}


void Scene_Edit::Input_Check_Scroll()
{
	Input_mgr& _Input  = Input_mgr::instance();

	if (_Input.Key_Pressing(VK_LEFT))
		controll_pos += vec{ -1,0 }*scrool_speed;
	if (_Input.Key_Pressing(VK_RIGHT))
		controll_pos += vec{ +1,0 }*scrool_speed;
	if (_Input.Key_Pressing(VK_UP))
		controll_pos += vec{ 0,-1 }*scrool_speed;
	if (_Input.Key_Pressing(VK_DOWN))
		controll_pos += vec{ 0,+1 }*scrool_speed;

	if (_Input.Key_Pressing(VK_LBUTTON))
	{
		if (bSelect)
		{
			vec mp = *_Input.GetWindowMousePos() - select_image_start_pos;
			
			if (mp.x < ImageMiddleX)
			{
				CurrentImageStartPos = Tile_image_start_pos.first.Pair<int>();
			}
			else if (mp.x > ImageMiddleX)
			{
				CurrentImageStartPos = Tile_image_start_pos.second.Pair<int>();
			}

			// 타일의 이미지 인덱스
			auto oWindowpos = _Input.GetWindowMousePos();
			if (!oWindowpos)return;
			CurrentTileIndex = Tile_mgr::instance().CalcTileImageSrcIndex(*oWindowpos
				, select_image_start_pos, vec{ CurrentImageStartPos }, game::TileImgX, game::TileImgY);
		}
		else if (!bSelect)
		{
			auto oWorldwpos = _Input.GetWorldMousePos();
			if (!oWorldwpos)return;
			std::pair<int, int> world_index = Tile_mgr::instance().CalcTileWorldIndex(*oWorldwpos, game::TileWorldX,
				game::TileWorldY);

			// 이미 해당 월드 자리에 타일이 있기때문에 인서트 안함
			if (Tile_mgr::instance().IsContain(world_index))return;
			
			Tile_mgr::instance().Insert_Tile(CurrentSelectImage,
				{ game::TileWorldX,game::TileWorldY }, { game::TileImgX,game::TileImgY },
				{ CurrentImageStartPos.first + CurrentTileIndex.first * game::TileImgX,CurrentImageStartPos.second + CurrentTileIndex.second * game::TileImgY },
				COLOR::MEGENTA(), world_index);
		}
	}

	if (_Input.Key_Pressing(VK_RBUTTON))
	{
		auto oWorldwpos = _Input.GetWorldMousePos();
		if (!oWorldwpos)return;
		std::pair<int, int> world_loc = Tile_mgr::instance().CalcTileWorldIndex(*oWorldwpos, game::TileWorldX,
			game::TileWorldY);

		world_loc.first *= game::TileWorldX;
		world_loc.second*= game::TileWorldY;

		collision_mgr::instance().Insert_CollisionTile(world_loc); 
	}


	if (_Input.Key_Pressing(VK_BACK)) 
	{
		auto oWorldwpos = _Input.GetWorldMousePos();
		if (!oWorldwpos)return;
		std::pair<int, int> world_index = Tile_mgr::instance().CalcTileWorldIndex(*oWorldwpos, game::TileWorldX,
			game::TileWorldY);

		Tile_mgr::instance().Erase_Tile(world_index);
	}

	if (_Input.Key_Pressing(VK_DELETE))
	{
		auto oWorldwpos = _Input.GetWorldMousePos();
		if (!oWorldwpos)return;
		std::pair<int, int> world_loc = Tile_mgr::instance().CalcTileWorldIndex(*oWorldwpos, game::TileWorldX,
			game::TileWorldY);

		world_loc.first *= game::TileWorldX;
		world_loc.second *= game::TileWorldY;

		collision_mgr::instance().Erase_CollisionTile(world_loc);
	}


	if (_Input.Key_Down('Q'))
	{
		Tile_mgr::instance().Save_Tile();
	}

	if (_Input.Key_Down('W'))
	{
		Tile_mgr::instance().Load_Tile();
	}

	if (_Input.Key_Down('Z'))
	{
		Tile_mgr::instance().Save_Tile(Tile_mgr::BossStageFileName);
	}

	if (_Input.Key_Down('X'))
	{
		Tile_mgr::instance().Load_Tile(Tile_mgr::BossStageFileName);
	}

	if (_Input.Key_Down('R'))
	{
		collision_mgr::instance().save_collision(collision_mgr::StageFileName);
	}

	if (_Input.Key_Down('T'))
	{
		collision_mgr::instance().load_collision(collision_mgr::StageFileName);
	}

	if (_Input.Key_Down('V'))
	{
		collision_mgr::instance().save_collision(collision_mgr::BossStageFileName);
	}

	if (_Input.Key_Down('B'))
	{
		collision_mgr::instance().load_collision(collision_mgr::BossStageFileName);
	}

	if (_Input.Key_Down('1'))
	{
		Tile_image_start_pos = { {73,82},{515,82} };

		CurrentSelectImage = ETileSelect::Info_1;
	}

	if (_Input.Key_Down('2'))
	{
		Tile_image_start_pos = { {73,82},{515,82} };

		CurrentSelectImage = ETileSelect::Info_2;
	}

	if (_Input.Key_Down('3'))
	{
		Tile_image_start_pos = { {73,82},{515,82} };

		CurrentSelectImage = ETileSelect::Info_3;
	}

	if (_Input.Key_Down('4'))
	{
		Tile_image_start_pos = { {0,0} ,{0,0} };
		CurrentSelectImage = ETileSelect::Info_4;
	}

	if (_Input.Key_Down(VK_TAB))
	{
		bSelect = !bSelect;
	}
}

void Scene_Edit::Camera_Scroll()
{
	RECT _rt = game::instance().client_rect;

	object_mgr& _object_mgr = object_mgr::instance();

	int width = (_rt.right - _rt.left) / 2;
	int height = (_rt.bottom - _rt.top) / 2;

	vec& cp = _object_mgr.camera_pos;

	cp = controll_pos;

	cp.x -= width;
	cp.y -= height;
}

Scene_Edit::~Scene_Edit() noexcept
{
	release();
}
