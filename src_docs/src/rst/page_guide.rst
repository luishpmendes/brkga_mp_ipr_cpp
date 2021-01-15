.. index:: pair: page; Guide / Tutorial
.. _doxid-guide:

Guide / Tutorial
===============================================================================

.. _doxid-guide_1guide_installation:
Installation
-------------------------------------------------------------------------------

BRKGA-MP-IPR is a header-only framework, which means that you only need to
download the header files and tell your compiler to include the path to where
the files were downloaded.

Quick example (unix): assume we are in an empty folder. So, we copy/clone
BRKGA-IPR-MP first:

.. code-block::

    $ git clone https://github.com/ceandrade/brkga_mp_ipr_cpp
    Cloning into 'brkga_mp_ipr'...
    remote: Enumerating objects: 118, done.
    remote: Counting objects: 100% (118/118), done.
    remote: Compressing objects: 100% (112/112), done.
    remote: Total 118 (delta 24), reused 0 (delta 0)
    Receiving objects: 100% (118/118), 3.50 MiB | 3.66 MiB/s, done.
    Resolving deltas: 100% (24/24), done.

Let's write a ``test.cpp`` with the following code:

.. ref-code-block:: cpp

    #include "brkga_mp_ipr.hpp"
    #include <iostream>

    int main() {
        std::cout << "Testing sense: " << BRKGA::Sense::MINIMIZE;
        return 0;
    }

Then, let's compile and see it works:

.. code-block::

    $ g++ --version
    g++ (MacPorts gcc8 8.2.0_3) 8.2.0

    $ g++ -std=c++14 -Ibrkga_mp_ipr/brkga_mp_ipr test.cpp -o test

    $ ./test
    Testing sense: MINIMIZE

Note the Git clones the whole repository that contains the library code,
documents, and examples. All the examples were built using `GNU/Make
<https://www.gnu.org/software/make/>`_ and `GCC toolchain
<https://gcc.gnu.org>`_. However, the code is standard C++, and we can
quickly adapt it to other toolchains such as `Clang
<https://clang.llvm.org>`_, `Microsoft
<https://visualstudio.microsoft.com>`_, or `Intel
<https://software.intel.com/en-us/c-compilers>`_ toolchains. To build this
documentation, we also need `Doxygen <http://www.doxygen.nl>`_.


.. _doxid-guide_1guide_tldr:
TL;DR
-------------------------------------------------------------------------------

The best way to keep it short is to look in the on examples on `the git repo.
<https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples>`_
Let's take a look into
`main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_minimal.cpp>`_,
which solves the
`Traveling Salesman Problem (TSP) <https://en.wikipedia.org/wiki/Travelling_salesman_problem>`_.
This is a trimmed copy:

.. ref-code-block:: cpp

    #include "tsp/tsp_instance.hpp"
    #include "decoders/tsp_decoder.hpp"
    #include "brkga_mp_ipr.hpp"
    #include <iostream>
    #include <stdexcept>
    #include <string>
    using namespace std;

    int main(int argc, char* argv[]) {
        if(argc < 4) {
            cerr << "Usage: "<< argv[0]
                 << " <seed> <config-file> <num-generations>"
                    " <tsp-instance-file>" << endl;
            return 1;
        }

        const unsigned seed = stoi(argv[1]);
        const string config_file = argv[2];
        const unsigned num_generations = stoi(argv[3]);
        const string instance_file = argv[4];

        auto instance = TSP_Instance(instance_file);

        auto params = BRKGA::readConfiguration(config_file);
        auto& brkga_params = params.first;

        TSP_Decoder decoder(instance);

        BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
                decoder, BRKGA::Sense::MINIMIZE, seed,
                instance.num_nodes, brkga_params);

        algorithm.initialize();

        algorithm.evolve(num_generations);

        auto best_cost = algorithm.getBestFitness();
        cout << "Best cost: " << best_cost;

        return 0;
    }

You can identify the following basic steps:

#. Create a data structure to hold your input data. This object should be
   passed to the decoder object/functor (example
   `tsp/tsp_instance.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/tsp/tsp_instance.hpp>`_);

#. Implement a decoder object/functor. This function translates a chromosome
   (array of numbers in the interval [0,1]) to a solution for your problem. The
   decoder must return the solution value or cost to be used as fitness by
   BRKGA (example `decoders/tsp_decoder.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/decoders/tsp_decoder.hpp>`_);

#. Load the instance and other relevant data;

#. Read the algorithm parameters using
   ``:ref:`BRKGA::readConfiguration() <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>```;
   or create a ``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` object by hand;

#. Create an ``:ref:`BRKGA::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` algorithm object;

#. Call ``:ref:`BRKGA::BRKGA_MP_IPR::initialize() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a7857351d4ce17199bd2fc157589a8592>``` to init the BRKGA state;

#. Call ``:ref:`BRKGA::BRKGA_MP_IPR::evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>``` to optimize;

#. Call ``:ref:`BRKGA::BRKGA_MP_IPR::getBestFitness() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1af0084ce8397e82db99391bf4dad85219>``` and/or ``:ref:`BRKGA::BRKGA_MP_IPR::getBestChromosome() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>``` to retrieve the best solution.

`main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_minimal.cpp>`_
provides a very minimal example to understand the necessary steps to use the
BRKGA-MP-IPR framework. However,
`main_complete.cpp` <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_complete.cpp>`_
provides a full-featured code, handy for scientific use, such as
experimentation and paper writing. This code allows fine-grained control of
the optimization, shows several features of BRKGA-MP-IPR such as the resets,
chromosome injection, and others. It also logs
all optimization steps, _creating outputs easy to be parsed._ **You should use
this code for serious business and experimentation.**

These are the basic steps, but I do recommend the reading of this guide.


.. _doxid-guide_1guide_getting_started:
Getting started
-------------------------------------------------------------------------------

BRKGA-MP-IPR is pretty simple, and you must provide one required *decoder*
object to translate chromosomes to solutions. In general, such decoder uses
the problem information to map a vector of real numbers in the interval [0,1]
to a (valid) solution. In some cases, even though a valid solution cannot be
found, library users apply penalization factors and push the BRKGA to find
valid solutions.

Before you go further, please take a look at the ``examples`` folder in `the
git repo <https://github.com/ceandrade/brkga_mp_ipr_cpp>`_. There, you can find
two folders with codes to solve `combinatorial auction problems
<http://dx.doi.org/10.1162/EVCO_a_00138>`_ and traveling salesman problems.
In this guide, we solve the classical `Traveling Salesman Problem
<https://en.wikipedia.org/wiki/Travelling_salesman_problem>`_. Given aset of
cities and the distances between them (full weighted undirect graph), one
must find a minimum-cost tour among all cities, such that each city is
visited only once (i.e., find a Hamiltonian cycle of minimum cost). The
folder has the following structure:

* ``src`` subdir: contains all the code;

* ``instances`` subdir: folder containing some TSP instances for testing;

The ``src`` subdir contains all the code to solve TSP. This is its structure:

* ``tsp`` subdir: contains code to load and build data for TSP;

* ``decoders`` subdir: contains the TSP decoder;

* ``heuristics`` subdir: contains a simple heuristic that computes a greedy
  tour;

* ``main_minimal.cpp`` file: minimal code useful to understand and test the
  framework. **You should start here!** Please take a look on this file before
  continue this tutorial;

* ``main_complete.cpp`` file: full-featured code, handy for scientific use,
  such as experimentation and paper writing. This code allows fine-grained
  control of the optimization, shows several features of BRKGA-MP-IPR such as
  the path-relinking calls, resets, chromosome injection, and others. It also
  logs all optimization steps, _creating outputs easy to be parsed._
  **You should use this code for serious business and experimentation;**

* ``config.conf`` file: example of parameter settings;

* ``Makefile`` file: the makefile used to build the executables;

* ``third_part`` subdir: contains the `docopt <https://github.com/docopt/docopt.cpp>`_
  dependence for ``main_complete.cpp``. This is not strictly necessary for
  BRKGA-MP-IPR, but it adds a nice command line interface. If you have problems
  with that, we can change ``main_complete.cpp`` using traditional argument
  handling, or another library you prefer.

