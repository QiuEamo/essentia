/*
 * spectrumtocent.h
 *
 *  Created on: Oct 26, 2016
 *      Author: pablo
 */

#ifndef ESSENTIA_SPECTRUMTOCENT_H
#define ESSENTIA_SPECTRUMTOCENT_H

#include "algorithm.h"
#include "essentiautil.h"
#include "algorithmfactory.h"

using namespace std;

namespace essentia {
namespace standard {

class SpectrumToCent : public Algorithm {

 protected:
  Input<std::vector<Real> > _spectrumInput;
  Output<std::vector<Real> > _bandsOutput;
  Output<std::vector<Real> > _freqOutput;

  std::vector<Real> _bandFrequencies;
  std::vector<Real> _freqBands;
  int _nBands;
  Real _sampleRate;
  bool _isLog;

  Real _minFrequency;
  //Real _maxFrequency;
  int _centBinRes;


  Algorithm* _triangularBands;

  void calculateFilterFrequencies();

 public:
  SpectrumToCent() {
    declareInput(_spectrumInput, "spectrum", "the input spectrum (must be greater than size one)");
    declareOutput(_bandsOutput, "bands", "the energy in each band");
    declareOutput(_freqOutput, "frequencies", "the central frequency band");
    _triangularBands = AlgorithmFactory::create("TriangularBands");
  }

  void declareParameters() {

    declareParameter("sampleRate", "the sampling rate of the audio signal [Hz]", "(0,inf)", 44100.);
    declareParameter("log", "compute log-energies (log10 (1 + energy))","{true,false}", true);
    declareParameter("minimumFrequency","central frequency of the first band of the bank [Hz]", "(0, inf)", 164.0 );
    declareParameter("centBinResolution", "number of cents in bins. default: 10 cents[REVIEW]","(0,inf)", 10);
    declareParameter("bands", "number of bins to compute. default: 720 (6 octaves)","[1,inf)", 720);
  }

  void compute();
  void configure();

  static const char* name;
  static const char* category;
  static const char* description;
};

} // namespace standard
} // namespace essentia

#endif /* SRC_ALGORITHMS_SPECTRAL_SPECTRUMTOCENT_H_ */
