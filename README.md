# hivemind
General Game playing environment for autonomous agents

## src
Written to be build on a Linux environment using `g++ -std=c++0x` compilation flag.

Enabled `-Wall -Werror` flags to catch all warnings and errors.

Just run `make` from within `src\` to compile project.

### File I/O Overheads
Ran some tests to check process startup times and I/O overheads for different languages.

Currently we have in order of speed: c++ > python > java

Look at `info_[lang]_IO.txt` files to check the I/O output diagnostics.

## Configurations
Place configurations under config.json for binary to parse.

> We have a json parser but currently not yet determined on the config interface. Also, WARNING: not tested...

## test
Test environment for me to play around with piping and process control stuff. Experimental code region (prototypes).
