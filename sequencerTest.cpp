#include "Sequencer.hpp"
#include "PlugInTis.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockTisActions {
public:
  MOCK_METHOD0(PlanTisScan, void(void));
  MOCK_METHOD0(QueueTisScan, void(void));
  MOCK_METHOD0(ModelTisScan, void(void));
};

class SequencerTest : public ::testing::Test {
public:
  SequencerTest()
  : factory()
  , plugInName("Test PlugIn")
  , sequencePtr(factory.createSequencer())
  , actionPtr(new ActionsImpl())
  , plugInPtr(new PlugInTis(plugInName, actionPtr))
  {}

 protected:
  SequenceFactoryImpl factory;
  std::string plugInName;
  SequencePtr sequencePtr;
  ActionsPtr actionPtr;
  PlugInPtr plugInPtr;
  MockTisActions mockTisActions;
};

TEST_F(SequencerTest, Init) {
}

TEST_F(SequencerTest, RegisterActionDoesnTriggerAction) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action, [&]{mockTisActions.PlanTisScan();});
  EXPECT_CALL(mockTisActions, PlanTisScan())
     .Times(0);
}

TEST_F(SequencerTest, RegisterSequenceDoesnTriggerAction) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action, [&]{mockTisActions.PlanTisScan();});
  EXPECT_CALL(mockTisActions, PlanTisScan())
     .Times(0);

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);
}

TEST_F(SequencerTest, ExecuteOnlyOneAction) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action, [&]{mockTisActions.PlanTisScan();});
  EXPECT_CALL(mockTisActions, PlanTisScan())
     .Times(1);

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
}

TEST_F(SequencerTest, ExecuteOnlyTwoActionWithSameActionId) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action, [&]{
    mockTisActions.PlanTisScan();
    mockTisActions.QueueTisScan();});

  EXPECT_CALL(mockTisActions, PlanTisScan())
     .Times(1);
  EXPECT_CALL(mockTisActions, QueueTisScan())
     .Times(1);

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
}

TEST_F(SequencerTest, ExecuteTwoActionIds) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action1 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action1, [&]{
    mockTisActions.PlanTisScan();});

  auto action2 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action2, [&]{
      mockTisActions.QueueTisScan();});

  EXPECT_CALL(mockTisActions, PlanTisScan())
     .Times(1);
  EXPECT_CALL(mockTisActions, QueueTisScan())
     .Times(1);

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action1);
  sequenceOfActions.push_back(action2);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
}

TEST_F(SequencerTest, ExecuteRightOrder) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action1 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action1, [&]{
    mockTisActions.PlanTisScan();});

  auto action2 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action2, [&]{
      mockTisActions.QueueTisScan();});

  auto action3 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action3, [&]{
      mockTisActions.ModelTisScan();});

  {
     ::testing::InSequence s;
     EXPECT_CALL(mockTisActions, PlanTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, QueueTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, ModelTisScan())
      .Times(1);
   }

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action1);
  sequenceOfActions.push_back(action2);
  sequenceOfActions.push_back(action3);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
}

TEST_F(SequencerTest, ExecuteReverseOrder) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action1 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action1, [&]{
    mockTisActions.PlanTisScan();});

  auto action2 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action2, [&]{
      mockTisActions.QueueTisScan();});

  auto action3 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action3, [&]{
      mockTisActions.ModelTisScan();});

  {
     ::testing::InSequence s;
     EXPECT_CALL(mockTisActions, ModelTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, QueueTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, PlanTisScan())
      .Times(1);
   }

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action3);
  sequenceOfActions.push_back(action2);
  sequenceOfActions.push_back(action1);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
}

TEST_F(SequencerTest, RunSameSequenceTwice) {
  auto instanceId = sequencePtr->registerInstance(plugInName, plugInPtr);
  auto action1 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action1, [&]{
    mockTisActions.PlanTisScan();});

  auto action2 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action2, [&]{
      mockTisActions.QueueTisScan();});

  auto action3 = sequencePtr->registerAction(instanceId);
  actionPtr->addAction(action3, [&]{
      mockTisActions.ModelTisScan();});

  {
     ::testing::InSequence s;
     EXPECT_CALL(mockTisActions, PlanTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, QueueTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, ModelTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, PlanTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, QueueTisScan())
      .Times(1);
     EXPECT_CALL(mockTisActions, ModelTisScan())
      .Times(1);
   }

  SequenceOfActions sequenceOfActions;
  sequenceOfActions.push_back(action1);
  sequenceOfActions.push_back(action2);
  sequenceOfActions.push_back(action3);
  auto sequenceId = sequencePtr->registerSequnce(sequenceOfActions);

  sequencePtr->run(sequenceId);
  sequencePtr->run(sequenceId);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
