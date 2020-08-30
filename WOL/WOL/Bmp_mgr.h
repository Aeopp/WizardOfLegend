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
	// ��θ� DefaultPath �� �������ְ� �����̸��� Ȯ���ڸ� �˷��ּ���.
	std::weak_ptr<class Bmp> Insert_Bmp(std::wstring  FileName ,std::wstring ImageKey);
	std::weak_ptr<class Bmp> Find_Image_WP(std::wstring ImageKey);
	// �������� �ٱ����� �����մϴ�.
	std::shared_ptr<class Bmp> Find_Image_SP(std::wstring ImageKey); 

	void Release();

	 std::wstring DefaultPath = L"..\\..\\Resources\\WOL_TEXTURE\\";

	 static inline std::map<EDecoTileInfo, RECT> DecoTileInfoMap{};

	 static inline std::map<ETileSelect, std::pair<std::weak_ptr<class Bmp>, std::wstring>> ImageSelectMap{};
	 ETileSelect CurrentSelectImage{};
private:
	std::map<std::wstring, std::shared_ptr<class Bmp>>bmp_map;
};

