#pragma once
#include "singleton_interface.h"
#include "Tile.h"

class Bmp_mgr : public singleton_interface<Bmp_mgr>
{
public :
	void initialize();
	void release() ;

	virtual ~Bmp_mgr()noexcept; 
public:
	// 경로를 DefaultPath 에 지정해주고 파일이름과 확장자만 알려주세요.
	std::weak_ptr<class Bmp> Insert_Bmp(std::wstring  FileName ,std::wstring ImageKey);
	std::weak_ptr<class Bmp> Find_Image_WP(std::wstring ImageKey);
	// 소유권을 바깥으로 전파합니다.
	std::shared_ptr<class Bmp> Find_Image_SP(std::wstring ImageKey); 

	void Release();

	 std::wstring DefaultPath = L"..\\..\\Resources\\WOL_TEXTURE\\";

	 static inline std::map<EDecoTileInfo, RECT> DecoTileInfoMap{};

	 static inline std::map<ETileSelect, std::pair<std::weak_ptr<class Bmp>, std::wstring>> ImageSelectMap{};
	 ETileSelect CurrentSelectImage{};
private:
	std::map<std::wstring, std::shared_ptr<class Bmp>>bmp_map;
};

