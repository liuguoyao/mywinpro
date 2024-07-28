#include "app.h"
#include "control_base.h"
#include <sstream>


int main(int argc,char ** argv) {
  app a;

  control_base c(L"c");
  auto c1 = a.addChild(c);
  c1->resize(60, 30);
  c1->setposition(10, 10);
  c1->onEnter = [&]()->void {
    auto c1 = a.findControlByName(L"c");
    c1->setBkColor(rgb(0, 220, 0));
    };
  c1->onLeave = [&]()->void {
    auto c1f = a.findControlByName(L"c");
    c1f->setBkColor(rgb(220, 220, 220));
    };

  control_base c_2(L"c2");
  c_2.onEnter = [&]()->void {
    auto c2f = a.findControlByName(L"c2");
    c2f->setBkColor(rgb(0, 220, 0));
    };
  auto c2 = a.addChild(c_2);
  c2->resize(60, 30);
  c2->setposition(50, 10);

  c2->onLeave = [&]()->void {
    auto c2f = a.findControlByName(L"c2");
    c2f->setBkColor(rgb(220, 220, 220));
    };

  control_base c_3(L"c3");
  auto c3 = a.addChild(c_3);
  c3->resize(60, 30);
  c3->setposition(90, 10);
  c3->onEnter=[&] ()->void{
    auto c2 = a.findControlByName(L"c3");
    c2->setBkColor(rgb(255, 0, 0));
	  };
  c3->onLeave = [&]()->void {
    auto c2f = a.findControlByName(L"c3");
    c2f->setBkColor(rgb(220, 220, 220));
    };

  return a.run();
}