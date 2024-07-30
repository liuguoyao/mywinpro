#ifndef _Ctrls_H_
#define _Ctrls_H_

#include "control_base.h"
#include "label.h"
#include <map>
#include <memory>

class Ctrls {
public:
	static Ctrls& instance();
	Ctrls(const Ctrls&) = delete;
	Ctrls& operator=(const Ctrls&) = delete;
	control_base* create_base(std::wstring name, control_base* parent);
	label* create_label(std::wstring name,control_base* parent = nullptr);
private:
	Ctrls();
	std::map<std::wstring, std::unique_ptr<control_base>> m_controls;

};

#endif