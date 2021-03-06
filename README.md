# GML-Modding-SDK

Tutorial can be found in `Tutorial` !!!
Or as a video here:
https://www.youtube.com/watch?v=hQ7mBLIGf6U

## Huh?
Yep. This is (becoming) an extension for making games made in GameMaker moddable!

You will be able to mod any game that includes this extension in runtime via DLL-injection.
## (Game Developer) I want to make my game moddable! How?
Currently, it is as easy as drag-and dropping the extension package (which can be found in the releases tab)
onto the GameMaker IDE window and importing everything

## (Modder) I want to mod a game using this SDK!
This requires you to understand C++.
I made a namespace called `gml::` which holds the functions from GameMaker.

The APIENTRY functions contains a method called `executePatching()` which gets called on-injection of the DLL.
You can either put your code there OR create a thread that runs the code parallel to the game.

If you understand programming you will eventually figure out what is smart and what not.

## Information
This is STRONGLY dependent on YAL's repo "function_get_address (https://github.com/YAL-GameMaker/function_get_address)", 
which allowed you to get the addresses of GML-functions in runtime.
This right here is me taking his work and expanding it to present it in a 'less hacky way'.
*Currently WIP and more like a proof-of-concept*

Yes, this allows you to use GML-code in C++
Original readme can be found below because I don't wanna reinvent the wheel.



# function_get_address
Supported versions:

* GameMaker: Studio: all versions released after 1.4.1773 (object value type wasn't supported before then)
* GameMaker Studio 2: all versions before 2.2.2 (2.2.1.291 is the last one where this works)

## What is this
This trick allows to acquire a pointer to a native GML function, as result allowing you to pass it to a native extension
and have the later call it - for example, your extensions now can suddenly make buffers, or data structures, or even call
GML code (to an extent)! A true extension rennaisance for the affected versions.

## How this works
Let's take a look at the heart of the system, the `function_get_closure` script
```js
var name = argument0;
variable_global_set(name, variable_global_get(name));
return ptr(variable_global_get(name));
```
So you might be wondering what is even going on here.

In what is most likely to be what remains of some delayed or scrapped feature,
recent versions of GameMaker _kind of_ support C closures for built-in functions.

Except you can't get them because the global scope isn't populated with built-in function names.

However, as an accident revealed, the way the built-in functions are expected to be set into scope,
is that the scope needs variables defined with names but no values - not just `undefined`, but the
special kind of value that resides in variables that were never assigned (`kind==VALUE_UNSET` on YYGML.h terms).
And you can't just get such a value by passing an nonexistent variable to a function (which would give you a read error) -
the value must come from a GML function call.

This is where `variable_global_get` comes into play - due to a bug that hadn't been fixed until GMS2.2.2,
if a nonexistent variable is being read via the function, it would return the exact thing that we need.

And thus, `variable_global_set(name, variable_global_get(name));` will generate an index for a global variable
while keeping it amiss, which allows the subsequent call to create a closure and return it, which we can get a pointer of.

Then, on C side we can grab the actual function pointer from the closure, call (or store) it accordingly, and voila!

We can then add a bit of caching and a couple C++ wrappers to make it all nicer.

## Limitations
* You cannot get a pointer to `script_execute` specifically because it got optimized and is no longer a function as such
  (that's why it's so fast)
* It is unclear whether and how you could get pointers to GML instances for use in C code.  
  For the most part, you don't need to, as you can use variable_instance_ functions to work with instances,
  but calling instance-specific functions (which require valid self/other pointers) is problematic without these.
* You can call GML code via `event_perform_object`, but you'll need to construct something resembling a valid `GMLInstance` first
  (feel free to research alternatives).

## Special thanks
Saturnyoshi for gml.h file (as seen in [UGMMS](https://github.com/Saturnyoshi/UGMMS)),
which allowed me to not write another one of these for this mini-demo.

## If anyone from YoYo Games is reading this
A built-in `function_get_address` would be nice, even if it was native-only (no HTML5) until further notice
(besides, it's pretty trivial to get function pointers in JS anyway).
