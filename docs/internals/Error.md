# Error Handling

## Overview

Exceptions are a nice utility for when something _truly_ exceptional happens. However, in my personal opinion, exceptions seem like they fall within a class of spaghetti code. 

How do you know what type an exception is returning from a function from which you haven't ran through the implementation? Unless you've been told what it is, or looked at the error messages _after_ an exception has been thrown, the simply answer is: you can't. 

How do you know if a function _doesn't_ throw an exception? You might be thinking: "Well, obviously, if it's marked `noexcept`, then it won't throw exceptions!" This couldn't be further from the truth. For some reason, `noexcept` is not a _contract_ with the compiler, it is a _promise_. So, even if a function is marked `noexcept`, it can still throw exceptions (but instead causing the program to terminate). 

Since this design makes my stomach churn when thinking about it, I've decided to resort to a different kind of error handling within the engine for circumstances that aren't exceptional. Examples of errors that are handleable: file system errors, shader compilation errors, input errors, etc. Errors that can be handled at the call site, or further up the ladder.

The solution to this is `std::expected` mixed with class-nested `enum class` error codes. This design, in my opinion, is the current best alternative to exceptions. First, `enum class`'s (declared with a cheap type) are inherently cheap. Not only being cheap, but paired with `switch` statements, handling which error correction method for an error is blazingly fast compared to `if else` statements. 

Also, since the enum is nested within a class, it won't conflict with other class's declarations of error codes. This added benefit also makes the code self documenting. 

To return an error code inside a class's function using this method, the line would look something like:

>return std::unexpected{ Error::missing };

Very clean and direct. Something unexpected happened, and the error is that something is missing. 

From the callers perspective, the return type from the `std::expected` would be:

>ExampleClass::Error

and if it returned an unexpected value, the caller could deduce what error happened by using switch statement cases like:

> ...  
>case ExampleClass::Error::missing:  
>handle_missing();  
break;  
>...

Extremely clear what happened and what was done to solve it. Unlike exceptions, I can see exactly what the function returned as an error code how it was solved.

The nice thing about these too is that these error codes are not invasive. If an error is returned, the caller can decide if they even want to handle it or not. With exceptions, the exception must be caught, or else it will unwind the entire stack until it has found a catcher, or the program with terminate.

Finally, the last thing I will mention in the overview is that these are extremely cheap too. Most implementations have an `enum class`'s underlying type to be `int`. We can guarantee that it is a 32-bit int by making the `enum class` as follows:

>enum class : std::int32_t  
>...

So, with whatever type is returned, there is a small integer paired with it as an error code. Even in higher-performance parts of the code, this can be viable (still would avoid putting this anywhere in the rendering parts of the engine, unless unavoidable).

## Requirements

For this section, I'll go over the requirements that each class must have if they want to adopt this pattern.