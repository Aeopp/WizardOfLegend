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
#include "Scene_mgr.h"




void Scene_Edit::render(HDC hdc, std::pair<float, float> size_factor)
{
	Scene::render(hdc, size_factor);

	object_mgr& _object_mgr = object_mgr::instance();
	vec cp = _object_mgr.camera_pos;

	{
		auto FontOn = Font(hdc, L"", 20, RGB(117, 245, 255));

		// 원활한 에디팅을 위해 타일 좌표만큼 사각형을 그린다.

		if (bDebug)
		{
			Debuger _Debug(hdc, RGB(120, 120, 120), RGB(60, 60, 60));

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

		Tile_mgr::instance().render(hdc,cp, size_factor);


		if (bSelect)
		{
			auto SP_CurrentSelectImage = Bmp_mgr::ImageSelectMap[CurrentSelectImage].first.lock();
			if (!SP_CurrentSelectImage)return;

			HDC hMemDC = SP_CurrentSelectImage->Get_MemDC();

			vec dl = select_image_start_pos;
			vec ds, ss;
			ss = ds = { 916,620 };
			vec sl = { 0,0 };

			if (CurrentSelectImage == ETileSelect::Info_4) { ds = ss = { 438,511 }; }

			GdiTransparentBlt(hdc
				, dl.x, dl.y
				, ds.x, ds.y
				, hMemDC
				, sl.x, sl.y
				, ss.x, ss.y
				, COLOR::MEGENTA());
		}

		_object_mgr.render(hdc, size_factor);
		Tile_mgr::instance().DecoRender(hdc, cp);
		_object_mgr.UIEffectRender(hdc, cp, size_factor);
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
		Timer::instance().render(hdc);

	};

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

			if (CurrentSelectImage == ETileSelect::Info_4)
			{
				// 맵을 순회하며 찾기
				for (auto& [k,ImgRt] : Bmp_mgr::DecoTileInfoMap)
				{
					if (math::RectInPoint(ImgRt, mp))
					{
						CurrentImageStartPos = {ImgRt.left, ImgRt.top};
						CurrentTileWorldSizeX = CurrentTileImgSizeX =  ImgRt.right - ImgRt.left;
						CurrentTileWorldSizeY  =CurrentTileImgSizeY = ImgRt.bottom- ImgRt.top;
					}
				}
			}
			else if (mp.x < ImageMiddleX)
			{
				CurrentImageStartPos = Tile_image_start_pos.first.Pair<int>();
				CurrentTileWorldSizeX  = CurrentTileImgSizeX = game::TileImgX;
				CurrentTileWorldSizeY = CurrentTileImgSizeY = game::TileImgY;
			}
			else if (mp.x > ImageMiddleX)
			{
				CurrentImageStartPos = Tile_image_start_pos.second.Pair<int>();
				CurrentTileWorldSizeX  = CurrentTileImgSizeX = game::TileImgX;
				CurrentTileWorldSizeY  = CurrentTileImgSizeY = game::TileImgY;
			}

			// 타일의 이미지 인덱스
			// TODO ::  데코 이미지의 경우   CurrentImageStartPos 좌표를 충돌한 데코 이미지의 시작 부분으로 변경 해주기.
			auto oWindowpos = _Input.GetWindowMousePos();
			if (!oWindowpos)return;
			CurrentTileIndex = Tile_mgr::instance().CalcTileImageSrcIndex(*oWindowpos
				, select_image_start_pos, vec{ CurrentImageStartPos }, CurrentTileImgSizeX, CurrentTileImgSizeY);
		}
		else if (!bSelect)
		{
			auto oWorldwpos = _Input.GetWorldMousePos();
			if (!oWorldwpos)return;
			std::pair<int, int> world_index = Tile_mgr::instance().CalcTileWorldIndex(*oWorldwpos, game::TileWorldX,
				game::TileWorldY);

			// 타일 위의 데코하는 타일들이 아니고 이미 타일이 자리잡았기 때문에 인서트를 취소
			bool bDeco  = CurrentSelectImage == ETileSelect::Info_4;

			if (!bDeco && Tile_mgr::instance().IsContain(world_index) )
			{
				return;
			}

			Tile_mgr::instance().Insert_Tile(CurrentSelectImage,
				{ CurrentTileWorldSizeX,CurrentTileWorldSizeY }, { CurrentTileImgSizeX,CurrentTileImgSizeY },
				{ CurrentImageStartPos.first + CurrentTileIndex.first * CurrentTileImgSizeX,CurrentImageStartPos.second + CurrentTileIndex.second * CurrentTileImgSizeY },
				COLOR::MEGENTA(), world_index, bDeco);
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
		Tile_mgr::instance().Load_Tile(Tile_mgr::StageFileName);
	}

	if (_Input.Key_Down('W'))
	{
		Tile_mgr::instance().Load_Tile(Tile_mgr::BossStageFileName);
	}
	if (_Input.Key_Down('Q'))
	{
		collision_mgr::instance().load_collision(Tile_mgr::StageFileName);
	}

	if (_Input.Key_Down('W'))
	{
		collision_mgr::instance().load_collision(Tile_mgr::BossStageFileName);
	}


	if (_Input.Key_Down('Z'))
	{
		Tile_mgr::instance().Save_Tile(Tile_mgr::StageFileName);
	}

	if (_Input.Key_Down('X'))
	{
		collision_mgr::instance().save_collision(collision_mgr::StageFileName);
	}

	if (_Input.Key_Down(VK_ESCAPE))
	{
		MessageBox(game::hWnd, L"시작 화면으로 이동합니다.", L"Main", MB_OK);
		Scene_mgr::instance().Scene_Change(ESceneID::EStart);
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
