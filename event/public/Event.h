#pragma once

#include <utility>
#include <typeindex>
#include <functional>
#include <vector>
#include <unordered_map>
#include <any>

namespace Events
{
  class Bus
  {
  public:
    /*
      this bus system does NOT use polymorphism to work. It uses typeid()
      to differentiate structs/classes. 
    */

    template <typename Event>
    using Listener = std::function<void(const Event&)>;

    template <typename Event>
    void subscribe(Listener<Event> listener) 
    {
      //finds specific "bucket" that return vector of listeners for
      //that specific type of event.
      //(or creates new one and returns it)
      auto& bucket = listeners_[typeid(Event)]; 

      //pushes function call into lamba
      bucket.push_back([listener](const void* e) //this is void as template 
        {                                        //map-values are disallowed.
          listener(*static_cast<const Event*>(e)); //casted to correct event when
        });                                        //called later

      /* 
        this systems works by storing vectors of listeners of different types.
        it completely relies on typeid() as a key for the unordered map to find
        the value pair. 

        as an overview:

        subscribe -> find bucket (or create new one) ->
        store lambda with const void* parameter type that calls the listener in bucket

        emit -> find bucket (early return if it doesn't exist) ->
        traverse through vector (the value of the key-value pair) ->
        call lambda function in vector with argument of address of event ->
        event gets converted to const void* ->
        event gets static_cast'ed to type specified in lambda ->
        calls actual function
      */
    }

    template <typename Event>
    void emit(const Event& event)
    {
      //find bucket type
      auto it = listeners_.find(typeid(Event));

      //check if it existed
      if (it == listeners_.end())
        return;

      //go through each lambda and call listeners with event argument
      for (auto& listener : it->second)
        listener(&event);
    }

    void push(std::any&& a)
    {
      events_.emplace_back(std::move(a));
    }

  private:
    //accepting the overhead of std::function
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> listeners_{};
    /*
       in the future, if the overhead is simply to much and is a bottleneck
       for performance, I might have to resort to "hot paths". hot paths are 
       making seperate buses for each different type of event.

       so that would mean having a bus for physics events, player events, etc. 

       a possible solution if performance is critical is to have one hot path fo that 
       specific event, and still keep the rest on the general bus. 
    */
    std::vector<std::any> events_{};
  };

  /*
    major fixing needed here. I need to design a better event loop. 
    scope:
    I need to build an object that lets functions subscribe for when
    a specific type of event is emitted. However, I want the events, 
    when they are called, to be stored in an event queue. In the update 
    phase, the events stored in the event queue should be emitted to
    the RIGHT listeners. 

    - event queue
    - passing arguments to correct listeners
    - allow functions to subscribe
    - allow a general type of events. 
  */
}