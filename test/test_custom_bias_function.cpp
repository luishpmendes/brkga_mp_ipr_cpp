/******************************************************************************
 * test_nsbrkga_mp_ipr.cpp: test implicit path relinking on NSBRKGA_IPR.
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Jan 06, 2015 by andrade
 *  Last update: Feb 21, 2018 by andrade
 *
 * This code is released under LICENSE.md.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "nsbrkga_mp_ipr.hpp"
#include "decoders.hpp"

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;
using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    if(argc < 4) {
        cerr << "Usage: "<< argv[0]
             << " <chromosome-size> <block_size> <seed>" << endl;
        return 1;
    }

    try {
        const unsigned chr_size = atoi(argv[1]);
        size_t block_size = atoi(argv[2]);
        const unsigned seed = atoi(argv[3]);

        if(block_size > chr_size)
            block_size = chr_size;

        cout << "\n> chr_size " << chr_size
             << "\n> block_size " << block_size
             << endl;

        auto params = readConfiguration("config.conf");
        auto& brkga_params = params.first;

        const bool evolutionary_mechanism_on = true;
        const unsigned max_threads = 1;

//        Sum_Decoder decoder;
        Sum_Decoder decoder;

        // The NSBRKGA_MP_IPR algorithm object.
        NSBRKGA_MP_IPR<Sum_Decoder> algorithm(decoder, BRKGA::Sense::MINIMIZE,
                seed, chr_size, brkga_params, evolutionary_mechanism_on,
                max_threads);

        cout << "log\n";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / log1p(r); }
        );
        cout << endl;

        cout << "LINEAR\n";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / r; }
        );
        cout << endl;

        cout << "QUADRATIC\n";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -2); }        );
        cout << endl;

        cout << "CUBIC\n";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -3); }        );
        cout << endl;

        cout << "EXPONENTIAL\n";
        algorithm.setBiasCustomFunction(
                [](const unsigned r) { return exp(-1.0 * r); }                );
        cout << endl;

        cout << "CONSTANT\n";
        algorithm.setBiasCustomFunction(
                [&](const unsigned) { return 1.0 / brkga_params.total_parents; }        );
        cout << endl;

        cout << "ERROR\n";
        algorithm.setBiasCustomFunction(
                [&](const unsigned r) { return 2.0 * r; }        );
        cout << endl;
    }
    catch(exception& e) {
        cerr << "\n***********************************************************"
             << "\n****  Exception Occured: " << e.what()
             << "\n***********************************************************"
             << endl;
        return 70; // BSD software internal error code
    }
    return 0;
}
