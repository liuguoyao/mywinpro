#include "app.h"
#include "control_base.h"
#include "global_var.h"
#include <sstream>
#include "label.h"
#include "Ctrls.h"

int main(int argc,char ** argv) {
  app& a = app::getInstance();

  auto l = Ctrls::instance().create_hlayout(L"hlayout", nullptr);
  auto parent = l;
  auto c = Ctrls::instance().create_label(L"c", parent);
  //control_base* parent = c;
  auto b = Ctrls::instance().create_base(L"label", parent);
  auto d = Ctrls::instance().create_label(L"labeld", parent);
  auto e = Ctrls::instance().create_label(L"labele", parent);
  auto f = Ctrls::instance().create_button(L"button", parent);
  auto g = Ctrls::instance().create_edit(L"edit", parent);
  auto h = Ctrls::instance().create_edit(L"edit1", parent);

  c->resize(30, 30);
  b->resize(60, 30);
  d->resize(60, 30);
  e->resize(60, 30);
  f->resize(60, 30);
  g->resize(260, 30);
  h->resize(60, 30);

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

  //a.addChild(parent);
  a.addLayout(parent);

  return a.run();
}