# Splash Text

Splash Text is a unified system for splash text, freely usable by any program.  The invocation of the system can be fine-tuned to the needs of the invoker.  It comes with some presupplied splash files, but you can also supply your own.  Splash Text also comes with a simple command-line wrapper program.

## Installing

The main function of the library, `splashtext()`, only requires compatibility with the C11 language and standard library, plus the extension allowing dollar signs in identifiers.  Building with the makefile requires compatibility with POSIX, and installation with the makefile additionally requires compatibility with the Filesystem Hierarchy Standard.  FHS compliance is also required for the the `splashtext` command-line utility.  I will be happy to port Splash Text to any _open_ and _actively maintained_ systems.  __Ports to obsolete or mallicious systems will not be considered.__

## Usage

See the manpage `splashtext(6)` for details.

## Contributing

Please see [CONTRIBUTING.md](Documentation/CONTRIBUTING.md) for details.

## License

This work is licensed under the BFCLLE.  See [LICENSE](LICENSE) for details.