The first step is to build the code. Here, we are using `GNU/Make
<https://www.gnu.org/software/make>`_ and `GCC toolchain
<https://gcc.gnu.org>`_. You may change for the toolchain of your choice.
You may need to edit this file according to your compiler version and
settings. The first thing to note in the makefile is the parameter ``OPT``
that, when set ``OPT=opt``, it turns on aggressive optimization flags (for
G++). If the flag is not set, aggressive debug options are set. For serious
implementations, and debugging, we do recommend to use such setup. However,
if your compiler does not like such settings, please, go ahead and change
accordingly. By typing just ``make``, you build both minimal and complete
versions. We also can just type ``make main_minimal`` or ``make
main_complete`` to build one or other version independently. Typing ``make
clean`` will clean up the compilation.

When you call the executables ``main_minimal`` or ``main_complete`` without
arguments, they show the usage. For example, assuming you are using a
terminal:

.. ref-code-block::

    $./main_minimal
    Usage: ./main_minimal <seed> <config-file> <num-generations> <tsp-instance-file>

    $ ./main_complete
    Arguments did not match expected patterns
    Usage:
      main_complete -c <config_file> -s <seed> -r <stop_rule> -a <stop_arg> -t <max_time> -i <instance_file> [-n <num_threads>] [--no_evolution]
      main_complete (-h | --help)
    ...

So, this is a possible output whe calling ``main_minimal`` :

.. ref-code-block::

    $ ./main_minimal 27000001 config.conf 100 ../instances/brazil58.dat
    Reading data...
    Reading parameters...
    Building BRKGA data and initializing...
    Evolving 100 generations...
    Best cost: 27709

For ``main_complete``, the output is more verbose, since we want to capture
as much information as possible to do some statistical analysis. The output
should be something close to this:

.. ref-code-block::

    $ ./main_complete -c config.conf -s 2700001 -r Generations -a 100 -t 60 -i ../instances/brazil58.dat
    ------------------------------------------------------
    > Experiment started at Tue Mar 12 10:03:31 2019
    > Instance: ../instances/brazil58.dat
    > Configuration: config.conf
    > Algorithm Parameters
    >   - population_size: 2000
    >   - elite_percentage: 0.3
    >   - mutants_percentage: 0
    >   - num_elite_parents: 2
    >   - total_parents: 3
    >   - bias_type: LOGINVERSE
    >   - num_independent_populations: 3
    >   - pr_number_pairs: 0
    >   - pr_minimum_distance: 0.15
    >   - pr_type: PERMUTATION
    >   - pr_selection: BESTSOLUTION
    >   - alpha_block_size: 1
    >   - pr_percentage: 1
    >   - exchange_interval: 200
    >   - num_exchange_individuals: 2
    >   - reset_interval: 600
    > Seed: 2700001
    > Stop rule: Generations
    > Stop argument: 100
    > Maximum time (s): 60
    > Number of parallel threads for decoding: 1
    ------------------------------------------------------

    [Tue Mar 12 10:03:31 2019] Reading TSP data...
    Number of nodes: 58

    [Tue Mar 12 10:03:31 2019] Generating initial tour...
    Initial cost: 30774

    [Tue Mar 12 10:03:31 2019] Building BRKGA...
    New population size: 580

    [Tue Mar 12 10:03:31 2019] Injecting initial solution...

    [Tue Mar 12 10:03:31 2019] Initializing BRKGA...

    [Tue Mar 12 10:03:31 2019] Evolving...
    * Iteration | Cost | CurrentTime
    * 0 | 30774 | 0.02
    * 24 | 30710 | 0.36
    * 25 | 30536 | 0.37
    * 26 | 30063 | 0.39
    * 29 | 29956 | 0.43

    [Tue Mar 12 10:03:32 2019] End of optimization

    Total number of iterations: 101
    Last update iteration: 29
    Total optimization time: 1.35
    Last update time: 0.43
    Large number of iterations between improvements: 24
    Total path relink time: 0.00
    Total path relink calls: 0
    Number of homogenities: 0
    Improvements in the elite set: 0
    Best individual improvements: 0

    % Best tour cost: 29956
    % Best tour: 0 29 12 39 24 8 31 19 52 49 3 17 43 23 57 4 26 42 11 56 22 53 54 1 40 34 9 51 50 46 48 2 47 38 28 35 16 25 5 18 27 13 36 14 33 45 55 44 32 20 10 15 21 7 37 41 30 6

    Instance,Seed,NumNodes,TotalIterations,TotalTime,TotalPRTime,PRCalls,NumHomogenities,NumPRImprovElite,NumPrImprovBest,LargeOffset,LastUpdateIteration,LastUpdateTime,Cost
    brazil58,2700001,58,101,1.35,0.00,0,0,0,0,24,29,0.43,29956

I hope by now you got your system set up and running. Let's see the essential
details on how to use the BRKGA-MP-IPR.


.. _doxid-guide_1guide_decoder:
First things first: the decoder function
-------------------------------------------------------------------------------

The core of the BRKGA algorithm is the definition of a decoder
function/object. The decoder maps the chromosomes (vectors of real numbers in
the interval [0, 1]) to solutions of the problem. In some sense, a decoder is
similar to a `kernel function from Support Vector Machines
<https://en.wikipedia.org/wiki/Kernel_method>`_ : both functions are used to
project solutions/distances in different spaces.

Here, we have a small difference between the C++/Python and the Julia
implementations. In the Julia version, you must define a data container
inherit from `AbstractInstance
<https://ceandrade.github.io/brkga_mp_ipr_julia/guide/index.html#First-things-first:-basic-data-structures-and-decoder-function-1>`_,
and a decoder function. The reason you must do that is because structs in
Julia have no methods (but constructors), and the decoder function must take
both chromosome and input data in the call. In C++/Python, we can encapsulate the
input data into the decoder object, resulting in a much more clear API.

The basic form of a decoder should be:

.. ref-code-block:: cpp

    class Decoder {
        double decode(:ref:`BRKGA::Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome, bool rewrite);
    };

The decoder **must** contain a **decode()** method that receives a
``:ref:`BRKGA::Chromosome
<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>``` reference
and an ``boolean``, and returns a float point number. But before going
further, let's talk about the chromosome.


The chromosome or vector of doubles
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Note that all long the BRKGA discussion, the chromosome is represented as a
vector of real numbers in the interval [0,1]. Indeed, we could use
straightforward ``std::vector<double>``. However, sometimes is interesting to
keep more information inside the chromosome for further analysis, such as,
other solution metrics that not the main fitness value. For example, in a
scheduling problem, we may choose to keep both makespan and total completion
time metrics. Therefore, we chose to make the chromosome a "generic" data
structure in our design.

File `chomosome.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/brkga_mp_ipr/chromosome.hpp>`_ shows the
basic represetation of a chromosome:

.. ref-code-block:: cpp

    typedef std::vector<double> :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`;

If this enough for you, you go already and use such a definition. However,
instead to redefine in your own code, **we do recommend to import and use the
definition from** `chomosome.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/brkga_mp_ipr/chromosome.hpp>`_,
since it is the same definition the main BRKGA-MP-IPR algorithm uses.

However, if you need more information to be tracked during the optimization,
you can redefine the chromosome. First, your definition must complain with
the ``std::vector<>`` interface. The easiest way to do that is to inherit
from the ``std::vector<>``. For instance, assume we want to keep track of the
makespan and the total completion time for a scheduling problem. We can do
the following:

.. ref-code-block:: cpp

    class :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`: public std::vector<double> {
        public:
            :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`() :
                :ref:`std <doxid-namespacestd>`::vector<double>(), makespan(0.0), total_completion_time(0.0)
                {}

            :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`(unsigned _size, double _value = 0.0)
                :ref:`std <doxid-namespacestd>`::vector<double>(_size, value),
                makespan(0.0), total_completion_time(0.0)
                {}

            :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`(const :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chr) = default;

        public:
            double makespan;
            double total_completion_time;
    };

In general, most people do not recommend to inherit publicly from
``std::vector<>`` because it has no virtual destructor. However, we may do
that as long as:

a) We remember that every operation provided by ``std::vector<>`` must be a
   semantically valid operation on an object of the derived class;

