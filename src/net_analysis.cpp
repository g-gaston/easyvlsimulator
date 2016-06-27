#include "syn.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include "simlib.h"
#include "net_analysis.h"

#ifndef PRINT_NETLIST
#define PRINT_NETLIST 0
#endif

enum gate_width_restriction{EQUAL, ANY};

int process_module (int argc, char *argv[], Evl_module &module, netlist &nlist,
					std::map<std::string, Evl_module *> &modules_table, std::vector<net *> ports_nets);
int create_pin(int w, gate *g, netlist &nlist, int pos, std::map<std::string, net *> &nets_table,
	std::string name, int first_bit_in_bus);

int link_with_buf (netlist &nlist, std::string port_name_aux,
	std::vector<net *>::iterator ports_nets_it, std::string file_name,
	std::map<std::string, net *> &nets_table, int i);

void create_aux_net (std::string port_name_aux, int pos, netlist &nlist,
					std::map<std::string, net *> &nets_table, net *net_to_link,
					std::string net_alias);

int net_analysis(int argc, char *argv[], netlist &nlist) {

	std::vector<Evl_module> modules;
	Evl_module top_module;

	if(syn_analysis(argc, argv, modules) != 0) {
		return -1;
	}

	std::map<std::string, Evl_module *> modules_table;

	for (std::vector<Evl_module>::iterator modules_it = modules.begin()
		; modules_it != modules.end(); ++modules_it) {
		if ((*modules_it).get_ports().size() == 0) {
			top_module = (*modules_it);
		} else {

			modules_table[(*modules_it).get_type()] = &(*modules_it);
		}
	}

	std::vector<net *> empty_ports_nets;
	return process_module(argc, argv, top_module, nlist, modules_table, empty_ports_nets);

}

int link_with_buf (netlist &nlist, std::string input_name, int input_first_bit,
					std::string output_name, int output_first_bit, std::string file_name,
					std::map<std::string, net *> &nets_table) {

	//Create buf to link
	gate *g = nlist.create_gate("buf", "buf_" + input_name + "_" + output_name, file_name);
	if (create_pin(1, g, nlist, 0, nets_table, output_name, output_first_bit) != 0) {
		return -1;
	}
	if (create_pin(1, g, nlist, 1, nets_table, input_name, input_first_bit) != 0) {
		return -1;
	}
	(*g).set_pin_num(2);
	return 0;
}

void create_aux_net (std::string port_name_aux, int pos, netlist &nlist,
					std::map<std::string, net *> &nets_table, net *net_to_link,
					std::string net_alias) {
	net *n = new net("aux_net_" + port_name_aux + "_" + net_to_link->get_name(), pos);
	nlist.nets.push_back(n);
	nets_table[port_name_aux] = n;
	nets_table[net_alias] = net_to_link;
}

