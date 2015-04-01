/*
 * signals.cpp
 *
 *  Created on: 25/03/2015
 *      Author: raster
 */

#include "signals.hh"

Signals::Signals() {

}

Signals::~Signals() {

	// Unsubscribe from all the emiters
	std::list<class Signals *>::iterator iter;
	for (iter = this->subscribed.begin(); iter != this->subscribed.end(); ++iter) {
		(*iter)->unregister_signal("",this);
	}
}


void Signals::send_signal(std::string signal_name) {

	std::map<string,class Signals *>::iterator iter;
	for(iter = this->signal_list.find(signal_name); iter != this->signal_list.end(); ++iter) {
		if (!((*iter).second->callback_receiver(signal_name,this))) {
			break;
		}
	}
}

void Signals::register_signal(std::string signal_name, class Signals *receiver) {

	std::map<string,class Signals *>::iterator iter;
	for(iter = this->signal_list.find(signal_name); iter != this->signal_list.end(); ++iter) {
		if ((*iter).second == receiver) {
			return; // this signal is already connected to the receiver
		}
	}
	this->signal_list.insert(std::pair<string,class Signals *>(signal_name,receiver));
}

void Signals::unregister_signal(std::string signal_name, class Signals *receiver) {

	std::map<string,class Signals *>::iterator iter;
	if (signal_name != "") {
		for(iter = this->signal_list.find(signal_name); iter != this->signal_list.end(); ++iter) {
			if ((*iter).second == receiver) {
				this->signal_list.erase(iter);
				return;
			}
		}
	} else {
		for(iter = this->signal_list.begin(); iter != this->signal_list.end(); ++iter) {
			if ((*iter).second == receiver) {
				this->signal_list.erase(iter);
				return;
			}
		}
	}
}

bool Signals::callback_receiver(std::string signal_name, class Signals *sender) {
	return true;
}
