# Tutorial

This is a step by step guide on how to use this in your game!

## Step 0: Using the right GameMaker Version:
*Supported versions are:*

    GameMaker: Studio: all versions released after 1.4.1773 (...)
    GameMaker Studio 2: all versions before 2.2.2 (2.2.1.291 is the last one where this works)
Downgrade your version if necessary.

WARNING: If your game relies on structs, I do not recommend downgrading.

## Step 1: Getting the thing
To import the SDK, just head to the `releases tab` on the right side of the page, download the newest version and import it into your game.

    IMAGE 1

Also, make sure to import the `scripts` from the extension

    image 2

Now, your project is set up! 

## Step 2: Configuring the SDK

Of course, depending on your game, you want to allow only certain functions to be used in mods (`instance_create(), show_debug_message(), etc..`).
 
To configure this, head to the `scripts -> initializing -> populateAddresses` script and open it.

You want to put all functions the mods should be able to use into this script. Syntax is given.

     IMAGE 3

## Step 3: How it works

Notice how you didn't need to change or add any objects?

This is because the script `pragma_holder` contains a `gml_pragma()` magic function! It automatically compiles it into the game and runs it as the first thing, before any other code in the game.

## Developer Tutorial End!
Congrats! You made your game moddable!
To check out what you have to do as a modder, keep reading!

# POV: You are a modder.
## Modding Step 0: Getting the thing
As a modder, the first thing you wanna do is, clone the repo / VisualStudio 2019 project and open it.

## Step 1: Switching modes
Now, when your project is loaded, open the `header file "config.h"` and comment the

    #define NATIVE_GAME_SDK // ...

so it looks like 

    //#define NATIVE_GAME_SDK // ...

This flips the switch on the project and now indicates that you are indeed using this to develop a mod for a game!

If you are leaving this active, the project will be built as the GameMaker Extension DLL, which is not what we want in this case.

## Step 2: Closer look

Taking a look at `functions.h` we can see a bunch of GML-like looking functions.
These are the ones we will use later on and they will get called in the GameMaker executable.

## Step 3: Writing our mod

The real magic part comes from the file `function_get_address.cpp`
(Which was originally written by YellowAfterlife iirc).

We are looking for the function `BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)`

which is in line 152 as of writing this.
This function gets called when the DLL is later on attached to the game's process and we will execute the mod from here.

You can see that a function called `executePatching()` is called.
This is where we can put our mod-code!

I have probably left some code in there which you probably want to delete and write your own. In fact, we can do that right now!

So, I will create a new project, put an instance in the room, check what ID that instance has by doing 
`Create Event -> show_message(id)`

I'm also gonna give it a variable called `sus = 99`.

Great! Now we know this object's ID which won't change.
We can use this to manipulate the value of `sus`!

```cpp
bool executePatching() // This is where your patch code goes.
{
	gml::show_message("Hello from C!"); // Shows a message

    // Sets a variable of the instance with ID 100000
	GMLVar val = gml::variable_instance_get(100000, "sus"); 

    // This will read the value set above
	gml::show_debug_message("Sus value is " + to_string(val.getReal()));
	return true;
}
```

The above code will write the new value to that object's variable and also read it again, then print that to the debug log.

## Step 4: Injection!!!!

Now the """hacky part"""

You now want to compile the DLL (Ctrl+B).
Cool, you now have the mod sitting around in your DLL file, but it doesn't do anything on its own.

Time to inject that mod into the game!

Download my (not janky at all) x86 Injector (OFC you can use any injector, you probably have your own sitting around somewhere)
(https://github.com/sam-k0/x86-Injector/releases/tag/v.1.1.0.5)

Open the exe, select your DLL from the ...-menu, scroll through the list and look for your game's name. If it doesn't show up, click the ...-menu again and reselect your DLL file. This refreshes the window list.

If you found your game window, click to select it and then press `Inject!`. You will now see your DLL-Code being executed! Hooray!


I hope this little guide helped. I will probably make a video on that in the future, which is god knows when, because as of writing this, I have Covid and can't really speak that much.

Anyways, thx 4 reading and stay safe!