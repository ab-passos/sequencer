#include <iostream>
#include <map>
#include <memory>
#include "Sequencer.hpp"
#include "TisActions.hpp"
#include "PlugInTis.hpp"
#include "boost/uuid/uuid_io.hpp"



int main() {
  std::cout << "Starting Client!" << std::endl;
  SequenceFactoryImpl factory;
  SequencePtr sequence = factory.createSequencer();
  std::string plugInName = "Test";

  ActionsPtr actionPtr = ActionsPtr(new ActionsImpl());
  std::unique_ptr<PlugIn> plugIn = std::unique_ptr<PlugIn>(
    new PlugInTis(plugInName, actionPtr));

  auto instanceId = sequence->registerInstance(plugInName, plugIn);

  std::cout << instanceId << std::endl;

  TIS tisActions;
  auto action1 = sequence->registerAction(instanceId);
  std::cout << "action " << action1 << std::endl;
  actionPtr->addAction(action1, [&]{tisActions.plan_tis_scan();
                                    tisActions.execute_tis_scan();});

  auto action2 = sequence->registerAction(instanceId);
  std::cout << "action " << action2 << std::endl;
  actionPtr->addAction(action2, [&]{tisActions.model_tis_scan();});

  auto action3 = sequence->registerAction(instanceId);
  std::cout << "action " << action3 << std::endl;
  actionPtr->addAction(action3, [&]{tisActions.print();});

  TISModel tismodel;

  auto action4 = sequence->registerAction(instanceId);
  std::cout << "action " << action4 << std::endl;
  actionPtr->addAction(action4, [&]{tismodel.modelRA(9999);});

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action1);
  sequenceOfActions.push_back(action2);
  sequenceOfActions.push_back(action3);
  sequenceOfActions.push_back(action4);

  auto sequenceId = sequence->registerSequnce(sequenceOfActions);
  std::cout << sequenceId << std::endl;
  sequence->run(sequenceId);
  std::cout << "END" << std::endl;

}
