#include "app.h"
#include "control_base.h"
#include <sstream>
#include "label.h"
#include "Ctrls.h"

int main(int argc,char ** argv) {
  app a;

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

  c->onEnter = [&]()->void {
    c->setBkColor(rgb(0, 220, 0));
    };
  c->onLeave = [&]()->void {
    c->setBkColor(rgb(220, 220, 220));
    };
  b->onEnter = [&]()->void {
    b->setBkColor(rgb(255, 0, 0));
    };
  b->onLeave = [&]()->void {
    b->setBkColor(rgb(220, 220, 220));
    };

  a.addChild(c);
  return a.run();
}