/*
 * spectrumtocent.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: pablo
 */

#include "spectrumtocent.h"
#include "essentiamath.h"

namespace essentia {
namespace standard {

const char* SpectrumToCent::name = "SpectrumToCent";
const char* SpectrumToCent::category = "Spectral";
const char* SpectrumToCent::description = DOC("This algorithm computes energy in triangular frequency bands of a spectrum. Each band is computed to have a constant wideness in the cent scale. For each band the power-spectrum (mag-squared) is summed.\n"
"\n"
"Parameter \"centBinResolution\" should be and integer greater than 1, otherwise an exception will be thrown. TriangularBands is only defined for spectrum, which size is greater than 1.\n");


void SpectrumToCent::configure() {

  _centBinRes= parameter("centBinResolution").toReal();
  _nBands= parameter("bands").toReal();
  _minFrequency = parameter("minimumFrequency").toReal();
  _sampleRate = parameter("sampleRate").toReal();

  if ( _minFrequency >= _sampleRate / 2 ) {
    throw EssentiaException("SpectrumToCent: 'minimumFrequency' parameter is out of the range (0 - fs/2)");
  }

  calculateFilterFrequencies();

  if ( _bandFrequencies.back() > _sampleRate / 2 ) {
    throw EssentiaException("SpectrumToCent: Attempted to create a band above the Nyquist cut-off frequency.");
  }

  _isLog = parameter("log").toBool();
  _triangularBands->configure("frequencyBands", _bandFrequencies, "log", _isLog, "sampleRate", _sampleRate);


}


void SpectrumToCent::compute() {
  const vector<Real>& spectrum = _spectrumInput.get();
  vector<Real>& bands = _bandsOutput.get();
  vector<Real>& freqs = _freqOutput.get();

  freqs.resize( _nBands );

  for (int i = 0; i<_nBands; ++i) {
    freqs[i]= _bandFrequencies[i+1];
  }

  if (spectrum.size() <= 1) {
    throw EssentiaException("SpectrumToCent: the size of the input spectrum is not greater than one");
  }

  _triangularBands->input("spectrum").set(spectrum);
  _triangularBands->output("bands").set(bands);
  _triangularBands->compute();



}


void SpectrumToCent::calculateFilterFrequencies() {
  //Real maxInCents = 1200 * log2( _maxFrequency / _minFrequency );
  //_nBands = int (ceil(maxInCents / ( _centBinRes )));

  _bandFrequencies.resize( _nBands + 2 );

  for (int i=-1; i<=_nBands ; ++i) {
    _bandFrequencies[i+1] = _minFrequency * pow( 2, _centBinRes * i / ( 1200.0 ) );
  }
}

} // namespace standard
} // namespace essentia
