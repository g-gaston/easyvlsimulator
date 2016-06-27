#include "syn.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "lex.h"
#include "simlib.h"

enum wire_fsm_state{WIRE, WIRE_NAME, BUS, BUS_MSB,
	BUS_COLON, BUS_LSB, BUS_DONE, WIRES, DONE};

enum component_fsm_state{TYPE, NAME, PIN_NAME, PINS,
	BUS_P, BUS_P_MSB, PINS_DONE, BUS_P_COLON, BUS_P_LSB,
	BUS_P_DONE, DONE_C};

enum assign_fsm_state{LHS, LHS_NAME, LHS_BUS, LHS_MSB,
	LHS_COLON, LHS_LSB, LHS_DONE, RHS, RHS_NAME, RHS_BUS,
	RHS_MSB, RHS_COLON, RHS_LSB, RHS_DONE, DONE_A};

enum module_fsm_state{MODULE, TYPE_M, PORTS, PORT_TYPE, BUS_M,
	BUS_MSB_M, BUS_COLON_M, BUS_LSB_M, BUS_DONE_M,
	PORT_NAME, PORTS_DONE, DONE_M};

void miss_semicolon(int line) {
	std::cerr << "LINE " << line
			<< ": semicolon missed" << std::endl;
}

void wire_error(int line) {
	std::cerr << "LINE " << line
			<< ": error in wire definition" << std::endl;
}

void component_error(int line) {
	std::cerr << "LINE " << line
			<< ": error in component definition" << std::endl;
}

