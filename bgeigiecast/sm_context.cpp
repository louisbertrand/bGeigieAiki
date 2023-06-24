#include "debugger.h"
#include "sm_context.h"

Context::Context() : _current_state(nullptr), _event_queue() {
}

Context::~Context() {
  if(_current_state) {
    _current_state->exit_action();
    delete _current_state;
  }
}

void Context::set_state(State* state) {
    DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::set_state, current State* is %x, new State* is %x.\n", _current_state, state);DEBUG_FLUSH();
  if(_current_state) {
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::set_state, State* is %x and calling exit_action().\n", _current_state); DEBUG_FLUSH();
    _current_state->exit_action();
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::set_state, State* is %x and executing delete _current_state.\n", _current_state); DEBUG_FLUSH();
    delete _current_state;
  }
  _current_state = state;
  if(_current_state) {
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::set_state, State* is %x and calling entry_action().\n", _current_state); DEBUG_FLUSH();
    _current_state->entry_action();
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::set_state, State* is %x and returning from entry_action().\n", _current_state);  DEBUG_FLUSH();
  }
}

void Context::run() {
  if(!_current_state) {
    DEBUG_PRINTLN("Trying to run state machine with no active state");
    return;
  }
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::run, State* is %x and calling do_activity().\n", _current_state); DEBUG_FLUSH();
  _current_state->do_activity();
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTLN("In Context::run, calling handle_events()."); DEBUG_FLUSH();
  handle_events();
}

void Context::schedule_event(Event_enum event_id) {
  _event_queue.add(event_id);
}

State* Context::get_current_state() const {
  return _current_state;
}

void Context::clear_events() {
  _event_queue.clear();
}

void Context::handle_events() {
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::handle_events, State* is %x and checking event_queue.\n", _current_state); DEBUG_FLUSH();
  while(!_event_queue.empty()) {
    Event_enum event_id = _event_queue.get();
  DEBUG_PRINTF("millis=%d ", millis()); DEBUG_PRINTF("In Context::handle_events, State* is %x and calling _current_state->handle_event(%d).\n", _current_state, event_id); DEBUG_FLUSH();
    if(_current_state) { _current_state->handle_event(event_id); }
  }
}
