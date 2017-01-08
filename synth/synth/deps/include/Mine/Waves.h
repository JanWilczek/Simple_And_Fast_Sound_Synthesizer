#include <STK/Instrmnt.h>
#include <STK/DelayL.h>
#include <STK/ReedTable.h>
#include <STK/OneZero.h>
#include <STK/Envelope.h>
#include <STK/Noise.h>
#include <STK/SineWave.h>
#include <STK/BlitSquare.h>
#include <STK/BlitSaw.h>
#include <STK/SKINImsg.h>



	/***************************************************/
	/*! \klasy sinus,sawtooth,square
	klasy pochodne od klasy Instrmnt symuluj¹ce fale o ró¿nych kszta³tach
	*/
	/***************************************************/

	class Sinus : public stk::Instrmnt
	{
	public:
		//! Class constructor, taking the lowest desired playing frequency.
		/*!
		An StkError will be thrown if the rawwave path is incorrectly set.
		*/
		Sinus(stk::StkFloat lowestFrequency = 8.0);

		//! Class destructor.
		~Sinus(void);

		//! Reset and clear all internal state.
		void clear(void);

		//! Set instrument parameters for a particular frequency.
		void setFrequency(stk::StkFloat frequency);

		//! Apply breath pressure to instrument with given amplitude and rate of increase.
		void startBlowing(stk::StkFloat amplitude, stk::StkFloat rate);

		//! Decrease breath pressure with given rate of decrease.
		void stopBlowing(stk::StkFloat rate);

		//! Start a note with the given frequency and amplitude.
		void noteOn(stk::StkFloat frequency, stk::StkFloat amplitude);

		//! Stop a note with the given amplitude (speed of decay).
		void noteOff(stk::StkFloat amplitude);

		//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
		void controlChange(int number, stk::StkFloat value);

		//! Compute and return one output sample.
		stk::StkFloat tick(unsigned int channel = 0);

		//! Fill a channel of the StkFrames object with computed outputs.
		/*!
		The \c channel argument must be less than the number of
		channels in the StkFrames argument (the first channel is specified
		by 0).  However, range checking is only performed if _STK_DEBUG_
		is defined during compilation, in which case an out-of-range value
		will trigger an StkError exception.
		*/
		stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);

	protected:

		stk::DelayL delayLine_;
		stk::ReedTable reedTable_;
		stk::OneZero filter_;
		stk::Envelope envelope_;
		stk::Noise noise_;
		stk::SineWave vibrato_;

		stk::StkFloat outputGain_;
		stk::StkFloat noiseGain_;
		stk::StkFloat vibratoGain_;

		stk::SineWave* sine;
	};


	////////////////////////KLASA SQUARE ODPOWIEDZIALNA ZA SINUS KWADRATOKSZTA£TNY//////////////////////////////////////////

	class Square : public stk::Instrmnt
	{
	public:
		//! Class constructor, taking the lowest desired playing frequency.
		/*!
		An StkError will be thrown if the rawwave path is incorrectly set.
		*/
		Square(stk::StkFloat lowestFrequency = 8.0);

		//! Class destructor.
		~Square(void);

		//! Reset and clear all internal state.
		void clear(void);

		//! Set instrument parameters for a particular frequency.
		void setFrequency(stk::StkFloat frequency);

		//! Apply breath pressure to instrument with given amplitude and rate of increase.
		void startBlowing(stk::StkFloat amplitude, stk::StkFloat rate);

		//! Decrease breath pressure with given rate of decrease.
		void stopBlowing(stk::StkFloat rate);

		//! Start a note with the given frequency and amplitude.
		void noteOn(stk::StkFloat frequency, stk::StkFloat amplitude);

		//! Stop a note with the given amplitude (speed of decay).
		void noteOff(stk::StkFloat amplitude);

		//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
		void controlChange(int number, stk::StkFloat value);

		//! Compute and return one output sample.
		stk::StkFloat tick(unsigned int channel = 0);

		//! Fill a channel of the StkFrames object with computed outputs.
		/*!
		The \c channel argument must be less than the number of
		channels in the StkFrames argument (the first channel is specified
		by 0).  However, range checking is only performed if _STK_DEBUG_
		is defined during compilation, in which case an out-of-range value
		will trigger an StkError exception.
		*/
		stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);

	protected:

		stk::DelayL delayLine_;
		stk::ReedTable reedTable_;
		stk::OneZero filter_;
		stk::Envelope envelope_;
		stk::Noise noise_;
		stk::SineWave vibrato_;

		stk::StkFloat outputGain_;
		stk::StkFloat noiseGain_;
		stk::StkFloat vibratoGain_;

		stk::BlitSquare * square;
	};

	
	////////////////////////KLASA SAWTOOTH ODPOWIEDZIALNA ZA SINUS PI£OKSZTA£TNY//////////////////////////////////////////

	class Sawtooth : public stk::Instrmnt
	{
	public:
		//! Class constructor, taking the lowest desired playing frequency.
		/*!
		An StkError will be thrown if the rawwave path is incorrectly set.
		*/
		Sawtooth(stk::StkFloat lowestFrequency = 8.0);

		//! Class destructor.
		~Sawtooth(void);

		//! Reset and clear all internal state.
		void clear(void);

		//! Set instrument parameters for a particular frequency.
		void setFrequency(stk::StkFloat frequency);

		//! Apply breath pressure to instrument with given amplitude and rate of increase.
		void startBlowing(stk::StkFloat amplitude, stk::StkFloat rate);

		//! Decrease breath pressure with given rate of decrease.
		void stopBlowing(stk::StkFloat rate);

		//! Start a note with the given frequency and amplitude.
		void noteOn(stk::StkFloat frequency, stk::StkFloat amplitude);

		//! Stop a note with the given amplitude (speed of decay).
		void noteOff(stk::StkFloat amplitude);

		//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
		void controlChange(int number, stk::StkFloat value);

		//! Compute and return one output sample.
		stk::StkFloat tick(unsigned int channel = 0);

		//! Fill a channel of the StkFrames object with computed outputs.
		/*!
		The \c channel argument must be less than the number of
		channels in the StkFrames argument (the first channel is specified
		by 0).  However, range checking is only performed if _STK_DEBUG_
		is defined during compilation, in which case an out-of-range value
		will trigger an StkError exception.
		*/
		stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);

	protected:

		stk::DelayL delayLine_;
		stk::ReedTable reedTable_;
		stk::OneZero filter_;
		stk::Envelope envelope_;
		stk::Noise noise_;
		stk::SineWave vibrato_;

		stk::StkFloat outputGain_;
		stk::StkFloat noiseGain_;
		stk::StkFloat vibratoGain_;

		stk::BlitSaw * saw;
	};

