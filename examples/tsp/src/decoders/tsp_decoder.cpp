/******************************************************************************
 * tsp_instance.cpp: Implementation for TSP_Instance class.
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Mar 05, 2019 by andrade
 *  Last update: Jul 02, 2020 by luishpmendes
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

#include "decoders/tsp_decoder.hpp"

#include <algorithm>

using namespace std;
using namespace BRKGA;

//-----------------------------[ Constructor ]--------------------------------//

TSP_Decoder::TSP_Decoder(const TSP_Instance& _instance):
    instance(_instance)
{}

//-------------------------------[ Decode ]-----------------------------------//

std::vector<double> TSP_Decoder::decode(Chromosome& chromosome, 
        bool /* not-used */) {
    vector<pair<double, unsigned>> permutation(instance.num_nodes);
    for(unsigned i = 0; i < instance.num_nodes; ++i) {
        permutation[i] = make_pair(chromosome[i], i);
    }

    sort(permutation.begin(), permutation.end());

    double cost = instance.distance(permutation.front().second,
                                    permutation.back().second);

    for(unsigned i = 0; i < instance.num_nodes - 1; ++i) {
        cost += instance.distance(permutation[i].second,
                                  permutation[i + 1].second);
    }

    return std::vector<double>(1, cost);
}