int process_module (int argc, char *argv[], Evl_module &module, netlist &nlist,
	std::map<std::string, Evl_module *> &modules_table, std::vector<net *> ports_nets) {

	std::map<std::string, net *> nets_table;
	std::map<std::string, int> wires_table;

	std::vector<Evl_port> ports = module.get_ports();
	std::vector<net *>::iterator ports_nets_it = ports_nets.begin();


	// No top module
	if (ports.size() != 0) {
		for (std::vector<Evl_port>::iterator ports_it = ports.begin()
			; ports_it != ports.end(); ++ports_it) {
			if (ports_nets_it == ports_nets.end()) {
				return -1;
			}
			int port_width = (*ports_it).get_width();
			std::string port_name = (*ports_it).get_name();
			std::string port_name_aux;
			wires_table[port_name] = port_width;
			std::string net_alias;
			if (port_width > 1) {
				for (int i = 0; i < port_width; ++i) {
					port_name_aux = port_name + "[" + itos(i) + "]";

					net_alias = (*ports_nets_it)->get_name() + "_$_alias";
					create_aux_net(port_name_aux, i, nlist, nets_table, (*ports_nets_it), net_alias);

					//std::cout << "Linking " << port_name << "[" << itos(i) << "] -> " << (*ports_nets_it)->get_name() << std::endl;
					if ((*ports_it).get_port_type() == PORT_OUT) {
						if (link_with_buf(nlist, port_name_aux, i, net_alias, (*ports_nets_it)->get_bus_pos(), std::string(argv[1]), nets_table) != 0) {
							return -1;
						}
					} else if ((*ports_it).get_port_type() == PORT_IN) {
						if (link_with_buf(nlist, net_alias, (*ports_nets_it)->get_bus_pos(), port_name_aux, i, std::string(argv[1]), nets_table) != 0) {
							return -1;
						}
					}

					ports_nets_it++;
				}
			} else {
				net_alias = (*ports_nets_it)->get_name() + "_$_alias";
				create_aux_net(port_name, 0, nlist, nets_table, (*ports_nets_it), net_alias);

				if ((*ports_it).get_port_type() == PORT_OUT) {
					if (link_with_buf(nlist, port_name, 0, net_alias, (*ports_nets_it)->get_bus_pos(), std::string(argv[1]), nets_table) != 0) {
						return -1;
					}
				} else if ((*ports_it).get_port_type() == PORT_IN) {
					if (link_with_buf(nlist, net_alias, (*ports_nets_it)->get_bus_pos(), port_name, 0, std::string(argv[1]), nets_table) != 0) {
						return -1;
					}
				}
				ports_nets_it++;
			}
		}
	}

	// List nets
	std::vector<Evl_wire> wires_vec = module.get_wires();
	int wire_width;
	std::string wire_name;
	std::string wire_name_aux;
	for (std::vector<Evl_wire>::iterator wires_it = wires_vec.begin();
		wires_it != wires_vec.end(); ++wires_it) {
		wire_width = (*wires_it).get_width();
		wire_name = (*wires_it).get_name();
		wires_table[wire_name] = wire_width;

		if (wire_width > 1) {
			for (int i = 0; i < wire_width; ++i) {
				wire_name_aux = wire_name + "[" + itos(i) + "]";
				std::map<std::string, net *>::iterator net_it = nets_table.find(wire_name_aux);
				if (net_it != nets_table.end()) {
					std::cerr << "Wire or BUS already defined" << std::endl;
					return -1;
				}
				net *n = new net(wire_name_aux, i);
				nets_table[wire_name_aux] = n;
				nlist.nets.push_back(n);
			}
		} else {
			std::map<std::string, net *>::iterator net_it = nets_table.find(wire_name);
			if (net_it != nets_table.end()) {
				std::cerr << "Wire or BUS already defined" << std::endl;
				return -1;
			}
			net *n = new net(wire_name);
			nets_table[wire_name] = n;
			nlist.nets.push_back(n);
		}
	}

	std::vector<Evl_assign> assign_vec = module.get_assignments();
	for (std::vector<Evl_assign>::iterator assign_it = assign_vec.begin()
		; assign_it != assign_vec.end(); ++assign_it) {
		std::map<std::string, int>::iterator wire_it_l = wires_table.find((*assign_it).get_lhs_name());
		std::map<std::string, int>::iterator wire_it_r = wires_table.find((*assign_it).get_rhs_name());
		if (wire_it_l == wires_table.end()) {
			std::cerr << "Assign refers to a wire (" << (*assign_it).get_lhs_name() << ") which does not exist" << std::endl;
			return -1;
		} else if (wire_it_r == wires_table.end()) {
			std::cerr << "Assign refers to a wire (" << (*assign_it).get_rhs_name() << ") which does not exist" << std::endl;
			return -1;
		}

		if (wire_it_l->second == 1) {
			if (wire_it_r->second == 1) {
				if (link_with_buf(nlist, (*assign_it).get_rhs_name(), 0, (*assign_it).get_lhs_name(), 0, std::string(argv[1]), nets_table) != 0) {
					return -1;
				}
			} else {
				if (link_with_buf(nlist, (*assign_it).get_rhs_name()+"["+itos((*assign_it).get_rhs_msb())+"]", (*assign_it).get_rhs_msb(),
					(*assign_it).get_lhs_name(), 0, std::string(argv[1]), nets_table) != 0) {
					return -1;
				}
			}
		} else if (wire_it_r->second == 1) {
			if (link_with_buf(nlist, (*assign_it).get_rhs_name(), 0,
				(*assign_it).get_lhs_name()+"["+itos((*assign_it).get_lhs_msb())+"]", (*assign_it).get_lhs_msb(), std::string(argv[1]), nets_table) != 0) {
				return -1;
			}
		} else {
			int lhs_first;
			int lhs_last;
			int rhs_first;
			int rhs_last;
			if ((*assign_it).get_lhs_msb() == -1 && (*assign_it).get_lhs_lsb() == -1) {
				lhs_first = 0;
				lhs_last = wire_it_l->second - 1;
			} else if ((*assign_it).get_lhs_lsb() == -1) {
				lhs_first = (*assign_it).get_lhs_msb();
				lhs_last = lhs_first;
			} else {
				lhs_first = (*assign_it).get_lhs_lsb();
				lhs_last = (*assign_it).get_lhs_msb();
				//std::cout << "lhs: " << itos(lhs_first) << " " << itos(lhs_last) << std::endl;
			}

			//std::cout << "wire: " << (*assign_it).get_rhs_name() <<  " rhs_lsb: " << itos((*assign_it).get_rhs_lsb()) << " rhs_msb: " << itos((*assign_it).get_rhs_msb()) << std::endl;

			if ((*assign_it).get_rhs_msb() == -1 && (*assign_it).get_rhs_lsb() == -1) {
				rhs_first = 0;
				rhs_last = wire_it_r->second - 1;
			} else if ((*assign_it).get_rhs_lsb() == -1) {
				rhs_first = (*assign_it).get_rhs_msb();
				rhs_last = rhs_first;
			} else {
				rhs_first = (*assign_it).get_rhs_lsb();
				rhs_last = (*assign_it).get_rhs_msb();
				//std::cout << "rhs: " << itos(rhs_first) << " " << itos(rhs_last) << std::endl;
			}

			//std::cout << "lhs_first: " << itos(lhs_first) << " lhs_last: " << itos(lhs_last) << " lrhs_first: " << itos(rhs_first) << " rhs_last: " << itos(rhs_last) << std::endl;

			int i = lhs_first;
			int j = rhs_first;
			while(i <= lhs_last && j <= rhs_last) {
				if (link_with_buf(nlist, (*assign_it).get_rhs_name()+"["+itos(j)+"]", j, (*assign_it).get_lhs_name()+"["+itos(i)+"]", i, std::string(argv[1]), nets_table) != 0) {
					return -1;
				}
				i++;
				j++;
			}
		}
	}

	// Linking pins to nets and nets to pins
	std::vector<Evl_component> components_vec = module.get_components();
	int pos = 0;
	std::string gate_type;
	gate_width_restriction width_res;
	int width_res_val;
	for (std::vector<Evl_component>::iterator components_it = components_vec.begin()
		; components_it != components_vec.end(); ++components_it) {

		gate_type = (*components_it).get_type();
		std::vector<Evl_pin> pins_vec = (*components_it).get_pins();
		std::map<std::string, Evl_module *>::iterator module_it = modules_table.find(gate_type);

		if (gate_type == "and" || gate_type == "or" || gate_type == "xor" || gate_type == "xnor") {
			if (pins_vec.size() < 3) {
				std::cerr << "Gate " << gate_type << " should have at least 3 pins" << std::endl;
				return -1;
			}
			width_res = EQUAL;
			width_res_val = 1;
		} else if (gate_type == "not" || gate_type == "buf") {
			if (pins_vec.size() != 2) {
				std::cerr << "Gate " << gate_type << " should have only 2 pins" << std::endl;
				return -1;
			}
			width_res = EQUAL;
			width_res_val = 1;
		} else if (gate_type == "evl_dff" || gate_type == "tris") {
			if (pins_vec.size() != 3) {
				std::cerr << "Gate " << gate_type << " should have only 3 pins" << std::endl;
				return -1;
			}
			width_res = EQUAL;
			width_res_val = 1;
		} else if (gate_type == "evl_clock") {
			if (pins_vec.size() != 1) {
				std::cerr << "Gate " << gate_type << " should have only 1 pin" << std::endl;
				return -1;
			}
			width_res = EQUAL;
			width_res_val = 1;
		} else if (gate_type == "evl_one" || gate_type == "evl_zero" || gate_type == "evl_input") {
			if (pins_vec.size() < 1) {
				std::cerr << "Gate " << gate_type << " should have at least 1 pin" << std::endl;
				return -1;
			}
			width_res = ANY;
			width_res_val = 0;
		} else if (gate_type == "evl_output") {
			if (pins_vec.size() < 1) {
				std::cerr << "Gate " << gate_type << " should have at least 1 pin" << std::endl;
				return -1;
			}
			width_res = ANY;
			width_res_val = 0;
		} else if (gate_type == "evl_lut") {
			if (pins_vec.size() != 2) {
				std::cerr << "Gate " << gate_type << " should have only 2 pins" << std::endl;
				return -1;
			}
			width_res = ANY;
			width_res_val = 0;
		} else if (module_it != modules_table.end()) {
			// Preparing list of connected nets
			std::vector<net *> ports_nets_for_module;
			for (std::vector<Evl_pin>::iterator pins_it = pins_vec.begin()
				; pins_it != pins_vec.end(); ++pins_it) {
				std::map<std::string, int>::iterator wire_it = wires_table.find((*pins_it).get_name());
				if (wire_it == wires_table.end()) {
					std::cerr << "Pin refers to a wire which does not exist" << std::endl;
					return -1;
				}
				int msb = (*pins_it).get_msb();
				int lsb = (*pins_it).get_lsb();
				std::map<std::string, net *>::iterator net_it;
				if (wire_it->second == 1) {
					net_it = nets_table.find((*pins_it).get_name());
					if (net_it == nets_table.end()) {
						std::cerr << "Net does not exist1" << std::endl;
						return -1;
					}
					ports_nets_for_module.push_back(net_it->second);
				} else {
					if (msb == -1 && lsb == -1) {
						for (int i = 0; i < wire_it->second; ++i) {
							net_it = nets_table.find((*pins_it).get_name()+"["+itos(i)+"]");
							if (net_it == nets_table.end()) {
								std::cerr << "Net does not exist2" << std::endl;
								return -1;
							}
							ports_nets_for_module.push_back(net_it->second);
						}
					} else if (lsb == -1) {
						net_it = nets_table.find((*pins_it).get_name()+"["+itos(msb)+"]");
						if (net_it == nets_table.end()) {
							std::cerr << "Net does not exist3" << std::endl;
							return -1;
						}
						ports_nets_for_module.push_back(net_it->second);
					} else {
						for (int i = lsb; i <= msb; ++i) {
							net_it = nets_table.find((*pins_it).get_name()+"["+itos(i)+"]");
							if (net_it == nets_table.end()) {
								std::cerr << "Net does not exist4" << std::endl;
								return -1;
							}
							ports_nets_for_module.push_back(net_it->second);
						}
					}
				}
			}

			//Procesing module
			if (process_module(argc, argv, *(module_it->second), nlist, modules_table, ports_nets_for_module) != 0) {
				return -1;
			}
			ports_nets_for_module.clear();
			continue;
		} else {
			std::cerr << "Unknow " << gate_type << " gate type." << std::endl;
			return -1;
		}

		// Añadir protección por si la gate ya ha sido definida ??? Puede haber varias puertas con mismo nombre y tipo?
		//gate *g = new gate(gate_type, (*components_it).get_name());
		gate *g = nlist.create_gate(gate_type, (*components_it).get_name(), std::string(argv[1]));

		pos = 0;
		for (std::vector<Evl_pin>::iterator pins_it = pins_vec.begin()
			; pins_it != pins_vec.end(); ++pins_it) {

			std::map<std::string, int>::iterator wire_it = wires_table.find((*pins_it).get_name());
			if (wire_it == wires_table.end()) {
				std::cerr << "Pin refers to a wire which does not exist" << std::endl;
				return -1;
			}
			int msb = (*pins_it).get_msb();
			int lsb = (*pins_it).get_lsb();
			std::map<std::string, net *>::iterator net_it;

			if (wire_it->second == 1) {
				if (msb == -1 && lsb == -1) {
					if (width_res == EQUAL && width_res_val != 1) {
						std::cerr << "Incorrect pin width in gate " << gate_type <<
							", should have width " << itos(width_res_val) << std::endl;
						return -1;
					}
					create_pin(wire_it->second, g, nlist, pos, nets_table, (*pins_it).get_name(), 0);
				} else {
					std::cerr << "Wire is not BUS" << std::endl;
					return -1;
				}
			} else if (wire_it->second >= 2) {
				if (msb == -1 && lsb == -1) {
					if (width_res == EQUAL && width_res_val != wire_it->second) {
						std::cerr << "Incorrect pin width in gate " << gate_type <<
							", should have width " << itos(width_res_val) << std::endl;
						return -1;
					}
					pin *p = new pin(wire_it->second, g, pos, 0);
					nlist.pins.push_back(p);
					for (int i = 0; i < wire_it->second; ++i) {
						std::map<std::string, net *>::iterator net_it = nets_table.find((*pins_it).get_name()+"["+itos(i)+"]");
						if (net_it == nets_table.end()) {
							std::cerr << "Net " << (*pins_it).get_name()+"["+itos(i)+"]" << " does not exist" << std::endl;
							return -1;
						}
						(*p).add_net(net_it->second);
						(*(net_it->second)).add_pin(p);
						if ((*g).is_output_pin(pos)) {
							(*(net_it->second)).set_driver_pin(p);
							p->set_pin_type(OUT);
						} else {
							p->set_pin_type(IN);
						}
					}
					(*g).add_pin(p);
				} else if (lsb == -1) {
					if (wire_it->second > msb && msb >= 0) {
						if (width_res == EQUAL && width_res_val != 1) {
							std::cerr << "Incorrect pin width in gate " << gate_type <<
								", should have width " << itos(width_res_val) << std::endl;
							return -1;
						}
						if(create_pin(1, g, nlist, pos, nets_table, (*pins_it).get_name()+"["+itos(msb)+"]", msb) != 0) {
							std::cerr << "Net does not exist6" << std::endl;
							return -1;
						}
					} else {
						std::cerr << "Malformed BUS" << std::endl;
						return -1;
					}
				} else {
					if (wire_it->second > msb && msb >= lsb && lsb >= 0) {
						if (width_res == EQUAL && width_res_val != msb-lsb+1) {
							std::cerr << "Incorrect pin width in gate " << gate_type <<
								", should have width " << itos(width_res_val) << std::endl;
							return -1;
						}
						pin *p = new pin(msb-lsb+1, g, pos, lsb);
						nlist.pins.push_back(p);
						for (int i = lsb; i <= msb; ++i) {
							std::map<std::string, net *>::iterator net_it = nets_table.find((*pins_it).get_name()+"["+itos(i)+"]");
							if (net_it == nets_table.end()) {
								std::cerr << "Net not exist" << std::endl;
								return -1;
							}
							(*p).add_net(net_it->second);
							(*(net_it->second)).add_pin(p);
							if ((*g).is_output_pin(pos)) {
								(*(net_it->second)).set_driver_pin(p);
								p->set_pin_type(OUT);
							} else {
								p->set_pin_type(IN);
							}
						}
						(*g).add_pin(p);
					} else {
						std::cerr << "Malformed BUS" << std::endl;
						return -1;
					}
				}
			}
			pos++;
		}
		(*g).set_pin_num(pos);
	}

#if PRINT_NETLIST

	//Print Top module
	std::cout << "module "
		<< module.get_type() << std::endl;

	//Print Nets
	if (nlist.nets.size() > 0) {
		std::cout << "nets "
			<< nlist.nets.size() << std::endl;
	}

	std::vector<pin *> pins_net_vec;
	std::string aux_str = "";
	gate *aux_gate;
	for (std::vector<net *>::iterator nets_it = nlist.nets.begin()
		; nets_it != nlist.nets.end(); ++nets_it) {

		pins_net_vec = (*(*nets_it)).get_pins();

		std::cout << "\tnet "
		<< (*(*nets_it)).get_name() << " " << pins_net_vec.size() << std::endl;

		for (std::vector<pin *>::iterator pins_net_it = pins_net_vec.begin();
		 pins_net_it != pins_net_vec.end(); ++pins_net_it) {
			aux_gate = ((*(*pins_net_it)).get_gate());
			// If name is not empty we add a space before it
			aux_str = aux_gate->get_name();
			if (aux_str != "") {
				aux_str = " " + aux_str;
			}

			std::cout << "\t\t"
			<< aux_gate->get_type() << aux_str << " " << (*(*pins_net_it)).get_position() << std::endl;

		}

	}

	//Print components
	if (nlist.gates.size() > 0) {
		std::cout << "components "
			<< nlist.gates.size() << std::endl;
	}

	std::vector<net *> nets_net_vec;

	for (std::vector<gate *>::iterator gates_it = nlist.gates.begin()
		; gates_it != nlist.gates.end(); ++gates_it) {

		pins_net_vec = (*(*gates_it)).get_pins();

		aux_str = (*(*gates_it)).get_name();
		if (aux_str != "") {
			aux_str = " " + aux_str;
		}

		std::cout << "\tcomponent "
		<< (*(*gates_it)).get_type() << aux_str << " " << (*(*gates_it)).get_pin_num() << std::endl;

		for (std::vector<pin *>::iterator pins_net_it = pins_net_vec.begin();
			pins_net_it != pins_net_vec.end(); ++pins_net_it) {

			std::cout << "\t\tpin "
				<< (*(*pins_net_it)).get_width();

			nets_net_vec = (*(*pins_net_it)).get_nets();

			for (std::vector<net *>::iterator nets_it = nets_net_vec.begin()
				; nets_it != nets_net_vec.end(); ++nets_it) {
				std::cout << " " << (*(*nets_it)).get_name();
			}
			std::cout << std::endl;
		}
	}
#endif

	return 0;
}

int create_pin(int w, gate *g, netlist &nlist, int pos, std::map<std::string, net *> &nets_table,
	std::string name, int first_bit_in_bus) {
	pin *p = new pin(w, g, pos, first_bit_in_bus);
	nlist.pins.push_back(p);
	std::map<std::string, net *>::iterator net_it = nets_table.find(name);
	if (net_it == nets_table.end()) {
		std::cerr << "Error creating pin, net " << name << " does not exist" << std::endl;
		return -1;
	}
	(*p).add_net(net_it->second);
	(*(net_it->second)).add_pin(p);
	if ((*g).is_output_pin(pos)) {
		(*(net_it->second)).set_driver_pin(p);
		p->set_pin_type(OUT);
	} else {
		p->set_pin_type(IN);
	}
	(*g).add_pin(p);
	return 0;
}