b) We avoid creating derived class objects with dynamic storage duration;

c) We **DO AVOID** polymorphism:

.. ref-code-block:: cpp

    std::vector<double>* pt = new :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`();     // Bad idea.
    delete pt;      // Delete does not call the Chromosome destructor.


Back to the decoder
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Again, the decoder is the heart of a BRKGA. An easy way to keep the API clean
is to define a decoder that has a reference for the input data. This is a TSP
decoder defined on file `decoders/tsp_decoder.hpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/decoders/tsp_decoder.hpp>`_:

.. ref-code-block:: cpp

    #include "tsp/tsp_instance.hpp"
    #include "chromosome.hpp"
    class TSP_Decoder {
    public:
        TSP_Decoder(const TSP_Instance& instance);
        double decode(:ref:`BRKGA::Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome, bool rewrite);

    public:
        const TSP_Instance& instance;
    };

Note that ``TSP_Decoder`` get a const reference to ``TSP_Instance``, that holds
the input data. Therefore, ``TSP_Decoder`` has direct access to the data for
optimization. This approach also benefits cache efficiency, mainly when
multiple threads are used for decoding, i.e., several threads can use the same
read-only data already in the cache, which speeds up the optimization.

The decode method also has a ``rewrite`` argument that indicates if the decoder
should rewrite the chromosome, in case of local search / local improvements be
performed during the decoder process. This flag is critical if you intend to
use the Implicit Path Relink (details on
``:ref:`BRKGA::BRKGA_MP_IPR::pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>```).
Even though you do not rewrite the chromosome in your decoder, you must provide
such signature for API compatibility.

The decoder must return a ``double`` that is used as the **fitness** to rank
the chromosomes. In general, fitness is the cost/value of the solution, but you
may want to use it to penalize solutions that violate the problem constraints,
for example.

.. note::
  When using multiple threads, **you must guarantee that the decoder is
  thread-safe.** You may want to create all read-write data structures on each
  call or create a separate storage space for each thread. Section
  :ref:`Multi-threading <doxid-guide_1guide_tips_multihreading>` brings some
  tips.

