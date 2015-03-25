/*
 * signals.hh
 *
 *  Created on: 25/03/2015
 *      Author: raster
 */

#ifndef SRC_SIGNALS_HH_
#define SRC_SIGNALS_HH_

using namespace std;
#include <map>
#include <list>
#include <string>

class Signals {

private:
	/**
	 * Contains the list of signals, and the objects that want to receive them
	 */
	std::map<std::string,class Signals * > signal_list;
	/**
	 * Contains the list of objects to whom this object registered signals to receive.
	 * Useful when destoying the object, to unregister all the signals automagically.
	 */
	std::list<class Signals *> subscribed;

protected:
	void send_signal(std::string signal_name);

public:
	Signals();
	virtual ~Signals();
	/**
	 * The callback that receives the signals being emited
	 * @param signal_name The signal that has been emited
	 * @param sender The object that sent the signal
	 * @return TRUE to continue sending signals to other objects registered to it; FALSE to stop propagating the signal
	 */
	virtual bool callback_receiver(std::string signal_name, class Signals *sender);
	/**
	 * Registers itself to receive a signal from an object
	 * @param signal_name The signal to receive
	 * @param receiver The object who sends the signal
	 */
	void register_signal(std::string signal_name, class Signals *receiver);
	/**
	 * Unregisters itself from receiving a signal from an object
	 * @param signal_name The signal to receive. Empty string to unregister all signals in this object
	 * @param receiver The object who sends the signal
	 */
	void unregister_signal(std::string signal_name, class Signals *receiver);
};



#endif /* SRC_SIGNALS_HH_ */
