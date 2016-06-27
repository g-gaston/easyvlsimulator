#include "net_analysis.h"
#include "netlist.h"

#ifndef SIM_CYCLES
#define SIM_CYCLES 1000
#endif

int main(int argc, char *argv[]) {
	netlist nlist;

	if(net_analysis(argc, argv, nlist) != 0) {
		return -1;
	}

	for (std::vector<output_gate *>::iterator o_gates_it = nlist.out_gates.begin();
		o_gates_it != nlist.out_gates.end(); ++o_gates_it) {
		(*o_gates_it)->print_first_info();
	}

	for (int i = 0; i < SIM_CYCLES; ++i) {
		for (std::vector<net *>::iterator nets_it = nlist.nets.begin();
			nets_it != nlist.nets.end(); ++nets_it) {
			(*nets_it)->reset_value();
		}
		for (std::vector<pin *>::iterator pins_it = nlist.pins.begin();
			pins_it != nlist.pins.end(); ++pins_it) {
			(*pins_it)->reset_pin_value();
		}
		for (std::vector<flip_flop *>::iterator ff_gates_it = nlist.ff_gates.begin();
			ff_gates_it != nlist.ff_gates.end(); ++ff_gates_it) {
			(*ff_gates_it)->clock_tick();
		}
		for (std::vector<output_gate *>::iterator o_gates_it = nlist.out_gates.begin();
			o_gates_it != nlist.out_gates.end(); ++o_gates_it) {
			if ((*o_gates_it)->print_value() != 0) {
				std::cerr << "Simulation problem, aborting" << std::endl;
				return -1;
			}
		}
		for (std::vector<flip_flop *>::iterator ff_gates_it = nlist.ff_gates.begin();
			ff_gates_it != nlist.ff_gates.end(); ++ff_gates_it) {
			(*ff_gates_it)->get_new_state();
		}
		for (std::vector<input_gate *>::iterator i_gates_it = nlist.in_gates.begin();
			i_gates_it != nlist.in_gates.end(); ++i_gates_it) {
			(*i_gates_it)->end_cycle();
		}
	}

	return 0;
}
