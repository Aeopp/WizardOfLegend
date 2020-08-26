#pragma once
#include "singleton_interface.h"

class Bmp_mgr : public singleton_interface<Bmp_mgr>
{
public :
	void initialize()override;
	void release() override;

	virtual ~Bmp_mgr()noexcept; 
public:
	// ��θ� DefaultPath �� �������ְ� �����̸��� Ȯ���ڸ� �˷��ּ���.
	std::weak_ptr<class Bmp> Insert_Bmp(std::wstring  FileName ,std::wstring ImageKey);
	std::weak_ptr<class Bmp> Find_Image(std::wstring ImageKey);
	void Release();

	 std::wstring DefaultPath = L"..\\..\\Resources\\WOL_TEXTURE\\";
private:
	std::map<std::wstring, std::shared_ptr<class Bmp>>bmp_map;
};

