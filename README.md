# tmx2snes
This is a simple quick-and-dirty commandline tool for converting maps created with [Tiled](https://github.com/bjorn/tiled) level editor for use in SNES ROMs.

It will create a background map that can be loaded into VRAM without further modification(e.g., with DMA). The color palette is set to palette zero of CG-RAM, as is the priority bit; this might need to be changed "by hand".

This is a tool I wrote quickly out of laziness for loading background maps in homebrew SNES ROM. It is unoptimized and there are no sanity checks. *Please be careful and use at own discretion.*

I hope to integrate it into my other SNES graphic tool [Kitsune](https://github.com/georgjz/kitsune) as soon as find the time for it. It does the other half of the work: converting image files into tile/sprite and palette data for use in a SNES ROM.

I admit that I shamelessly stole the code for the parser somewhere long time ago. I will attribute the original creator if I find out where I got it.

## Building
Use `make` in combination with `clang++`. The makefile(at least on my system) only works with `clang++` at the moment. You will also need the [cimg library](http://cimg.eu).

It's inconvenient but again, this was written out of laziness.

## Usage
`tmx2snes -f input.tmx -o output.map`
Both flags `-f` and `-o` are needed, else tmx2snes will not execute.
