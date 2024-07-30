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


Ctrls::Ctrls()
{
}
