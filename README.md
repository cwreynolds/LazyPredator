# LazyPredator
## Genetic programming, negative selection, genetic drift.

**LazyPredator** is a framework for genetic programming — a type of genetic algorithm — with an emphasis on negative selection, genetic drift, and relative tournament-based fitness. It is intended for population-based evolutionary optimization. Unlike gradient descent methods, evolutionary computation does not use gradient so does not require differentiablity.

It is written in C++ and structured as a “header only” library. Simply copy (or `git clone`) its source files and include `LazyPredator.h` in your code.

LazyPredator supports genetic programming (GP: [[Koza 1992]](https://mitpress.mit.edu/9780262527910/genetic-programming/), [[Cramer 1985]](http://gpbib.cs.ucl.ac.uk/gp-html/icga85_cramer.html))
more specifically, the varient called strongly typed genetic programming (STGP: [[Montana 1995]](http://gpbib.cs.ucl.ac.uk/gp-html/montana_stgpEC.html)).

**LazyPredator** was initially used to optimize programs to perform procedural texture synthesis based on the [TexSyn](https://cwreynolds.github.io/TexSyn/) library. That combination of LazyPredator and TexSyn was a large component of the research project described in the paper [Coevolution of Camouflage](https://arxiv.org/abs/2304.11793) published at the Artificial Life Conference 2023, in Sapporo, Japan, then in a poster at SIGGRAPH 2023 in Los Angeles, USA.

There is a bit of a [LazyPredator development notebook](https://cwreynolds.github.io/LazyPredator/). Much deeper discussion of its initial use for the camouflage project is in [TexSyn's devo blog](https://cwreynolds.github.io/TexSyn/).

Please contact the [author](https://github.com/cwreynolds) if you have questions about using LazyPredator.
