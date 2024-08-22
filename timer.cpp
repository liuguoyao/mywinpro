#include "timer.h"
#include "app.h"

timer::timer(int expire_ticks,int runtype,std::function<void()> callback_fun)
{
    this->expire_ticks = expire_ticks;
    this->expire_ticks_origin = expire_ticks;
    this->callback_fun = callback_fun;
    this->runtype = runtype;
    running = false;
    APP.add_timer(this);
}

void timer::update_expire_ticks(uint64_t delta_ticks)
{
  if (!is_running()) return;
  expire_ticks-=(int)delta_ticks;
  if(expire_ticks<=0)
  {
      callback_fun();
      if(runtype==ONCE)
      {
          APP.remove_timer(this);
      }
      else
      {
          expire_ticks = expire_ticks_origin;
      }
  }
}

void timer::start()
{
    running = true;
}

bool timer::is_running()
{
  return running;
}

void timer::stop()
{
    running = false;
}
