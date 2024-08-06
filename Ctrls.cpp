#include "Ctrls.h"


Ctrls& Ctrls::instance()
{
  static Ctrls m_instance ;
  return m_instance;
}

label* Ctrls::create_label(std::wstring name, control_base* parent)
{
  if (m_controls.find(name) == m_controls.end()) {
    m_controls[name] = std::move(std::make_unique<label>(name, parent));
  }
  
  return (label*)m_controls[name].get();
}

button* Ctrls::create_button(std::wstring name, control_base* parent)
{
  if (m_controls.find(name) == m_controls.end()) {
    m_controls[name] = std::move(std::make_unique<button>(name, parent));
  }

  return (button*)m_controls[name].get();
}

edit* Ctrls::create_edit(std::wstring name, control_base* parent)
{
  if (m_controls.find(name) == m_controls.end()) {
    m_controls[name] = std::move(std::make_unique<edit>(name, parent));
  }

  return (edit*)m_controls[name].get();
}


hlayout* Ctrls::create_hlayout(std::wstring name, control_base* parent)
{
  if (m_controls.find(name) == m_controls.end()) {
    m_controls[name] = std::move(std::make_unique<hlayout>(name, parent));
  }

  return (hlayout*)m_controls[name].get();
}

control_base* Ctrls::create_base(std::wstring name, control_base* parent)
{
  if (m_controls.find(name) == m_controls.end()) {
    m_controls[name] = std::move(std::make_unique<control_base>(name, parent));
  }

  return (control_base*)m_controls[name].get();
}


Ctrls::Ctrls()
{
}
