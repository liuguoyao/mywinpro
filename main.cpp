#include "app.h"
#include "control_base.h"
#include "global_var.h"
#include <sstream>
#include "label.h"
#include "Ctrls.h"

int main(int argc,char ** argv) {
  app& a = app::getInstance();

  auto c = Ctrls::instance().create_label(L"c", nullptr);
  auto b = Ctrls::instance().create_base(L"label", c);
  auto d = Ctrls::instance().create_label(L"labeld", c);
  auto e = Ctrls::instance().create_label(L"labele", c);
  auto f = Ctrls::instance().create_button(L"button", c);
  
  
  c->resize(60, 30);
  c->setposition(10, 10);

  b->resize(60, 30);
  b->setposition(50, 10);

  d->resize(60, 30);
  d->setposition(90, 10);
  e->resize(60, 30);
  e->setposition(230, 10);
  f->resize(60, 30);
  f->setposition(270, 10);

  //c->onEnter = [&]()->void {
  //  c->setBkColor(::hoverColor);
  //  };
  //c->onLeave = [&]()->void {
  //  c->setBkColor(::backgroundColor);
  //  };
  //b->onEnter = [&]()->void {
  //  b->setBkColor(::hoverColor);
  //  };
  //b->onLeave = [&]()->void {
  //  b->setBkColor(::backgroundColor);
  //  };

  a.addChild(c);
  //f->onLButtonDown = []() {
  //  OutputDebugString(L"button click\n");
  //  };

  return a.run();
}