In our TSP example, we have a very simple decoder that generates a permutation
of nodes, and compute the cost of the cycle from that permutation
(note that we don't use the flag ``rewrite`` in this example):

.. ref-code-block:: cpp

    double TSP_Decoder::decode(:ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome,  bool /* not-used */) {
        vector<pair<double, unsigned>> permutation(instance.num_nodes);
        for(unsigned i = 0; i < instance.num_nodes; ++i)
            permutation[i] = make_pair(chromosome[i], i);

        sort(permutation.begin(), permutation.end());

        double cost = instance.distance(permutation.front().second,
                                        permutation.back().second);

        for(unsigned i = 0; i < instance.num_nodes - 1; ++i)
            cost += instance.distance(permutation[i].second,
                                      permutation[i + 1].second);

        return cost;
    }

With the instance data and the decoder ready, we can build the BRKGA data
structures and perform the optimization.


.. _doxid-guide_1guide_brkga_object:
Building BRKGA-MP-IPR algorithm object
-------------------------------------------------------------------------------

``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` is the main object that
implements all BRKGA-MP-IPR algorithms such as evolution, path relink, and
other auxiliary procedures. Note that ``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` is a template
parametrized by the decoder type. This strategy avoids runtime polymorphism,
drastically improving the performance of the code.

The first step is to call the algorithm constructor that has the following
signature:

.. ref-code-block:: cpp

    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        const :ref:`Sense <doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense,
        const unsigned seed,
        const unsigned chromosome_size,
        const BrkgaParams& params,
        const unsigned max_threads = 1,
        const bool evolutionary_mechanism_on = true);

The first argument is the decoder object that must implement the ``decode()``
method as discussed before. You also must indicate whether you are minimizing
or maximizing through parameter ``:ref:`BRKGA::Sense
<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>```.

A good seed also must be provided for the (pseudo) random number generator
(according to `this paper <http://doi.acm.org/10.1145/1276927.1276928>`_).
BRKGA-MP-IPR uses the Mersenne Twister engine
`[1] <http://dx.doi.org/10.1145/272991.272995>`_
`[2] <https://en.wikipedia.org/wiki/Mersenne_Twister>`_
from the standard C++ library
`[3] <http://www.cplusplus.com/reference/random/mt19937>`_
`[4] <https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine>`_.

The ``chromosome_size`` also must be given. It indicates the length of each
chromosome in the population. In general, this size depends on the instance and
how the decoder works. The constructor also takes a ``:ref:`BRKGA::BrkgaParams
<doxid-class_b_r_k_g_a_1_1_brkga_params>``` object that holds several
parameters. We will take about that later.

``max_threads`` defines how many threads the algorithm should use for decoding
and some other operations. As said before, **you must guarantee that the
decoder is thread-safe** when using two or more threads. See
:ref:`Multi-threading <doxid-guide_1guide_tips_multihreading>` for more
information.

Another common argument is ``evolutionary_mechanism_on`` which is enabled by
default. When disabled, no evolution is performed. The algorithm only decodes
the chromosomes and ranks them. On each generation, all population is replaced
excluding the best chromosome. This flag helps on implementations of simple
multi-start algorithms.

All BRKGA and Path Relink hyper-parameters
are stored in a ``:ref:`BRKGA::BrkgaParams
<doxid-class_b_r_k_g_a_1_1_brkga_params>``` object. Such objects can be read
and write from plain text files or can be created by hand by the user. There is
also a companion ``:ref:`BRKGA::ExternalControlParams
<doxid-class_b_r_k_g_a_1_1_external_control_params>``` object that stores extra
control parameters that can be used outside the BRKGA-MP-IPR to control several
aspects of the optimization. For instance, interval to apply path relink, reset
the population, perform population migration, among others. This is how a
configuration file looks like (see `config.conf
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/config.conf>`_
for a commented version):

.. ref-code-block::

    population_size 2000
    elite_percentage 0.30
    mutants_percentage 0.15
    num_elite_parents 2
    total_parents 3
    bias_type :ref:`LOGINVERSE <doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adca762bd1443afdcf03af352da1c9184>`
    num_independent_populations 3
    pr_number_pairs 0
    pr_minimum_distance 0.15
    pr_type :ref:`PERMUTATION <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a48deaef68056f516e0091a15c1db3daa>`
    pr_selection :ref:`BESTSOLUTION <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`
    alpha_block_size 1.0
    pr_percentage 1.0
    exchange_interval 200
    num_exchange_individuals 2
    reset_interval 600

To read this file, you can use the function ``:ref:`BRKGA::readConfiguration()
<doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>```, which
returns a ``std::pair<BrkgaParams, ExternalControlParams>``. When reading such
file, the function ignores all blank lines, and lines starting with ``#``. As
commented before, ``:ref:`BRKGA::BrkgaParams
<doxid-class_b_r_k_g_a_1_1_brkga_params>``` contains all hyper-parameters
regarding :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR methods and
``:ref:`BRKGA::ExternalControlParams
<doxid-class_b_r_k_g_a_1_1_external_control_params>``` contains extra control
parameters, and although their presence is required on the config file, they
are not mandatory to the BRKGA-MP-IPR itself.

Let's take a look in the example from `main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/main_minimal.cpp>`_:

.. ref-code-block:: cpp

    const unsigned seed = stoi(argv[1]);
    const string config_file = argv[2];
    const unsigned num_generations = stoi(argv[3]);
    const string instance_file = argv[4];

    auto instance = TSP_Instance(instance_file);

    // C++14 syntax.
    auto params = :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>`(config_file);
    auto& brkga_params = params.first;

    // C++17 syntax. Isn't cool?
    // auto [brkga_params, control_params] =
    //     BRKGA::readConfiguration(config_file);

    TSP_Decoder decoder(instance);

    :ref:`BRKGA::BRKGA_MP_IPR<TSP_Decoder> <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>` algorithm(
            decoder, :ref:`BRKGA::Sense::MINIMIZE <doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9ba704bfa6c1ed5e479c8cfb5bdfc8cccda>`, seed,
            instance.num_nodes, brkga_params);

This code gets some arguments from the command line and loads a TSP instance.
After that, it reads the BRKGA parameters from the configuration file. Since in
this example, we only care about the BRKGA parameters, we ignore the control
parameters (tip: note how C++ 17 notation is much cleaner and more elegant than
the traditional version). We then build the decoder object, and the BRKGA
algorithm. Since we are looking for cycles of minimum cost, we ask for the
algorithm ``MINIMIZE``. The starting seed is also given. Since ``TSP_Decode``
considers each chromosome key as a node/city, the length of the chromosome must
be the number of nodes, i.e., ``instance.num_nodes``. Finally, we also pass the
BRKGA parameters.

Now, we have a ``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` which will be used to
call all other functions during the optimization. Note that we can build
several ``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` objects using different
parameters, decoders, or instance data. These structures can be evolved in
parallel and mixed-and-matched at your will. Each one holds a self-contained
BRKGA state including populations, fitness information, and a state of the
random number generator.


.. _doxid-guide_1guide_algo_init:
Initialization and Warm-start solutions
-------------------------------------------------------------------------------

Before starting the optimization, we need to initialize the :ref:`BRKGA
<doxid-namespace_b_r_k_g_a>` algorithm state using
``:ref:`BRKGA::BRKGA_MP_IPR::initialize()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a7857351d4ce17199bd2fc157589a8592>```
method. This procedure initializes the populations and others data structures
of the BRKGA. If an initial population (warm
start) is supplied, the initialization method completes the remaining
individuals, if they do not exist. This method also performs the initial
decoding of the chromosomes. Therefore, depending on the decoder
implementation, this can take a while, and you may want to time such procedure.
Assuming that ``algorithm`` is our ``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>```
object, the syntax is pretty straightforward:

.. ref-code-block:: cpp

    algorithm.initialize();

.. warning::
  ``initialize()`` must be called before any optimization methods.

.. warning::
  As it is in method ``evolve()``, the decoding is done in parallel using
  threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If
  such property cannot be held, we suggest using a single thread for
  optimization.

Warm-start solutions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

One good strategy is to bootstrap the main optimization algorithm with good
solutions from fast heuristics
[`1 <http://dx.doi.org/10.1002/net.21685>`_,
`2 <http://dx.doi.org/10.1016/j.ejor.2017.10.045>`_,
`3 <http://dx.doi.org/10.1016/j.ejor.2017.10.045>`_]
or even from relaxations of integer linear programming models
`[4] <http://dx.doi.org/10.1162/EVCO_a_00138>`_.

To do it, you must set these initial solutions before call ``initialize()``.
Since BRKGA-MP-IPR does not know the problem structure, you must *encode* the
warm-start solution as chromosomes (vectors in the interval [0, 1]). In other
words, you must do the inverse process that your decoder does. For instance,
this is a piece of code from `main_complete.cpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/main_complete.cpp>`_
showing this process:

.. ref-code-block:: cpp

    auto initial_solution = greedy_tour(instance);
    ...

    std::mt19937 rng(seed);
    vector<double> keys(instance.num_nodes);
    for(auto& key : keys)
        key = generate_canonical<double,
                                 numeric_limits<double>::digits>(rng);

    sort(keys.begin(), keys.end());

    :ref:`BRKGA::Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>` initial_chromosome(instance.num_nodes);
    auto& initial_tour = initial_solution.second;
    for(size_t i = 0; i < keys.size(); i++)
        initial_chromosome[initial_tour[i]] = keys[i];

    algorithm.setInitialPopulation(
        vector<BRKGA::Chromosome>(1, initial_chromosome));

Here, we create one incumbent solution using the greedy heuristic
``greedy_tour()`` `found here <https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/v1.0/examples/tsp/src/heuristics>`_.
It gives us
``initial_solution`` which is a ``std::pair<double, std::vector<unsigned>>``
containing the cost of the tour and the tour itself which is a sequence of
nodes to be visited. In the next lines, we encode ``initial_solution``. First,
we create a vector of sorted random ``keys``. For that, we create a new random
number generator ``rng``, the vector ``keys``, and fill up ``keys`` with random
numbers in the interval [0,1], using C++ standard library function
``generate_canonical<>()``. Once we have the keys, we sort them as
``TSP_Decoder::decode()`` does. We then create the ``initial_chromosome``, and
fill it up with ``keys`` according to the nodes' order in ``initial_solution``.
Finally, we use ``:ref:`BRKGA::BRKGA_MP_IPR::setInitialPopulation()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```
to assign the incumbent to the initial population. Note that we enclose the
initial solution inside a vector of chromosomes, since
``setInitialPopulation()`` may take more than one starting solution. See its
signature:

.. ref-code-block:: cpp

    void setInitialPopulation(const std::vector<Chromosome>& chromosomes);

Indeed, you can have as much warm-start solutions as you like, limited to the
size of the population. Just remember:

.. warning::
  ``setInitialPopulation()`` must be called **BEFORE** ``initialize()``.


.. _doxid-guide_1guide_opt:
Optimization time: evolving the population
-------------------------------------------------------------------------------

Once all data is set up, it is time to evolve the population and perform other
operations like path-relinking, shaking, migration, and others. The call is
pretty simple:

.. ref-code-block:: cpp

    algorithm.evolve(num_generations);

``:ref:`BRKGA::BRKGA_MP_IPR::evolve()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>```
evolves all populations for ``num_generations``. If ``num_genertions`` is
omitted, ``evolve()`` evolves only one generation.

For example, in `main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/main_minimal.cpp>`_, we just evolve the population for a given
number of generations directly and then extract the best solution cost.

.. ref-code-block:: cpp

    algorithm.evolve(num_generations);
    auto best_cost = algorithm.getBestFitness();

On
`main_complete.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/main_complete.cpp>`_
we have fine-grained control on the optimization.
There, we have a main loop that evolves the population one generation at a time
and performs several operations as to hold the best solution, to check whether
it is time for path relink, population reset, among others. The advantage of
that code is that we can track all optimization details, and I do recommend
similar style for experimentation.

.. warning::
  Again, the decoding of each chromosome is done in parallel if multi-thread is
  enabled. Therefore, **we must guarantee that the decoder is THREAD-SAFE.** If
  such property cannot be held, we suggest using a single thread.


.. _doxid-guide_1guide_access_solutions:
Accessing solutions/chromosomes
-------------------------------------------------------------------------------

BRKGA-MP-IPR C++ offers several mechanisms to access a variaty of data during
the optimization. Most common, we want to access the best chromosome after some
iterations. You can use the companion functions:

.. ref-code-block:: cpp

    double getBestFitness() const;

    const :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getBestChromosome() const;

``:ref:`BRKGA::BRKGA_MP_IPR::getBestFitness()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1af0084ce8397e82db99391bf4dad85219>```
returns the value/fitness of the best chromosome across all populations.

``:ref:`BRKGA::BRKGA_MP_IPR::getBestChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>```
returns a *reference* of the best chromosome across all populations. You may
want to extract an actual solution from such chromosome, i.e., to apply a
decoding function that returns the actual solution instead only its value.

You may also want to get a reference of specific chromosome and its fitness 
for a given population using ``:ref:`BRKGA::BRKGA_MP_IPR::getChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f>```.

.. ref-code-block:: cpp

    const :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getChromosome(unsigned population_index,
                                    unsigned position) const

    const :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getFitness(unsigned population_index,
                                 unsigned position) const

For example, you can get the 3rd best chromosome (and it fitness) 
from the 2nd population using

.. ref-code-block:: cpp

    third_best_chr = algorithm.getChromosome(1, 2);
    third_best_fitness = algorithm.getFitness(1, 2);

.. note::
  Just remember that C++ is zero-indexed. So, the first population index is 0
  (zero), the second population index is 1 (one), and so forth. The same happens
  for the chromosomes.

Now, suppose you get such chromosome or chromosomes and apply a quick local
search procedure on them. It may be useful to reinsert such new solutions in
the BRKGA population for the next
evolutionary cycles. You can do that using
``:ref:`BRKGA::BRKGA_MP_IPR::injectChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a70bbe32b8bb3f662b629698319dc0261>```.

.. ref-code-block:: cpp

    void injectChromosome(const :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome,
                          unsigned population_index,
                          unsigned position,
                          double fitness = std::numeric_limits<double>::infinity());

Note that the chromosome is put in a specific position of a given population.
If you do not provide the fitness, ``injectChromosome()`` will decode the
injected chromosome. For example, assuming the ``algorithm`` is your
BRKGA-MP-IPR object and ``brkga_params`` is your ``BrkgaParams`` object, the
following code injects the random chromosome ``keys`` into the population #1 in
the last position (``population_size``), i.e., it will replace the worst
solution by a random one:

.. ref-code-block:: cpp

    std::mt19937 rng(seed);
    vector<double> keys(instance.num_nodes);
    for(auto& key : keys)
        key = generate_canonical<double,
                                 numeric_limits<double>::digits>(rng);

    algorithm.injectChromosome(keys, 0, brkga_params.population_size);


.. _doxid-guide_1guide_ipr:
Implicit Path Relink
-------------------------------------------------------------------------------

The Implicit Path Relinking (IPR) is a nice addition to the standard BRKGA
framework, and it provides an excellent way to create hybrid heuristics and
push the optimization further. The good thing about IPR is that you do not
need to worry about the path relink implementation, which can be long and
tedious if done by hand or customized per problem.

BRKGA-MP-IPR provides a friendly interface to use IPR directly from the BRKGA
population, and you only must provide a few functions and arguments to have a
Path Relink algorithm ready to go. This is the main signature of
``:ref:`BRKGA::BRKGA_MP_IPR::pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>```
:

.. ref-code-block:: cpp

    :ref:`PathRelinking::PathRelinkingResult <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` pathRelink(
                    :ref:`PathRelinking::Type <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type,
                    :ref:`PathRelinking::Selection <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection,
                    std::shared_ptr<DistanceFunctionBase> dist,
                    unsigned number_pairs,
                    double minimum_distance,
                    std::size_t block_size = 1,
                    long max_time = 0,
                    double percentage = 1.0);

The first argument defines the type of implicit path relink to be performed
``:ref:`BRKGA::PathRelinking::Type
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>```.
The ``DIRECT`` path relink exchanges the keys of two chromosomes directly, and
it is usually more suitable to or threshold representations, i.e., where the
key values are used to some kind of discretization, such as "if x < 0.5, then
0, otherwise 1." The ``PERMUTATION`` path relink switches the order of a key
according to its position in the other chromosome. Usually, this kind of path
relink is more suitable to permutation representations, where the chromosome
induces an order or permutation. For example, chromosome ``[0.4, 0.7, 0.1]``
may induce the increasing order ``(3, 1, 2)``. More details about threshold and
permutation representations in `this paper <https://doi.org/10.1016/j.ejor.2019.11.037>`_.

``:ref:`BRKGA::PathRelinking::Selection
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>```
defines how the algorithm picks the chromosomes for relinking. ``BESTSOLUTION``
selects, in the order, the best solution of each population. ``RANDOMELITE``
chooses uniformly random solutions from the elite sets.

The next argument is a pointer to a functor object used to compute the distance
between two chromosomes, and determine if changes in a given (block) of alleles
change the solution. This object must inherit from
``:ref:`BRKGA::DistanceFunctionBase
<doxid-class_b_r_k_g_a_1_1_distance_function_base>```, which has the following
signature:

.. ref-code-block:: cpp

    class DistanceFunctionBase {
    public:
        DistanceFunctionBase() {}
        virtual ~DistanceFunctionBase() {}

        virtual double distance(const std::vector<double>& v1,
                                const std::vector<double>& v2) = 0;

        virtual bool affectSolution(const double key1, const double key2) = 0;

        virtual bool affectSolution(
                std::vector<double>::const_iterator v1_begin,
                std::vector<double>::const_iterator v2_begin,
                const std::size_t block_size) = 0;
    };

Note that ``:ref:`BRKGA::DistanceFunctionBase
<doxid-class_b_r_k_g_a_1_1_distance_function_base>``` is an abstract interface,
and children classes must implement all methods.

If the value returned by method ``distance()`` is greater than or equal to
``minimum_distance`` (on ``pathRelink()`` call), the algorithm will perform the
path relink between the two chromosomes. Otherwise, it will look for another
pair of chromosomes. The algorithm will try ``number_pairs`` chromosomes before
gives up. In the presence of multiple populations, the path relinking is
performed between elite chromosomes from different populations, in a circular
fashion. For example, suppose we have 3 populations. The framework performs 3
path relinkings: the first between individuals from populations 1 and 2, the
second between populations 2 and 3, and the third between populations 3 and 1.
In the case of just one population, both base and guiding individuals are
sampled from the elite set of that population.

Note that in traditional path relink algorithms, method ``distance()`` depends
on the problem structure. On IPR, you can use a generic distance function, or
provide one that incorporates more knowledge about the problem. BRKGA-MP-IPR
provides a class/functor to compute the (modified)
`Hamming distance <https://en.wikipedia.org/wiki/Hamming_distance>`_
for threshold representations (``:ref:`BRKGA::HammingDistance
<doxid-class_b_r_k_g_a_1_1_hamming_distance>```),
and a class/functor that computes the
`Kendall Tau distance <https://en.wikipedia.org/wiki/Kendall_tau_distance>`_
distance for permutation representations (``:ref:`BRKGA::KendallTauDistance
<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>```). Again, details about
threshold and permutation representations in `this paper
<https://doi.org/10.1016/j.ejor.2019.11.037>`_.

As a simple example, suppose you are using a threshold representation where
each chromosome key can represent one of 3 different values (a ternary
threshold representation). So, one possible way to compute the distance between
two chromosomes can be:

.. ref-code-block:: cpp

    class TernaryHammingDistance: public DistanceFunctionBase {
    protected:
        double value(const double key) const {
            return key < 0.33 ? 0.0 : (key < 0.66 ? 1.0 : 2.0);
        }

    public:
        explicit TernaryHammingDistance() {}
        virtual ~TernaryHammingDistance() {}

        virtual double distance(const std::vector<double>& vector1,
                                const std::vector<double>& vector2) {
            double dist = 0.0;
            for(std::size_t i = 0; i < vector1.size(); ++i)
                dist += std::fabs(value(vector1[i]) - value(vector2[i]));
            return dist;
        }

        virtual bool affectSolution(const double key1, const double key2) {
            return std::fabs(value(key1) - value(key2)) > 0.0;
        }

        virtual bool affectSolution(std::vector<double>::const_iterator v1_begin,
                                    std::vector<double>::const_iterator v2_begin,
                                    const std::size_t block_size) {
            for(std::size_t i = 0; i < block_size;
                ++i, ++v1_begin, ++v2_begin) {
                if(std::fabs(value(*v1_begin) - value(*v2_begin)) > 0.0)
                    return true;
            }
            return false;
        }
    };

To avoid changes that do not lead to new solutions, we must verify if such key
exchanges affect the solution. For that, the distance functor object must
implement the methods ``affectSolution()``, which come with two signatures,
depicted in the previous example.

In the first version, ``affectSolution()`` takes two keys and check whether the
exchange of ``key1`` by ``key2`` could change the solution. If so, the method
returns ``true``. The second version takes two iterators for two chromosomes
and checks ``block_size`` keys from those iterators. The idea is, instead to
check only individual keys, we check an entire block of keys. This is very
usual for path relinks that exchange blocks of keys instead individual ones.
For instance, suppose that the alleles/keys are used as threshold such that
values > 0.5 activate a feature. Suppose we have ``chromosome1 = [0.3, 0.4,
0.1, 0.8]`` and ``chromosome2 = [0.6, 0.1, 0.2, 0.9]``. If the key blocks start
on the first keys, and ``block_size = 2``, ``affectSolution()`` will return
``true`` since the very first keys have different activation value. However, if
we start from the 3rd keys and exchange blocks of 2 keys (``[0.4, 0.1]`` by
``[0.1, 0.2]``), nothing changes since both values have the same activation
level (< 0.5). The blocks can hold only one key/allele, sequential key blocks,
or even the whole chromosome.

.. note::
  Note that ``affectSolution()`` is crucial to the IPR performance since this
  function helps to avoid exploring regions already surveyed. Also, note that
  ``affectSolution()`` can incorporate some problem knowledge.

.. note::
  The current implementation of permutation path relink does not make use of
  ``affectSolution()``. However, ``pathRelink()``) requires the function.
  Therefore, we can implement simple constant methods:

  .. ref-code-block:: cpp

      virtual bool affectSolution(const double key1, const double key2) {
          return true;
      }

      virtual bool affectSolution(std::vector<double>::const_iterator v1_begin,
                                  std::vector<double>::const_iterator v2_begin,
                                  const std::size_t block_size) {
          return true;
      }

