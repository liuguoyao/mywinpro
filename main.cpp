#include "app.h"
#include "control_base.h"

int main(int argc,char ** argv) {
  app a;

  control_base c;
  
  auto c1 = a.addChild(c);
  c1->resize(60, 30);
  c1->setposition(10, 10);

  //control_base c_2;
  //c_2.resize(60, 30);
  //c_2.setposition(50, 10);
  //auto c2 = a.addChild(c_2);


  //control_base c_3;
  //auto c3 = a.addChild(c_3);
  //c3->resize(60, 30);
  //c3->setposition(90, 10);

  return a.run();
}