#pragma once

class TISModel {
public:

  void modelRA(int value) {
    std::cout << "value is: " << value << std::endl;
  }

};

class TIS{
public:

  void plan_tis_scan() {
    std::cout << "Does some planning" << std::endl;
  }

  void execute_tis_scan() {
    tis_scan_result = 5;
  }

  void model_tis_scan() {
    std::cout << "The tis scan model result is: " << tis_scan_result << std::endl;
  }

  void print() {
    std::cout << "Just print something" << std::endl;
  }

private:
  int tis_scan_result;
};