``block_size`` defines the number of keys / size of the chromosome block to be
exchanged during the direct path relink. This parameter is also critical for
IPR performance since it avoids too many exchanges during the path building.
Usually, we can compute this number based on the size of the chromosome by some
factor (``alpha_block_size`` in the configuration file), chosen by you. Again,
details `here <https://doi.org/10.1016/j.ejor.2019.11.037>`_.


.. note::
  Experiments have shown that a good choice is
  :math:`block\_size = alpha\_block\_size \times \sqrt{size~of~chromosome}`

The last two parameters are stopping criteria. The algorithm stops either when
``max_time`` seconds is reached or ``percentage`` % of the path is built.

.. warning::
  IPR is a very time-intensive process. You must set the stopping criteria
  accordingly.

Let's see the example on `main_complete.cpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/main_complete.cpp>`_.
Remember, since we are solving the TSP, we want to use the permutation-based
IPR, and the Kendall Tau distance functions.

.. ref-code-block:: cpp

    if(brkga_params.pr_type == :ref:`BRKGA::PathRelinking::Type::DIRECT <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76>`)
        dist_func.reset(new :ref:`BRKGA::HammingDistance <doxid-class_b_r_k_g_a_1_1_hamming_distance>`(0.5));
    else
        dist_func.reset(new :ref:`BRKGA::KendallTauDistance <doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`())
    ...
    auto result = algorithm.pathRelink(
        brkga_params.pr_type,
        brkga_params.pr_selection,
        dist_func,
        brkga_params.pr_number_pairs,
        1, // block_size doesn't not matter for permutation.
        max_time - elapsedFrom(start_time),
        brkga_params.pr_percentage)

