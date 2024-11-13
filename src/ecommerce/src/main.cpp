#include "core/index.hpp"
#include "utils/index.hpp"

const vector<Item> ITEMS = {
    {"Laptop", {99999, 100}},  {"Mouse", {1999, 100}},
    {"Keyboard", {2999, 100}}, {"Tablet", {49999, 100}},
    {"Phone", {59999, 100}},   {"Headphones", {19999, 100}}};

const vector<Voucher> VOUCHERS = {
    {"DISCOUNT_10", 10}, {"DISCOUNT_20", 20}, {"DISCOUNT_30", 30}};

int main() {
  Application* app = new Application();
  app->setItems(ITEMS);
  app->setVouchers(VOUCHERS);
  app->run();

  delete app;
  return 0;
}
