// This is the main file of the project.

#include "Bot.hpp"
#include "MyUtils.hpp"

int main(void) {
  Bot bot;
  try {
    bot.Start();
  } catch (MyExceptions& e) {
    std::cerr << e.What() << '\n';
    bot.Stop();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    bot.Stop();
  } catch (...) {
    std::cerr << "Unknown exception caught\n";
    bot.Stop();
  }
  return 0;
}