Note that most parameters come from ``brkga_params``. The maximum IPR time is
set to the remaining time for optimization (global ``maximum_time`` minus the
elapsed time). ``pathRelink()`` returns a
``:ref:`BRKGA::PathRelinking::PathRelinkingResult
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>```
object which defines the status of the IPR optimization.

.. note::
  The ``TOO_HOMOGENEOUS`` status is directly linked to the chosen distance
  function and minimum distance. If the minimum distance is too large, IPR may
  not be able to find a pair of chromosomes far enough for path relink.

If the found solution is the best solution found so far, IPR replaces the worst
solution by it. Otherwise, IPR computes the distance between the found solution
and all other solutions in the elite set, and replaces the worst solution by it
if and only if the found solution is, at least, ``minimum_distance`` from all
them.


Important notes about IPR
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The API will call ``decode()`` function always with ``writeback = false``. The
reason is that if the decoder rewrites the chromosome, the path between
solutions is lost and inadvertent results may come up. Note that at the end of
the path relinking, the method calls the decoder with ``writeback = true`` in
the best chromosome found to guarantee that this chromosome is re-written to
reflect the best solution found.

.. warning::
  Make sure your decoder does not rewrite the chromosome when called with the
  argument ``writeback = false``.

BRKGA-MP-IPR ``pathRelink()`` implementation is multi-threaded. Instead of to
build and decode each chromosome one at a time, the method builds a list of
candidates, altering the alleles/keys according to the guide solution, and then
decode all candidates in parallel. Note that
:math:`O(chromosome\_size^2~/~block\_size)` additional memory is necessary to
build the candidates, which can be costly if the ``chromosome_size`` is very
large.

.. warning::
  As it is in method ``evolve()``, the decoding is done in parallel using
  threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If
  such property cannot be held, we suggest using a single thread for
  optimization.


.. _doxid-guide_1guide_shaking_reset:
Shaking and Resetting
-------------------------------------------------------------------------------

Sometimes, BRKGA gets stuck, converging to local maxima/minima, for several
iterations. When such a situation happens, it is a good idea to perturb the
population, or even restart from a new one completely new. BRKGA-MP-IPR offers
``:ref:`BRKGA::BRKGA_MP_IPR::shake()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>```,
an improved variation of the original version proposed in `this paper
<http://dx.doi.org/10.1016/j.eswa.2019.03.007>`_.

.. ref-code-block:: cpp

    void shake(unsigned intensity, :ref:`ShakingType <doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` shaking_type,
               unsigned population_index =
                   std::numeric_limits<unsigned>::infinity());

``shake()`` method gets an ``intensity`` parameter that measures how many times
the perturbation is applied on the elite set for a given ``population_index``
(if not given, all populations are shaken). This method offers two
generic/implicit ``:ref:`BRKGA::ShakingType
<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>``` s. With
``CHANGE``, direct modifications are done in the keys/alleles. This kind of
shaking is recommended when the chromosome uses direct or threshold
representations. ``SWAP`` exchanges keys/alleles inducing new permutations. For
representational definitions, please read `this paper
<https://doi.org/10.1016/j.ejor.2019.11.037>`_. For instance, the following
code shakes all populations using 10 swap moves:

.. ref-code-block:: cpp

    algorithm.shake(10, SWAP);

Sometimes, even shaking the populations does not help to escape from local
maxima/minima. So, we need a drastic measure, restarting from scratch the role
population. This can be easily accomplished with
``:ref:`BRKGA::BRKGA_MP_IPR::reset()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>```.

.. ref-code-block:: cpp

    algorithm.reset();

.. note::
  When using ``reset()``, all warm-start solutions provided by
  ``setInitialPopulation()`` are discarded. You may use ``injectChromosome()``
  to insert those solutions again.

.. warning::
  Again, the decoding of each chromosome is done in parallel if multi-thread is
  enabled. Therefore, **we must guarantee that the decoder is THREAD-SAFE.** If
  such property cannot be held, we suggest using a single thread..


.. _doxid-guide_1guide_migration:
Multi-population and migration
-------------------------------------------------------------------------------

Multi-population or *island model* was introduced in genetic algorithms in
`this paper
<http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.36.7225>`_. The idea
is to evolve parallel and independent populations and, once a while, exchange
individuals among these populations. In several scenarios, this approach is
very beneficial for optimization.

BRKGA-MP-IPR is implemented using such island idea from the core. If you read
the guide until here, you may notice that several methods take into account
multiple populations. To use multiple populations, you must set
``:ref:`BRKGA::BrkgaParams.num_independent_populations
<doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926>```
with 2 ou more populations, and build the BRKGA algorithm from such parameters.

The immigration process is implemented by

.. ref-code-block:: cpp

    void exchangeElite(unsigned num_immigrants);

``exchangeElite()`` copies ``num_immigrants`` from one population to another,
replacing the worst ``num_immigrants`` individuals from the recipient
population. Note that the migration is done for all pairs of populations. For
instance, the following code exchanges 3 best individuals from each population:

.. ref-code-block:: cpp

    algorithm.exchangeElite(3);


.. _doxid-guide_1guide_standard_brkga:
Simulating the standard BRKGA
-------------------------------------------------------------------------------

