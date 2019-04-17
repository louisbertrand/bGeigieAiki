#ifndef BGEIGIE_POINTCAST_CONNECTEDSTATE_H
#define BGEIGIE_POINTCAST_CONNECTEDSTATE_H

#include "sm_c_active_fixed.h"

class ConnectedState: public FixedModeState {
 public:
  explicit ConnectedState(Controller& context);
  virtual ~ConnectedState() = default;

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;

};

#endif //BGEIGIE_POINTCAST_CONNECTEDSTATE_H
