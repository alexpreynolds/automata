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
 *  Automata.h
 *  Automata
 */

#pragma once

#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <bitset>

using namespace std;

static const unsigned int kNumberOfRules =							8;
static const unsigned int kAutomataGenerationLength =				160;
static const unsigned int kAutomataGenerations =					240;
static const unsigned int kDefaultRule =							30;
static const unsigned int kDefaultSeedStartingPosition =			kAutomataGenerationLength/2 - 1;
static const bool kDefaultSeedInitializerIsRandom =					false;
static const bool kDefaultShouldSeedFromEndOfRef =					false;
static const unsigned int kLowBoundOnNeighborhood =					0;
static const unsigned int kHighBoundOnNeighborhood =				7;
static const unsigned int kLowBoundOnOutput =						0;
static const unsigned int kHighBoundOnOutput =						1;
static const bool kOffPixel =										0;
static const bool kOnPixel =										1;
static const char kOffPixelChar =									'_';
static const char kOnPixelChar =									'+';
static const unsigned int kOffPixelInteger =						0;
static const unsigned int kOnPixelInteger =							1;
static const std::string kAutomataBadNeighborhoodBoundsException =	"Bad neighborhood bounds!";
static const std::string kAutomataBadOutputBoundsException =		"Bad output bounds!";
static const std::string kAutomataBadStringLengthException =		"Automata generation is too long!";

#pragma mark -
#pragma mark Typedefs

typedef std::bitset< kNumberOfRules >								AutomataRule;
typedef std::bitset< kAutomataGenerationLength >					AutomataGeneration;
typedef std::vector< AutomataGeneration >							AutomataGenerations;

#pragma mark -
#pragma mark Class

class Automata {		
public:
	// constructors
	Automata(unsigned int rule = kDefaultRule, bool seedIsRandom = kDefaultSeedInitializerIsRandom, unsigned int seedStartingPosition = kDefaultSeedStartingPosition, unsigned int generationLength = kAutomataGenerationLength);
	Automata(const Automata &ref, bool seedFromEndOfRef = kDefaultShouldSeedFromEndOfRef);
	
	// destructor
	~Automata() { cout << "Deleting automaton..." << endl; delete _generations; }
	
	// operators
	friend ostream& operator<< (ostream &os, const Automata &ref);
	Automata& operator= (const Automata &ref);
	
	void iterateAutomata ();
	void printBuffer () const;
	void fillBuffer ()																{ for (int counter = 0; counter < kAutomataGenerations-1; ++counter) iterateAutomata(); }
	int currentGenerationIndex () const												{ return _currentIndex; }
	void currentGeneration (AutomataGeneration &output) const						{ getGeneration(output, _currentIndex); }
	void generationAtIndex (AutomataGeneration &output, unsigned int index) const	{ getGeneration(output, index); }
	std::string stringFromGeneration (AutomataGeneration &g) const					{ std::string str(g.size(), kOffPixelChar); for (int i = 0; i < g.size(); ++i) { str[i] = (g[i] == kOffPixel ? kOffPixelChar : kOnPixelChar); } return str; }
	
private:
	int _currentIndex, _overallIndex;
	AutomataRule _rule;
	AutomataGenerations* _generations;
	
	// private methods
	void setBinaryRuleFromInteger (unsigned int numericalCode);
	void getNextGeneration (AutomataGeneration &cg, AutomataGeneration &ng);
	unsigned int getOutputForNeighborhood (const unsigned int neighborhood);
	unsigned int getNeighborhoodForCurrentGenerationAtPosition (AutomataGeneration &cg, const unsigned int position);
	
	// inline functions
	int	currentIndex () const														{ return _currentIndex; }
	int	overallIndex () const														{ return _overallIndex; }
	void getGeneration (AutomataGeneration &output, unsigned int index) const		{ output = _generations->at(index); }
	void appendGeneration (AutomataGeneration &g)									{ _overallIndex++; _currentIndex = (_currentIndex + 1) % kAutomataGenerations; _generations->at(_currentIndex) = g;	}
	void randomizeSeed (AutomataGeneration &seed)									{ for (int i = 0; i < kAutomataGenerationLength; ++i) seed[i] = random() % 2; }
	void initializeSeedAtPosition (AutomataGeneration &seed, unsigned int position)	{ seed.reset(); seed.set(position); }
	void validateNeighborhood (unsigned int neighborhood)							{ if ((neighborhood < kLowBoundOnNeighborhood) || (neighborhood > kHighBoundOnNeighborhood)) throw kAutomataBadNeighborhoodBoundsException; }
	void validateOutput (unsigned int output)										{ if ((output < kLowBoundOnOutput) || (output > kHighBoundOnOutput)) throw kAutomataBadOutputBoundsException; }
	void validateGeneration (AutomataGeneration &g)									{ if (g.size() > kAutomataGenerationLength) throw kAutomataBadStringLengthException; }
};

#endif