#include "app.h"
#include "control_base.h"

int main(int argc,char ** argv) {
  app a;

  control_base c;
  
  auto c1 = a.addChild(c);
  c1->resize(100, 100);
  c1->setposition(10, 10);

  control_base c_2;
  auto c2 = c1->addChild(c_2);
  c2->resize(80, 80);
  c2->setposition(10, 10);

  control_base c_3;
  auto c3 = c2->addChild(c_3);
  c3->resize(60, 60);
  c3->setposition(10, 10);

  return a.run();
}