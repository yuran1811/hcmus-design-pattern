#include "core/index.hpp"
#include "utils/index.hpp"

const vector<Item> ITEMS = {
    {"Laptop", {99999, 2}}, {"Mouse", {1999, 2}},  {"Keyboard", {2999, 2}},
    {"Tablet", {49999, 2}}, {"Phone", {59999, 2}}, {"Headphones", {19999, 2}}};

const time_t MOCK_TIME = time(nullptr) + 3;

const vector<Coupon> COUPONS = {{"DISCOUNT_10", 10, false, MOCK_TIME, 10},
                                {"DISCOUNT_20", 20, false, MOCK_TIME, 10},
                                {"DISCOUNT_30", 30, false, MOCK_TIME, 10}};

int main() {
  Application* app = new Application();
  app->setItems(ITEMS);
  app->setCoupons(COUPONS);
  app->run();

  delete app;
  return 0;
}
