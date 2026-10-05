#include <iostream>
#include <random>
#include <string>
#include <string_view>

constexpr std::string_view str =
    "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789";

std::string get_random(size_t len) {
  thread_local std::mt19937_64 rng{std::random_device{}()};
  std::uniform_int_distribution<size_t> dist(0, str.size() - 1);
  std::string pass;
  pass.reserve(len);
  for (size_t i = 0; i < len; i++) {
    pass += str[dist(rng)];
  }
  return pass;
}

int main() {
  std::cout << "Введите длину пароля: ";
  int raw_len;
  if (!(std::cin >> raw_len)) {
    std::cerr << "Не удалось ввести число" << "\n";
    return 1;
  }
  if (raw_len <= 0 || raw_len > 4096) {
    std::cerr << "Введите число больше нуля и меньше 4096" << "\n";
    return 1;
  }
  size_t len = static_cast<size_t>(raw_len);
  std::cout << "Пароль: " << get_random(len) << "\n";
  return 0;
}
