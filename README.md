# hivemind
General Game playing environment for autonomous agents. Runs an arbitrary amount of processes against each other as specified by a `Referee` type object.

Components Layout:

```
                    ################## ----> ################### --||-> ==============
                    #                #       # AGENT INTERFACE #   ||   | AI PROCESS |
                    #                # <---- ################### <-||-- ==============
                    #                #
  ########### ----> #                # ----> ################### --||-> ==============
  # REFEREE #       # GAME INTERFACE #       # AGENT INTERFACE #   ||   | AI PROCESS |
  ########### <---- #                # <---- ################### <-||-- ==============
                    #                #
                    #                # ----> ################### --||-> ==============
                    #                #       # AGENT INTERFACE #   ||   | AI PROCESS |
                    ################## <---- ################### <-||-- ==============
 ^--------------------------|-----------------------------------^      ^-------|------^
                 Main ./gameManager process                          Standalone processes
```

## src
Written to be build on a Linux environment using `g++ -std=c++0x` compilation flag.

Enabled `-Wall -Werror` flags to catch all warnings and errors.

Just run `make` from within `src\` to compile project.

Output `gameManager` binary executable that reads from `config.json` and spawns children processes to play against each other.

## Configurations
Place configurations under `config.json` for binary to parse.

We must have at least an "agents" key with agent process specifications. See example configuration file below:
```
{
    "agents":
    {
        "FastBot":
        {
            "type": "python3",
            "cmd": "simple_uttt.py 10"
        },
        "GoodBot":
        {
            "type": "python3",
            "cmd": "mcts_ucb.py 48"
        }
    },
    "config":
    {
        "init": "1000",
        "round": "50",
        "games": "5"
    }
}
```
Each "agent" requires a unique name that identifies it (key within "agents") and have at least the two "type" and "cmd" fields.

- "type": Specifies what language it is written as, if it is a binary executable, an empty string suffices.
- "cmd": Specifies what to run as the command line

For instance:
```
    "JavaBot":
    {
        "type": "java",
        "cmd": "SimpleIO"
    },
    "CppBot":
        "type": "",
        "cmd": "./timed_child"
    }
```

## Referee Specifications
Currently, Referee code is in-built into `./gameManager` executable during the compilation step.

> TODO: Future work will be to separate the Referee object entirely as a separate process from our manager.

We **MUST** define an `#define NUM_AGENTS` within our `MyReferee.h` file to notify the gameManager how many agents we expect within our configurations file (so we can raise an error if we have insufficient agents to play the game with).

Abstract Referee class to implement:

```cpp
class Referee{
    public:
        Referee() {}
        virtual ~Referee() {}
        //Interface Methods
        virtual void run() = 0;
    protected:
        shared_ptr<GameInterface> hive;
};
```

Custom Referee Format:
```cpp
class MyReferee: public Referee{
    public:
        MyReferee(shared_ptr<GameInterface> game){
            //Manager will pass a GameInterface Object
            //Take it and update 'hive'
            hive = game;
        }

        ...

        virtual void run(); //Must implement runs till game is done
        
        ...
};
```

> No other changes are necessary if we leave custom Referee class name as `MyReferee.h` `MyReferee.cpp`

Modifying `main` to run our new Referee (if necessary):
```cpp
[7]   #include "MyReferee.h"                          //--> Change file name accordingly

...

[41]  Referee* referee = new MyReferee(game_handle);  //--> Change object name accordingly
```

Compile with `make` in src folder and run executable `./gameManager`.

### File I/O Overheads
In-built 1ms file I/O overhead leeway seems sufficient. (Even slowest Java uses ~300us < 1ms)

Ran some tests to check process startup times and I/O overheads for different languages.

Currently we have in order of speed: c++ > python > java

## Ultimate Tic Tac Toe
We have MyReferee default to a UTTT judge. Included python mcts program to play UTTT. Usage as follows: `python3 mcts_ucb.py [ms]` where `ms` defaults to a timeout of 48ms if not given an argument.

## Valgrind
Execute `./chkmem.sh` to run a valgrind memory leak trace on the `./gameManager` executable. Outputs to log file `valgrind_memcheck.txt`. Currently program has no memory leaks.

## test
Test environment for playing around with piping and process control stuff. Experimental code prototypes.
