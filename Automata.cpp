/*
 *  Copyright 2008 Alex Reynolds
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *  http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  Automata.cpp
 *  Automata
 */

#include "Automata.h"

#pragma mark -
#pragma mark Constructors

Automata::Automata(unsigned int rule, bool seedIsRandom, unsigned int seedStartingPosition, unsigned int generationLength) {
	cout << "Initializing automaton... (rule: " << rule << ", generationLength: " << generationLength << ", seedIsRandom: " << seedIsRandom << ", seedStartingPosition: " << seedStartingPosition << ")" << endl;
	try {
		_currentIndex = -1;
		_overallIndex = -1;
		setBinaryRuleFromInteger(rule);
		AutomataGeneration *_seed = new AutomataGeneration;
		_generations = new AutomataGenerations(kAutomataGenerations);
		if (seedIsRandom)
			randomizeSeed(*_seed);
		else
			initializeSeedAtPosition(*_seed, seedStartingPosition);
		validateGeneration(*_seed);
		appendGeneration(*_seed);
		delete _seed;	
	}
	catch (std::bad_alloc &exc) {
		cout << exc.what() << endl;
	}
	catch (...) {
		delete _generations;
	}
}

Automata::Automata(const Automata& ref, bool seedFromEndOfRef) {
	_currentIndex = ref._currentIndex;
	_overallIndex = ref._overallIndex;
	for (int i = 0; i < kNumberOfRules; ++i)
		_rule[i] = ref._rule[i];
	
	if (seedFromEndOfRef)
		_generations->at(0) = ref._generations->at(kAutomataGenerations-1);
	else
		for (int i = 0; i < kAutomataGenerations; ++i)
			_generations->push_back(ref._generations->at(i));
}

#pragma mark -
#pragma mark Operators

ostream& operator << (ostream &os, const Automata &ref) {
	AutomataGeneration *test = new AutomataGeneration;
	for (int i = 0; i < kAutomataGenerations; ++i) {
		ref.generationAtIndex(*test, i);
		os << i << " " << ref.stringFromGeneration(*test) << endl;
	}
	delete test;
	return os;
}

Automata& Automata::operator = (const Automata &ref) {
	_currentIndex = ref._currentIndex;
	_overallIndex = ref._overallIndex;
	_rule = ref._rule;
	*_generations = *(ref._generations);
	return *this;
}

#pragma mark -
#pragma mark Member functions

void Automata::setBinaryRuleFromInteger (unsigned int numericalCode) {
	int x, y; 		
	for (y = kNumberOfRules-1; y >= 0; y--) {
		x = numericalCode / (1 << y);
		numericalCode = numericalCode - x * (1 << y);
		_rule[y] = (x == 0 ? kOffPixel : kOnPixel);
	}
}

void Automata::getNextGeneration (AutomataGeneration &cg, AutomataGeneration &ng) {
	unsigned int output, neighborhood;
	for (int position = 0; position < cg.size(); ++position) {
		neighborhood = getNeighborhoodForCurrentGenerationAtPosition(cg, position);
		validateNeighborhood(neighborhood);
		output = getOutputForNeighborhood(neighborhood);
		validateOutput(output);
		ng[position] = (output == 0 ? kOffPixel : kOnPixel);
	}
}

unsigned int Automata::getOutputForNeighborhood (unsigned int neighborhood) {
	validateNeighborhood(neighborhood);
	unsigned int output = _rule[neighborhood];
	validateOutput(output);
	return output;
}

unsigned int Automata::getNeighborhoodForCurrentGenerationAtPosition (AutomataGeneration &cg, const unsigned int position) {
	unsigned int center = (cg[position] == kOffPixel ? kOffPixelInteger : kOnPixelInteger);
	unsigned int left;
	if (position == 0) {
		left = (cg[cg.size()-1] == kOffPixel ? kOffPixelInteger : kOnPixelInteger);
	} else {
		left = (cg[position-1] == kOffPixel ? kOffPixelInteger : kOnPixelInteger);
	}
	unsigned int right;
	if (position == cg.size()-1) {
		right = (cg[0] == kOffPixel ? kOffPixelInteger : kOnPixelInteger);
	} else {
		right = (cg[position+1] == kOffPixel ? kOffPixelInteger : kOnPixelInteger);
	}
	unsigned int neighborhood = (1 * right) + (2 * center) + (4 * left);
	validateNeighborhood(neighborhood);
	return neighborhood;
}

void Automata::iterateAutomata () {
	try {
		AutomataGeneration *current = new AutomataGeneration;
		AutomataGeneration *next = new AutomataGeneration;
		
		generationAtIndex(*current, _currentIndex);
		getNextGeneration(*current, *next);
		validateGeneration(*next);
		appendGeneration(*next);
		
		delete next;
		delete current;
	}
	catch (std::bad_alloc &exc) {
		cout << exc.what() << endl;
	}
}

void Automata::printBuffer () const {
	AutomataGeneration *test = new AutomataGeneration;
	for (int i = 0; i < kAutomataGenerations; ++i) {
		generationAtIndex(*test, i);
		cout << i << " " << stringFromGeneration(*test) << endl;
	}
	delete test;		
}