int syn_analysis(int argc, char *argv[], std::vector<Evl_module> &modules) {

	if (argc < 2) {
		std::cerr << "You should provide a file name." << std::endl;
		return -1;
	}


	std::vector<Evl_token> tokens;
	if(lex_analysis(argc, argv, tokens) != 0) {
		return -1;
	}

	std::string type;

	std::vector<Evl_token>::iterator it = tokens.begin();

	wire_fsm_state wire_st;
	component_fsm_state component_st;
	assign_fsm_state assign_st;
	module_fsm_state module_st;
	Evl_module module;
	Evl_wire wire;
	Evl_component component;
	Evl_pin pin;
	Evl_assign assign;
	Evl_port port;
	int pos = 0;

	for (; it != tokens.end(); ++it) {

		if ((*it).is_name() && (*it).get_value() == "module") {
			module_st = MODULE;
			module.clear();
			while(module_st != DONE_M) {
			    it++;
			    switch (module_st) {
			    	case MODULE:
			    		if((*it).is_name()) {
			    			module.set_type((*it).get_value());
			    			module_st = TYPE_M;
			    			continue;
			    		}
			    		break;
			    	case TYPE_M:
			    		if ((*it).is_single() && (*it).get_value() == ";")  {
			    			module_st = DONE_M;
			    			continue;
			    		} else if ((*it).is_single() && (*it).get_value() == "(") {
			    			module_st = PORTS;
			    			continue;
			    		}
			    		break;
			    	case PORTS:
			    		if ((*it).is_name()) {
			    			port.set_type((*it).get_value());
			    			port.set_width(1);
			    			module_st = PORT_TYPE;
			    		}
			    		break;
			    	case PORT_TYPE:
			    		if ((*it).is_single() && (*it).get_value() == "[") {
			    			module_st = BUS_M;
			    		} else if ((*it).is_name()) {
			    			port.set_name((*it).get_value());
			    			module.add_port(port);
			    			// wire.set_name(port.get_name());
			    			// wire.set_width(port.get_width());
			    			// module.add_wire(wire);
			    			module_st = PORT_NAME;
			    		}
			    		break;
			    	case BUS_M:
			    		if ((*it).is_number()) {
			    			port.set_width((*it).get_number() + 1);
			    			module_st = BUS_MSB_M;
			    			continue;
			    		}
			    		break;
			    	case BUS_MSB_M:
			    		if ((*it).get_value() == ":") {
			    			module_st = BUS_COLON_M;
			    			continue;
			    		}
			    		break;
			    	case BUS_COLON_M:
			    		if ((*it).get_value() == "0") {
							module_st = BUS_LSB_M;
							continue;
						}
			    		break;
			    	case BUS_LSB_M:
			    		if ((*it).get_value() == "]") {
							module_st = BUS_DONE_M;
							continue;
						}
			    		break;
			    	case BUS_DONE_M:
			    		if ((*it).is_name()) {
			    			port.set_name((*it).get_value());
			    			module.add_port(port);
			    			// wire.set_name(port.get_name());
			    			// wire.set_width(port.get_width());
			    			// module.add_wire(wire);
			    			module_st = PORT_NAME;
			    		}
			    		break;
			    	case PORT_NAME:
			    		if ((*it).get_value() == ",") {
							module_st = PORTS;
							continue;
						} else if ((*it).get_value() == ")") {
							module_st = PORTS_DONE;
							continue;
						}
			    		break;
			    	case PORTS_DONE:
			    		if ((*it).is_single() && (*it).get_value() == ";")  {
			    			module_st = DONE_M;
			    			continue;
			    		}
			    	case DONE_M:
			    		break;
			    	default:
			    		std::cerr << "Invalid module in wire FSM" << std::endl;
			    		return -1;
			    		break;
			    }
			}
		} else if ((*it).is_name() && (*it).get_value() == "endmodule") {
			modules.push_back(module);
		} else if ((*it).is_name() && (*it).get_value() == "wire") {
			// Wire FSM
			wire.set_width(1);
			wire_st = WIRE;
			while (wire_st != DONE) {
				it++;
				switch (wire_st) {
					case WIRE:
						if((*it).is_name()) {
							wire.set_name((*it).get_value());
							module.add_wire(wire);
							wire_st = WIRE_NAME;
							continue;
						} else if ((*it).is_single() && (*it).get_value() == "[") {
							// BUS
							wire_st = BUS;
							continue;
						}
						break;
					case WIRE_NAME:
						if ((*it).is_single() && (*it).get_value() == ";") {
							wire_st = DONE;
							continue;
						} else if ((*it).is_single() && (*it).get_value() == ",") {
							wire_st = WIRES;
							continue;
						}
						break;
					case BUS:
						if ((*it).is_number()) {
							wire.set_width((*it).get_number() + 1);
							wire_st = BUS_MSB;
							continue;
						}
						break;
					case BUS_MSB:
						if ((*it).get_value() == ":") {
							wire_st = BUS_COLON;
							continue;
						}
						break;
					case BUS_COLON:
						if ((*it).get_value() == "0") {
							wire_st = BUS_LSB;
							continue;
						}
						break;
					case BUS_LSB:
						if ((*it).get_value() == "]") {
							wire_st = BUS_DONE;
							continue;
						}
						break;
					case BUS_DONE:
						if((*it).is_name()) {
							wire.set_name((*it).get_value());
							module.add_wire(wire);
							wire_st = WIRE_NAME;
							continue;
						}
						break;
					case WIRES:
						if((*it).is_name()) {
							wire.set_name((*it).get_value());
							module.add_wire(wire);
							wire_st = WIRE_NAME;
							continue;
						}
						break;
					case DONE:
						// Should never reach this zone
						break;
					default:
						std::cerr << "Invalid state in wire FSM" << std::endl;
						return -1;
						break;
				}

				wire_error((*it).get_line());
				return -1;
			}

		} else if ((*it).is_name() && (*it).get_value() == "assign") {

			assign_st = LHS;

			while (assign_st != DONE_A) {
				it++;
				switch(assign_st) {
					case LHS:
						if((*it).is_name()) {
							assign.set_lhs_msb(-1);
							assign.set_lhs_lsb(-1);
							assign.set_lhs_name((*it).get_value());
							assign_st = LHS_NAME;
							continue;
						}
						break;
					case LHS_NAME:
						if ((*it).get_value() == "[") {
							assign_st = LHS_BUS;
							continue;
						} else if ((*it).get_value() == "=") {
							assign_st = RHS;
							continue;
						}
						break;
					case LHS_BUS:
						if ((*it).is_number()) {
							assign.set_lhs_msb((*it).get_number());
							assign_st = LHS_MSB;
							continue;
						}
						break;
					case LHS_MSB:
						if ((*it).get_value() == "]") {
							assign_st = LHS_DONE;
							continue;
						} else if ((*it).get_value() == ":") {
							assign_st = LHS_COLON;
							continue;
						}
						break;
					case LHS_COLON:
						if ((*it).is_number()) {
							assign.set_lhs_lsb((*it).get_number());
							assign_st = LHS_LSB;
							continue;
						}
						break;
					case LHS_LSB:
						if ((*it).get_value() == "]") {
							assign_st = LHS_DONE;
							continue;
						}
						break;
					case LHS_DONE:
						if ((*it).get_value() == "=") {
							assign_st = RHS;
							continue;
						}
						break;
					case RHS:
						if((*it).is_name()) {
							assign.set_rhs_msb(-1);
							assign.set_rhs_lsb(-1);
							assign.set_rhs_name((*it).get_value());
							assign_st = RHS_NAME;
							continue;
						}
						break;
					case RHS_NAME:
						if ((*it).get_value() == "[") {
							assign_st = RHS_BUS;
							continue;
						} else if ((*it).get_value() == ";") {
							assign_st = DONE_A;
							continue;
						}
						break;
					case RHS_BUS:
						if ((*it).is_number()) {
							assign.set_rhs_msb((*it).get_number());
							assign_st = RHS_MSB;
							continue;
						}
						break;
					case RHS_MSB:
						if ((*it).get_value() == "]") {
							assign_st = RHS_DONE;
							continue;
						} else if ((*it).get_value() == ":") {
							assign_st = RHS_COLON;
							continue;
						}
						break;
					case RHS_COLON:
						if ((*it).is_number()) {
							assign.set_rhs_lsb((*it).get_number());
							assign_st = RHS_LSB;
							continue;
						}
						break;
					case RHS_LSB:
						if ((*it).get_value() == "]") {
							assign_st = RHS_DONE;
							continue;
						}
						break;
					case RHS_DONE:
						if ((*it).get_value() == ";") {
							assign_st = DONE_A;
							continue;
						}
						break;
					case DONE_A:
						// Should never reach this zone
						break;
					default:
						std::cerr << "Invalid state in assign FSM" << std::endl;
						return -1;
						break;
				}
				return -1;
			}
			module.add_assign(assign);

		} else if ((*it).is_name()) {
			// Component
			component.set_type((*it).get_value());
			component.set_name("");
			component_st = TYPE;
			pos = 0;

			while (component_st != DONE_C) {
				it++;
				switch (component_st) {
					case TYPE:
						if ((*it).is_name()) {
							component.set_name((*it).get_value());
							component_st = NAME;
							continue;
						} else if ((*it).is_single() && (*it).get_value() == "(") {
							component_st = PINS;
							continue;
						}
						break;
					case NAME:
						if ((*it).get_value() == "(") {
							component_st = PINS;
							continue;
						}
						break;
					case PINS:
						if ((*it).is_name()) {
							pin.set_name((*it).get_value());
							pin.set_msb(-1);
							pin.set_lsb(-1);
							component_st = PIN_NAME;
							continue;
						}
						break;
					case PIN_NAME:
						if ((*it).get_value() == "[") {
							component_st = BUS_P;
							continue;
						} else if ((*it).get_value() == ",") {
							component.add_pin(pin);
							component_st = PINS;
							pos++;
							continue;
						} else if ((*it).get_value() == ")") {
							component.add_pin(pin);
							component_st = PINS_DONE;
							pos++;
							continue;
						}
						break;
					case BUS_P:
						if ((*it).is_number()) {
							pin.set_msb((*it).get_number());
							component_st = BUS_P_MSB;
							continue;
						}
						break;
					case BUS_P_MSB:
						if ((*it).get_value() == ":") {
							component_st = BUS_P_COLON;
							continue;
						} else  if ((*it).get_value() == "]") {
							component_st = BUS_P_DONE;
							continue;
						}
						break;
					case BUS_P_COLON:
						if ((*it).is_number()) {
							pin.set_lsb((*it).get_number());
							component_st = BUS_P_LSB;
							continue;
						}
						break;
					case BUS_P_LSB:
						if ((*it).get_value() == "]") {
							component_st = BUS_P_DONE;
							continue;
						}
						break;
					case BUS_P_DONE:
						if ((*it).get_value() == ",") {
							component.add_pin(pin);
							component_st = PINS;
							pos++;
							continue;
						} else if ((*it).get_value() == ")") {
							component.add_pin(pin);
							component_st = PINS_DONE;
							pos++;
							continue;
						}
						break;
					case PINS_DONE:
						if ((*it).get_value() == ";") {
							module.add_component(component);
							component_st = DONE_C;
							continue;
						}
						break;
					case DONE_C:
						// Should never reach this zone
						break;
					default:
						std::cerr << "Invalid state in component FSM" << std::endl;
						return -1;
						break;
				}
				component_error((*it).get_line());
				return -1;
			}
			component.clear();

		} else {
			std::cerr << "LINE " << (*it).get_line()
				<< ": invalid token" << std::endl;
			return -1;
		}

	}

	//Ended syntax analysis, now we print it out

	return 0;
}
