#ifndef BGEIGIECAST_CONTROLLER_HPP
#define BGEIGIECAST_CONTROLLER_HPP

#include <Handler.hpp>
#include <Aggregator.hpp>

#ifndef ARDUINO_M5STACK_Core2
#include "button.h"
#endif

#include "sm_context.h"
#include "local_storage.h"

/**
 * Main controller for the system, implements state machine to run
 */
class Controller : public Context, public Aggregator, private Handler, private Worker<bool> {
#ifndef ARDUINO_M5STACK_Core2
class Controller : private ButtonObserver, public Context, public Aggregator, private Handler, private Worker<bool> {
#endif
 public:

  typedef enum {
    k_savable_MobileMode,
    k_savable_FixedMode
  } SavableState;

  Controller(LocalStorage& config);
  virtual ~Controller() = default;

  /**
   * Set initial state for the state machine,
   */
  void setup_state_machine();

  /**
   * Override the context run to also run the reporter state machine
   */
  void run() override;

#ifndef ARDUINO_M5STACK_Core2
  /**
   * Callback for the button
   */
  void on_button_pressed(Button* button, uint32_t millis) override;
#endif

  /**
   * override set state from context, to flag worker that change has been made
   * @param state
   */
  void set_state(State* state) override;

 protected:
  int8_t handle_produced_work(const worker_status_t& worker_reports) override;
 private:
  int8_t produce_data() override;
 private:

  /**
   * Reset and restart the system
   */
  void reset_system();

  /**
   * Save a state to the memory
   * @param state
   */
  void save_state(SavableState state);

  /**
   * get the saved state from the memory
   */
  SavableState get_saved_state();

  /**
   * Initialize the controller and all of its components
   */
  void initialize();

  LocalStorage& _config;
#ifndef ARDUINO_M5STACK_Core2
  Button _mode_button;
#endif
  bool _state_changed;

  friend class InitializeState;
  friend class PostInitializeState;
  friend class ConfigurationModeState;
  friend class MobileModeState;
  friend class FixedModeState;
  friend class ResetState;
};

#endif //BGEIGIECAST_CONTROLLER_HPP
