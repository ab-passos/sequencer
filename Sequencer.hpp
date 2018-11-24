#pragma once

#include <map>
#include <vector>
#include <memory>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "boost/uuid/uuid_io.hpp"

using ActionId = boost::uuids::uuid;
using InstanceId = boost::uuids::uuid;
using SequenceId = boost::uuids::uuid;

using SequenceOfActions = std::vector<ActionId>;

class PlugIn {
public:
  virtual void execute(const ActionId& actionId) = 0;
  virtual ~PlugIn() = default;
};

using PlugInPtr = std::unique_ptr<PlugIn>;

struct InstanceDetails {
  const std::string& instanceName_;
  const PlugInPtr& plugIn_;

  InstanceDetails(
    const std::string& instanceName,
    const PlugInPtr& plugIn)
    : instanceName_(instanceName)
    , plugIn_(plugIn)
    {}
};

class Sequence {
public:
  virtual InstanceId registerInstance(
    const std::string&,
    const PlugInPtr&) = 0;

  virtual ActionId registerAction(
    const InstanceId& instanceId) = 0;

  virtual SequenceId registerSequnce(
    const SequenceOfActions& sequenceOfActions) = 0;

  virtual void run(const SequenceId& sequnceId) = 0;

  virtual ~Sequence() = default;
};

class SequenceImpl : public Sequence{
public:
  SequenceImpl() = default;

  InstanceId registerInstance(
    const std::string& instanceName_,
    const PlugInPtr& plugIn_) final {

      auto instanceId = boost::uuids::random_generator()();
      instances.insert(
        std::pair<InstanceId, InstanceDetails>(
          instanceId,
          InstanceDetails(instanceName_, plugIn_)
        ));

      return instanceId;
  }

  ActionId registerAction(const InstanceId& instanceId) final {
    auto actionId = boost::uuids::random_generator()();
    actions.insert(
      std::pair<ActionId, InstanceId>(
          actionId,
          instanceId
      ));
    return actionId;
  }

  SequenceId registerSequnce(
    const SequenceOfActions& sequenceOfActions) final {
      auto sequenceId = boost::uuids::random_generator()();
      sequence.insert(
        std::pair<SequenceId, SequenceOfActions>(
            sequenceId,
            sequenceOfActions
        ));

      return sequenceId;
  }

  void run(const SequenceId& sequnceId) {
    const auto& s = sequence[sequnceId];
    for(auto&& elem : s) {
      //std::cout << "Actions" << std::endl;
      //std::cout << elem << std::endl;
      //std::cout << "---" << std::endl;
      auto instanceId = actions[elem];
      const auto& ins = instances.at(instanceId);
      (ins.plugIn_)->execute(elem);
    }
  }

private:
  std::map<InstanceId, InstanceDetails> instances;
  std::map<ActionId, InstanceId> actions;
  std::map<SequenceId, SequenceOfActions> sequence;

};

using SequencePtr = std::unique_ptr<Sequence>;

class SequenceFactory {
public:
  virtual SequencePtr createSequencer() const = 0;
};

using SequenceFactoryPtr = std::unique_ptr<SequenceFactory>;

class SequenceFactoryImpl : public SequenceFactory {
public:
  SequencePtr createSequencer() const {
    return SequencePtr(new SequenceImpl());
  }
};
