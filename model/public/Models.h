#pragma once

#include "Model.h"

namespace Nuke
{
  class Models
  {
  public:
    Models() = default;


  private:
    std::vector<Model> models_{};
  };
}

/*
  GOAL:
  So far, I have been drawing models manually. That is, I store a variable for each
  model in global (Nuke) space, lazy initialize, update, and render using a draw call.

  It has been good for what I need right now, but this is extremely unscalable. for
  each model I make, I have to uphold 5 conditions right off the bat:
  1. Unique and relevant name
  2. Store in global space
  3. initialize
  4. update
  5. draw

  The least annoying ones are unique name and updating, but having to store,
  initialize, and draw is a drag and very error prone (already experienced
  bugs because of it).

  So, the plan is to have this handled by the engine. The engine should really
  be the one to care about this because I, as the user, don't care how it's
  managed. The only things I care about are:
  1. Shaders
  2. optimization
  3. model manipulation (specifying certain models to do actions upon)

  If I can't manipulate 1 and 3, and notice lacking in 2, there's a huge problem.
  While writing, it also seems that a render system is needed. The model manager
  should not be concerned with what shader the model is being drawn with. It's
  only concern is to store, initialize, and draw them (with a high level function).

  There is also a need for a way to manipulate the model without too much coupling
  with the class. The only thing I can think of is an event system to solve this
  problem, because having a function to access specific models is very cumbersome
  and raises the problem of trying to name anonymous vector objects (it doesn't work).

  I believe the correct strategy going forward is to have models listen for events, and
  when the events are dispatched from the queue, the event listeners are triggered. This
  allows for essentially no coupling as the model only needs to know what happened, not
  who did it.

  some prerequisites for effectively making this class would be:
  1. Render system
  2. Event system
  3. complete Model class

  Once these are complete, making the model manager should be trivial. 

  OUTLINE:
  The only function that should be in the public interface I can think of at the
  time of writing is a "load()" member function. 
*/