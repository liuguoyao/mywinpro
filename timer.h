#ifndef _H_TIMER_H
#define _H_TIMER_H
#include <functional>
class timer
{
public:
  timer(int expire_ticks, int runtype,std::function<void()> callback_fun);
  void update_expire_ticks(uint64_t delta_ticks);
  void start();
  bool is_running();
  void stop();
private:
  int expire_ticks;
  int expire_ticks_origin;
  std::function<void()> callback_fun;
  int runtype;
  bool running;
};

#endif // !1

