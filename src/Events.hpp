#pragma once


namespace CADERA_APP_NAMESPACE {


/// @brief Flags that can be toggled in a bitset to trigger events
	enum CaderaEventFlags {
		
		/// Triggers adding sketch to Cadera
		cadera_add_sketch,
		/// Will trigger deletion of the currently selected object
		cadera_delete,
		/// Number of flags to be used in the creation of the bitset 
		cadera_num_flags  
	};


class Events {

public:

    Events();

    ~Events();

    std::bitset<cadera_num_flags> flags;

};

}