Sometimes, it is a good idea to test how the standard BRKGA algorithm performs
for a problem. You can use BRKGA-MP-IPR framework to quickly implement and test
a standard BRKGA.

First, you must guarantee that, during the crossover, the algorithm chooses
only one elite individual and only one non-elite individual. This is easily
accomplished setting ``num_elite_parents = 1`` and ``total_parents = 2``. Then,
you must set up a bias function that ranks the elite and no-elite individual
according to the original BRKGA bias parameter :math:`\rho` (rho).

You can use ``:ref:`BRKGA::BRKGA_MP_IPR::setBiasCustomFunction()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>```
for that task. The given function receives the index of the chromosome and
returns a ranking for it. Such ranking is used in the roulette method to choose
the individual from which each allele comes to build the new chromosome. Since
we have one two individuals for crossover in the standard BRKGA, the bias
function must return the probability to one or other individual. In the
following code, we do that with a simple ``if...else`` lambda function.

.. ref-code-block:: cpp

    // Create brkga_params by hand or reading from a file,
    // then set the following by hand.
    brkga_params.num_elite_parents = 1;
    brkga_params.total_parents = 2;

    const double rho = 0.75;
    algorithm.setBiasCustomFunction!([&](const unsigned x) {
        return x == 1 ? rho : 1.0 - rho;
    });
    algorithm.initialize();

Here, we first set the ``num_elite_parents = 1`` and ``total_parents = 2`` as
explained before. Following, we set a variable ``rho = 0.75``. This is the
:math:`\rho` from standard BRKGA, and you may set it as you wish. Then, we set
the bias function as a very simple lambda function (note that we must use
``[&]`` to capture ``rho`` in the outside context):

.. ref-code-block:: cpp

    [&](const unsigned x) {
        return x == 1 ? rho : 1.0 - rho;
    }

So, if the index of the chromosome is 1 (elite individual), it gets a 0.75
rank/probability. If the index is 2 (non-elite individual), the chromosome gets
0.25 rank/probability.

.. note::
  All these operations must be done before calling ``initialize()``.

.. warning::
    Note that we consider the index 1 as the elite individual instead index
    0, and index 2 to the non-elite individual opposed to index 1. The reason
    for this is that, internally, BRKGA always pass ``r > 0`` to the bias
    function to avoid division-by-zero exceptions.

.. _doxid-guide_1guide_parameters:
Reading and writing parameters
-------------------------------------------------------------------------------

Although we can build the BRKGA algorithm data by set up a
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` object
manually, the easiest way to do so is to read such parameters from a
configuration file. For this, we can use ``:ref:`BRKGA::readConfiguration()
<doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>``` that reads a
simple plain text file and returns a tuple of ``:ref:`BRKGA::BrkgaParams
<doxid-class_b_r_k_g_a_1_1_brkga_params>``` and
``:ref:`BRKGA::ExternalControlParams
<doxid-class_b_r_k_g_a_1_1_external_control_params>``` objects. For instance,

.. ref-code-block:: cpp

    // C++14 syntax.
    auto params = :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>`("tuned_conf.txt");
    auto& brkga_params = params.first;
    auto& control_params = params.second;

    // C++17 syntax. Isn't cool?
    auto [brkga_params, control_params] = :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>`("tuned_conf.txt");

The configuration file must be plain text such that contains pairs of
parameter name and value. This file must list all fields from
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` and
``:ref:`BRKGA::ExternalControlParams
<doxid-class_b_r_k_g_a_1_1_external_control_params>```, even though you do
not use each one at this moment. In `examples folder
<https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/v1.0/examples/tsp>`_, we
have `config.conf
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/config.conf>`_
that looks like this:

.. ref-code-block:: cpp

    population_size 2000
    elite_percentage 0.30
    mutants_percentage 0.15
    num_elite_parents 2
    total_parents 3
    bias_type :ref:`LOGINVERSE <doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adca762bd1443afdcf03af352da1c9184>`
    num_independent_populations 3
    pr_number_pairs 0
    pr_minimum_distance 0.15
    pr_type :ref:`PERMUTATION <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a48deaef68056f516e0091a15c1db3daa>`
    pr_selection :ref:`BESTSOLUTION <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`
    alpha_block_size 1.0
    pr_percentage 1.0
    exchange_interval 200
    num_exchange_individuals 2
    reset_interval 600

It does not matter whether we use lower or upper cases. Blank lines and lines
starting with ``#`` are ignored. The order of the parameters should not
matter either. And, finally, this file should be readble for both C++, Julia,
and Python framework versions.

In some cases, you define some of the parameters at the running time, and you
may want to save them for debug or posterior use. To do so, you can use
``:ref:`BRKGA::writeConfiguration()
<doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>```, call upon a
``BrkgaParams`` object.

.. ref-code-block:: cpp

    :ref:`writeConfiguration <doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>`("test.conf", brkga_params);
    //or
    :ref:`writeConfiguration <doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>`("test.conf", brkga_params, control_params);

If ``control_params`` is not given, default values are used in its place.

.. note::
  ``:ref:`BRKGA::writeConfiguration()
  <doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>```
  rewrites the given file. So, watch out to not lose previous configurations.


.. _doxid-guide_1guide_tips:
(Probable Valuable) Tips
-------------------------------------------------------------------------------

Algorithm warmup
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

While in Julia framework version is primordial to do a dry-run to precompile
all functions (and a good idea on Python version), in C++ and Python this
warmup is not necessary. However, few dry-runs can help the OS/processor with
cache locality and give some speedup..

Besides the dry-runs, I would recommend the pre-allocation of all
resource/memory that you need, if you know in advance how much will be
necessary. This pre-allocation speeds the decoding process dramatically.


Complex decoders and timing
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Some problems require complex decoders while for others, the decoder contains
local search procedures, that can be time-consuming. In general, the decoding
is the most time-expensive component of a BRKGA algorithm, and it may skew some
stopping criteria based on running time. Therefore, if your decoder is
time-consuming, it is a good idea to implement a timer or chronometer kind of
thing inside the decoder.

Testing for stopping time uses several CPU cycles, and you need to be careful
when/where to test it, otherwise, you spend all the optimization time doing
system calls to the clock.

IMHO, the most effective way to do it is to test time at the very end of the
decoding. If the current time is larger than the maximum time allowed, simple
return ``Inf`` or ``-Inf`` according to your optimization direction. In this
way, we make the solution **invalid** since it violates the maximum time
allowed. The BRKGA framework takes care of the rest.

Multi-threading
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Since `Moore's law <https://en.wikipedia.org/wiki/Moore%27s_law>`_ is not
holding its status anymore, we, simple mortals, must appeal to the wonders of
multi-threading. This paradigm can be tricky to code, and `Amdahl's law
<https://en.wikipedia.org/wiki/Amdahl%27s_law>`_ plays against us. Several
genetic algorithms, and in particular, BRKGA, can use parallel solution
evaluation (or decoding), which makes the use of multi-threading relatively
straightforward. BRKGA-MP-IPR C++ framework is not different, and it uses
`OpenMP <https://www.openmp.org>`_ capabilities to do so.

First, as commented several times in this guide, **the decoder must be
THREAD-SAFE.** So, each thread must have its own read/write data structures and
may share other read-only data. The simplest way to do it is to create those
structures inside the decoder (like most people do). **But be aware**, this
strategy slows down the algorithm significantly depending on the size and
format of the structures, and *I do not recommend it*.

IMHO, the best way to do that is to preallocate the data structure per thread
and pass them to the decoder through the problem instance. Then, inside the
decoder, you can use `omp_get_thread_num()
<https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf>`_
and recover the memory you want to use.

Let's see a simple example considering the TSP example. ``TSP_Decode`` uses a
single array to create the permutation of nodes. Let's pre-allocate its memory
per thread. To keep things separeted and easy to understand, we created a new
class `TSP_Decoder_pre_allocating <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/v1.0/examples/tsp/src/decoders/tsp_decoder_pre_allocating.hpp>`_
so that we allocate, for each thread, a vector to hold the permutation during
the decoding:

