/***************************************************/
/*! \class Waves
\

Wprowadza trzy typy fal jako instrumenty

*/
/***************************************************/

#include <Mine/Waves.h>



	/***************************************************/
	/*! \klasy sinus,sawtooth,square
	klasy pochodne od klasy Instrmnt symuluj¹ce fale o ró¿nych kszta³tach
	*/
	/***************************************************/

Sinus::Sinus(stk::StkFloat lowestFrequency)
	{
		if (lowestFrequency <= 0.0) {
			oStream_ << "Sinus::Sinus: argument is less than or equal to zero!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}

		sine = new stk::SineWave;

		unsigned long nDelays = (unsigned long)(0.5 * stk::Stk::sampleRate() / lowestFrequency);
		delayLine_.setMaximumDelay(nDelays + 1);

		vibrato_.setFrequency(5.735);
		outputGain_ = 1.0;
		noiseGain_ = 0.2;
		vibratoGain_ = 0.1;

		this->setFrequency(220.0);
		this->clear();
	}

	Sinus :: ~Sinus(void)
	{
		delete sine;
	}

	void Sinus::clear(void)
	{
		delayLine_.clear();
		filter_.tick(0.0);
	}

	void Sinus::setFrequency(stk::StkFloat frequency)
	{
#if defined(_STK_DEBUG_)
		if (frequency <= 0.0) {
			oStream_ << "Sinus::setFrequency: argument is less than or equal to zero!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		// Account for filter delay and one sample "lastOut" delay.
		sine->setFrequency(frequency);
		stk::StkFloat delay = (Stk::sampleRate() / frequency) * 0.5 - filter_.phaseDelay(frequency) - 1.0;
		delayLine_.setDelay(delay);
	}

	void Sinus::startBlowing(stk::StkFloat amplitude, stk::StkFloat rate)
	{
		if (amplitude < 0.0 || rate <= 0.0) {
			oStream_ << "Sinus::startBlowing: one or more arguments is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(amplitude);
	}

	void Sinus::stopBlowing(stk::StkFloat rate)
	{
		if (rate < 0.0) {
			oStream_ << "Sinus::stopBlowing: argument is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(0.0);
	}

	void Sinus::noteOn(stk::StkFloat frequency, stk::StkFloat amplitude)
	{
		this->setFrequency(frequency);
		this->startBlowing(0.55 + (amplitude * 0.30), amplitude * 0.005);
		outputGain_ = amplitude + 0.001;
	}

	void Sinus::noteOff(stk::StkFloat amplitude)
	{
		this->stopBlowing(amplitude * 0.01);
	}

	void Sinus::controlChange(int number, stk::StkFloat value)
	{
#if defined(_STK_DEBUG_)
		if (Stk::inRange(value, 0.0, 128.0) == false) {
			oStream_ << "Sinus::controlChange: value (" << value << ") is out of range!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		stk::StkFloat normalizedValue = value * stk::ONE_OVER_128;
		if (number == __SK_ReedStiffness_) // 2
			reedTable_.setSlope(-0.44 + (0.26 * normalizedValue));
		else if (number == __SK_NoiseLevel_) // 4
			noiseGain_ = (normalizedValue * 0.4);
		else if (number == __SK_ModFrequency_) // 11
			vibrato_.setFrequency(normalizedValue * 12.0);
		else if (number == __SK_ModWheel_) // 1
			vibratoGain_ = (normalizedValue * 0.5);
		else if (number == __SK_AfterTouch_Cont_) // 128
			envelope_.setValue(normalizedValue);
#if defined(_STK_DEBUG_)
		else {
			oStream_ << "Sinus::controlChange: undefined control number (" << number << ")!";
			handleError(stk::StkError::WARNING);
		}
#endif
	}

	inline stk::StkFloat Sinus::tick(unsigned int)
	{
		lastFrame_[0] = sine->tick();

		// Apply output gain.
		lastFrame_[0] *= outputGain_;

		return lastFrame_[0];
	}

	inline stk::StkFrames& Sinus::tick(stk::StkFrames& frames, unsigned int channel)
	{
		unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
		if (channel > frames.channels() - nChannels) {
			oStream_ << "Sinus::tick(): channel and StkFrames arguments are incompatible!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}
#endif

		stk::StkFloat *samples = &frames[channel];
		unsigned int j, hop = frames.channels() - nChannels;
		if (nChannels == 1) {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop)
				*samples++ = tick();
		}
		else {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop) {
				*samples++ = tick();
				for (j = 1; j<nChannels; j++)
					*samples++ = lastFrame_[j];
			}
		}

		return frames;
	}

	////////////////////////////////////////////////METODY KLASY SQUARE////////////////////////////////////////////////

	Square::Square(stk::StkFloat lowestFrequency)
	{
		if (lowestFrequency <= 0.0) {
			oStream_ << "Square::Square: argument is less than or equal to zero!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}

		square = new stk::BlitSquare;

		unsigned long nDelays = (unsigned long)(0.5 * stk::Stk::sampleRate() / lowestFrequency);
		delayLine_.setMaximumDelay(nDelays + 1);

		vibrato_.setFrequency(5.735);
		outputGain_ = 1.0;
		noiseGain_ = 0.2;
		vibratoGain_ = 0.1;

		this->setFrequency(220.0);
		this->clear();
	}

	Square :: ~Square(void)
	{
		delete square;
	}

	void Square::clear(void)
	{
		delayLine_.clear();
		filter_.tick(0.0);
	}

	void Square::setFrequency(stk::StkFloat frequency)
	{
#if defined(_STK_DEBUG_)
		if (frequency <= 0.0) {
			oStream_ << "Square::setFrequency: argument is less than or equal to zero!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		// Account for filter delay and one sample "lastOut" delay.
		square->setFrequency(frequency);
		stk::StkFloat delay = (Stk::sampleRate() / frequency) * 0.5 - filter_.phaseDelay(frequency) - 1.0;
		delayLine_.setDelay(delay);
	}

	void Square::startBlowing(stk::StkFloat amplitude, stk::StkFloat rate)
	{
		if (amplitude < 0.0 || rate <= 0.0) {
			oStream_ << "Square::startBlowing: one or more arguments is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(amplitude);
	}

	void Square::stopBlowing(stk::StkFloat rate)
	{
		if (rate < 0.0) {
			oStream_ << "Square::stopBlowing: argument is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(0.0);
	}

	void Square::noteOn(stk::StkFloat frequency, stk::StkFloat amplitude)
	{
		this->setFrequency(frequency);
		this->startBlowing(0.55 + (amplitude * 0.30), amplitude * 0.005);
		outputGain_ = amplitude + 0.001;
	}

	void Square::noteOff(stk::StkFloat amplitude)
	{
		this->stopBlowing(amplitude * 0.01);
	}

	void Square::controlChange(int number, stk::StkFloat value)
	{
#if defined(_STK_DEBUG_)
		if (Stk::inRange(value, 0.0, 128.0) == false) {
			oStream_ << "Square::controlChange: value (" << value << ") is out of range!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		stk::StkFloat normalizedValue = value * stk::ONE_OVER_128;
		if (number == __SK_ReedStiffness_) // 2
			reedTable_.setSlope(-0.44 + (0.26 * normalizedValue));
		else if (number == __SK_NoiseLevel_) // 4
			noiseGain_ = (normalizedValue * 0.4);
		else if (number == __SK_ModFrequency_) // 11
			vibrato_.setFrequency(normalizedValue * 12.0);
		else if (number == __SK_ModWheel_) // 1
			vibratoGain_ = (normalizedValue * 0.5);
		else if (number == __SK_AfterTouch_Cont_) // 128
			envelope_.setValue(normalizedValue);
#if defined(_STK_DEBUG_)
		else {
			oStream_ << "Square::controlChange: undefined control number (" << number << ")!";
			handleError(stk::StkError::WARNING);
		}
#endif
	}

	inline stk::StkFloat Square::tick(unsigned int)
	{
		lastFrame_[0] = square->tick();

		// Apply output gain.
		lastFrame_[0] *= outputGain_;

		return lastFrame_[0];
	}

	inline stk::StkFrames& Square::tick(stk::StkFrames& frames, unsigned int channel)
	{
		unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
		if (channel > frames.channels() - nChannels) {
			oStream_ << "Square::tick(): channel and StkFrames arguments are incompatible!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}
#endif

		stk::StkFloat *samples = &frames[channel];
		unsigned int j, hop = frames.channels() - nChannels;
		if (nChannels == 1) {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop)
				*samples++ = tick();
		}
		else {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop) {
				*samples++ = tick();
				for (j = 1; j<nChannels; j++)
					*samples++ = lastFrame_[j];
			}
		}

		return frames;
	}


	////////////////////////////////////////////////METODY KLASY SAWTOOTH////////////////////////////////////////////////

	Sawtooth::Sawtooth(stk::StkFloat lowestFrequency)
	{
		if (lowestFrequency <= 0.0) {
			oStream_ << "Sawtooth::Sawtooth: argument is less than or equal to zero!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}

		saw = new stk::BlitSaw;

		unsigned long nDelays = (unsigned long)(0.5 * stk::Stk::sampleRate() / lowestFrequency);
		delayLine_.setMaximumDelay(nDelays + 1);

		vibrato_.setFrequency(5.735);
		outputGain_ = 1.0;
		noiseGain_ = 0.2;
		vibratoGain_ = 0.1;

		this->setFrequency(220.0);
		this->clear();
	}

	Sawtooth :: ~Sawtooth(void)
	{
		delete saw;
	}

	void Sawtooth::clear(void)
	{
		delayLine_.clear();
		filter_.tick(0.0);
	}

	void Sawtooth::setFrequency(stk::StkFloat frequency)
	{
#if defined(_STK_DEBUG_)
		if (frequency <= 0.0) {
			oStream_ << "Sawtooth::setFrequency: argument is less than or equal to zero!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		// Account for filter delay and one sample "lastOut" delay.
		saw->setFrequency(frequency);
		stk::StkFloat delay = (Stk::sampleRate() / frequency) * 0.5 - filter_.phaseDelay(frequency) - 1.0;
		delayLine_.setDelay(delay);
	}

	void Sawtooth::startBlowing(stk::StkFloat amplitude, stk::StkFloat rate)
	{
		if (amplitude < 0.0 || rate <= 0.0) {
			oStream_ << "Sawtooth::startBlowing: one or more arguments is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(amplitude);
	}

	void Sawtooth::stopBlowing(stk::StkFloat rate)
	{
		if (rate < 0.0) {
			oStream_ << "Sawtooth::stopBlowing: argument is less than zero!";
			handleError(stk::StkError::WARNING); return;
		}

		envelope_.setRate(rate);
		envelope_.setTarget(0.0);
	}

	void Sawtooth::noteOn(stk::StkFloat frequency, stk::StkFloat amplitude)
	{
		this->setFrequency(frequency);
		this->startBlowing(0.55 + (amplitude * 0.30), amplitude * 0.005);
		outputGain_ = amplitude + 0.001;
	}

	void Sawtooth::noteOff(stk::StkFloat amplitude)
	{
		this->stopBlowing(amplitude * 0.01);
	}

	void Sawtooth::controlChange(int number, stk::StkFloat value)
	{
#if defined(_STK_DEBUG_)
		if (Stk::inRange(value, 0.0, 128.0) == false) {
			oStream_ << "Sawtooth::controlChange: value (" << value << ") is out of range!";
			handleError(stk::StkError::WARNING); return;
		}
#endif

		stk::StkFloat normalizedValue = value * stk::ONE_OVER_128;
		if (number == __SK_ReedStiffness_) // 2
			reedTable_.setSlope(-0.44 + (0.26 * normalizedValue));
		else if (number == __SK_NoiseLevel_) // 4
			noiseGain_ = (normalizedValue * 0.4);
		else if (number == __SK_ModFrequency_) // 11
			vibrato_.setFrequency(normalizedValue * 12.0);
		else if (number == __SK_ModWheel_) // 1
			vibratoGain_ = (normalizedValue * 0.5);
		else if (number == __SK_AfterTouch_Cont_) // 128
			envelope_.setValue(normalizedValue);
#if defined(_STK_DEBUG_)
		else {
			oStream_ << "Sawtooth::controlChange: undefined control number (" << number << ")!";
			handleError(stk::StkError::WARNING);
		}
#endif
	}

	inline stk::StkFloat Sawtooth::tick(unsigned int)
	{
		lastFrame_[0] = saw->tick();

		// Apply output gain.
		lastFrame_[0] *= outputGain_;

		return lastFrame_[0];
	}

	inline stk::StkFrames& Sawtooth::tick(stk::StkFrames& frames, unsigned int channel)
	{
		unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
		if (channel > frames.channels() - nChannels) {
			oStream_ << "Sawtooth::tick(): channel and StkFrames arguments are incompatible!";
			handleError(stk::StkError::FUNCTION_ARGUMENT);
		}
#endif

		stk::StkFloat *samples = &frames[channel];
		unsigned int j, hop = frames.channels() - nChannels;
		if (nChannels == 1) {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop)
				*samples++ = tick();
		}
		else {
			for (unsigned int i = 0; i<frames.frames(); i++, samples += hop) {
				*samples++ = tick();
				for (j = 1; j<nChannels; j++)
					*samples++ = lastFrame_[j];
			}
		}

		return frames;
	}