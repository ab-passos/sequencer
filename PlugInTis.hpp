#pragma once

class Actions {
public:
  virtual void execute(const ActionId& actionId) = 0;

  virtual void addAction(
    const ActionId& actionId,
    std::function<void(void)>f) = 0;

  virtual ~Actions() = default;
};

using ActionsPtr = std::unique_ptr<Actions>;

class ActionsImpl : public Actions {
public:

  void addAction(
    const ActionId& actionId,
    std::function<void(void)>f) final {
      function_map.insert(
        std::pair<ActionId, std::function<void(void)>>(
            actionId,
            f
        ));
    }

  void execute(const ActionId& actionId) final {
      function_map.at(actionId)();
  }

private:
  std::map<ActionId, std::function<void(void)>> function_map;
};

class PlugInTis : public PlugIn {
public:
  PlugInTis(
    const std::string& name,
    const ActionsPtr& actions)
  : name_{name}
  , actions_{actions}
  {}

  void execute(const ActionId& actionId)
  {
    actions_->execute(actionId);
  }

  private:
    const std::string& name_;
    const ActionsPtr& actions_;
};
