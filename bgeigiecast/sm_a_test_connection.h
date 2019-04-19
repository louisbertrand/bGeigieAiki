#ifndef BGEIGIECAST_SM_R_RR_TEST_CONNECTION_H
#define BGEIGIECAST_SM_R_RR_TEST_CONNECTION_H

#include "sm_a_state.h"

class TestConnectionState : public ApiReporterState {
 public:
  explicit TestConnectionState(IApiConnector& context, Reading* reading);
  virtual ~TestConnectionState() = default;

  void entry_action() override;
  void do_activity() override;
  void exit_action() override;
  void handle_event(Event_enum event_id) override;
 private:

};

#endif //BGEIGIECAST_SM_R_RR_TEST_CONNECTION_H
