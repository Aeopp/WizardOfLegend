#pragma once

template<typename SubType>
class singleton_interface abstract
{
protected:
	singleton_interface() = default; 
	virtual ~singleton_interface()noexcept = default;
public:
	friend class std::shared_ptr<singleton_interface>;
	virtual void render(HDC hdc) {};
	virtual void update() {};
	virtual void initialize() {};
	virtual void release() {};
public:
	 static SubType& instance();
};

template<typename SubType>
inline SubType& singleton_interface<SubType>::instance()
{
	static std::once_flag _flag;

	static std::shared_ptr<SubType> _instance{ nullptr };

	static auto make_instance =
		[](auto& _instance_ptr) {
		_instance_ptr = std::make_shared<SubType>();
	};

	std::call_once(_flag, make_instance, _instance);

	return *_instance.get();
}