.. ref-code-block:: cpp

    class TSP_Decoder_pre_allocating {
    public:
        TSP_Decoder_pre_allocating(const TSP_Instance& instance,
                                   const unsigned num_threads = 1);

        double decode(:ref:`BRKGA::Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome, bool rewrite = true);

    public:
        const TSP_Instance& instance;

    protected:
        typedef std::vector<std::pair<double, unsigned>> Permutation;
        std::vector<Permutation> permutation_per_thread;
    };

Note that the constructor has one more argument indicating how many threads we
are using. We also define a type ``Permutation`` with is a simple vector of
key-node pairs. The important structure is ``permuration_per_thread`` which is
a simple vector of the size of the number of threads where we allocate the
permutation vectors for each thread.

Then, in the implementation, we allocate all memory in the constructor. In
``decode``, we use `omp_get_thread_num()
<https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf>`_
to identify which thread called the decoder, and retrieve the respective data
strucuture.

.. ref-code-block:: cpp

    #include <omp.h>

    TSP_Decoder_pre_allocating::TSP_Decoder_pre_allocating(
                const TSP_Instance& _instance, const unsigned num_threads):
        instance(_instance),
        // Pre-allocate space for permutations for each thread.
        permutation_per_thread(num_threads, Permutation(instance.num_nodes))
    {}

    double TSP_Decoder_pre_allocating::decode(:ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome,
                                              bool /* not-used */) {
        // If you have OpenMP available, get the allocated memory per thread ID.
        #ifdef _OPENMP
        auto& permutation = permutation_per_thread[omp_get_thread_num()];
        #else
        auto& permutation = permutation_per_thread[0];
        #endif

        for(unsigned i = 0; i < instance.num_nodes; ++i)
            permutation[i] = make_pair(chromosome[i], i);

        sort(permutation.begin(), permutation.end());

        double cost = instance.distance(permutation.front().second,
                                        permutation.back().second);

        for(unsigned i = 0; i < instance.num_nodes - 1; ++i)
            cost += instance.distance(permutation[i].second,
                                      permutation[i + 1].second);

        return cost;
    }

.. note::
  Pre-allocation and multi-threading only make sense for large data structures
  and time-consuming decoders. Otherwise, the code spends too much time on
  context switching and system calls.

.. warning::
  Multi-threading consumes many resources of the machine and have diminishing
  returns. I recommend using **at most the number of physical cores** (may -1)
  to avoid racing and too much context switching. You must test which is the
  best option for your case. In my experience, complex decoders benefit more
  from multi-threading than simple and fast decoders.


Known issues
-------------------------------------------------------------------------------

One of the interesting features implemented in this C++ BRKGA-MP-IPR framework
is the capability to easily load and write the algorithm configuration into
text files. To enable this feature, we borrow some nice code from Bradley Plohr
(`https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
<https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header>`_),
which make easy to read and write enumerations from standard streams (cout and
cerr).

However, since BRKGA-MP-IPR is header-only, this feature can cause some
headaches during the linking, especially if you include the BRKGA-MP-IPR header
in different modules (translation units on C++ jargon) and compile them
separately (which normally we do). For example, suppose we have two pieces of
code, module_a.cpp and module_b.cpp, such that we include BRKGA in both (i.e.,
``#include <brkga_mp_ipr>`` in both files.

File module_a.cpp

.. ref-code-block:: cpp

    #include "brkga_mp_ipr.hpp"
    int main() {
        auto params = :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>`("config.conf");
        return 0;
    }

File module_b.cpp

.. ref-code-block:: cpp

    #include "brkga_mp_ipr.hpp"
    void test() {
        auto params = :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>`("config.conf");
    }

Let's compile each one with GCC and link them:

.. ref-code-block::

    $ g++ -std=c++14 -I../brkga_mp_ipr -c module_a.cpp -o module_a.o

    $ g++ -std=c++14 -I../brkga_mp_ipr -c module_b.cpp -o module_b.o

    $ g++ module_a.o module_b.o -o test
    duplicate symbol EnumIO<BRKGA::PathRelinking::Selection>::enum_names[abi:cxx11]()    in:
        module_a.o
        module_b.o
    duplicate symbol EnumIO<BRKGA::Sense>::enum_names[abi:cxx11]()    in:
        module_a.o
        module_b.o
    duplicate symbol EnumIO<BRKGA::BiasFunctionType>::enum_names[abi:cxx11]()     in:
        module_a.o
        module_b.o
    duplicate symbol EnumIO<BRKGA::PathRelinking::Type>::enum_names[abi:cxx11]()    in:
        module_a.o
        module_b.o
    duplicate symbol BRKGA::writeConfiguration(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, BRKGA::BrkgaParams const&, BRKGA::ExternalControlParams const&) in:
        module_a.o
        module_b.o
    duplicate symbol BRKGA::readConfiguration(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) in:
        module_a.o
        module_b.o
    ld: 6 duplicate symbols for architecture x86_64
    collect2: error: ld returned 1 exit status

Let's try with Clang:

.. ref-code-block::

    $ clang++-mp-7.0 -std=c++14 -pthread -fopenmp -I../brkga_mp_ipr -c module_a.cpp -o module_a.o

    $ clang++-mp-7.0 -std=c++14 -pthread -fopenmp -I../brkga_mp_ipr -c module_b.cpp -o module_b.o

    $ clang++-mp-7.0 module_a.o module_b.o -o test
    duplicate symbol __ZN6EnumIOIN5BRKGA13PathRelinking9SelectionEE10enum_namesEv in:
        module_a.o
        module_b.o
    duplicate symbol __ZN6EnumIOIN5BRKGA5SenseEE10enum_namesEv in:
        module_a.o
        module_b.o
    duplicate symbol __ZN6EnumIOIN5BRKGA16BiasFunctionTypeEE10enum_namesEv in:
        module_a.o
        module_b.o
    duplicate symbol __ZN6EnumIOIN5BRKGA13PathRelinking4TypeEE10enum_namesEv in:
        module_a.o
        module_b.o
    duplicate symbol __ZN5BRKGA18writeConfigurationERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEERKNS_11BrkgaParamsERKNS_21ExternalControlParamsE in:
        module_a.o
        module_b.o
    duplicate symbol __ZN5BRKGA17readConfigurationERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE in:
        module_a.o
        module_b.o
    ld: 6 duplicate symbols for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)

So, note that we have several duplicated symbols, which are the ``enum``
writer/readers, and the two stand-alone functions
``:ref:`BRKGA::writeConfiguration()
<doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>``` and
``:ref:`BRKGA::readConfiguration()
<doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>```.

To avoid such a situation, we have to ``inline`` these functions and template
specializations. We can do that passing the compiler directive
``BRKGA_MULTIPLE_INCLUSIONS`` which inlines the functions and template
specializations properly.

.. ref-code-block::

    $ g++ -std=c++14 -I../brkga_mp_ipr -DBRKGA_MULTIPLE_INCLUSIONS -c module_a.cpp -o module_a.o

    $ g++ -std=c++14 -I../brkga_mp_ipr -DBRKGA_MULTIPLE_INCLUSIONS -c module_b.cpp -o module_b.o

    $ g++ module_a.o module_b.o -o test

However, we have two side-effects. First, such inlining can make the object
code grows large since we include several copies of the same functions (which
are I/O functions which already are large by their nature). Second, the
compiler may complain about too many inline functions, if you are already using
several inline functions.

.. warning::
  Avoid including ``brkga_mp_ip.hpp`` in several files/translation units. If
  unavoidable, use the compiler directive ``BRKGA_MULTIPLE_INCLUSIONS``.

But now, suppose we must use multiple inclusions of BRKGA header, and our
compiler finds issues on inline such functions. The last resource is to move
functions ``:ref:`BRKGA::writeConfiguration()
<doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>``` and
``:ref:`BRKGA::readConfiguration()
<doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>```, and all enum
template specializations (at the end of file ``brkga_mp_ipr.hpp``), to a unique
translation unit. I recommend to it on your ``main()`` module, so that they are
compiled just once.

