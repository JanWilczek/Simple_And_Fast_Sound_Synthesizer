#ifndef SYNTH_APP_H
#define SYNTH_APP_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <RTMidi/RtMidi.h>
#include <STK/Stk.h>
#include <STK/SineWave.h>
#include <STK/WvIn.h>
#include <STK/WvOut.h>
#include <STK/RtWvIn.h>
#include <STK/RtWvOut.h>
#include <STK/Skini.h>
#include <STK/SKINImsg.h>
#include <STK/Generator.h>
#include <STK/Messager.h>
#include <STK/Voicer.h>
#include <STK/Thread.h>
#include <STK/Socket.h>
#include <STK/TcpServer.h>
#include <STK/Instrmnt.h>
#include <STK/Clarinet.h>
#include <STK/Delay.h>
#include <STK/BiQuad.h>
#include <STK/DelayL.h>
#include <STK/OneZero.h>
#include <STK/OnePole.h>
#include <STK/Envelope.h>
#include <STK/Noise.h>
#include <STK/JCRev.h>
#include <STK/Chorus.h>
#include <STK/Echo.h>
#include <STK/ADSR.h>
#include <Mine/Waves.h>
#include <algorithm>
#include <signal.h>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>
#include <sstream>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


//Tworzymy klasê bêd¹c¹ syntezatorem
class Synth
{
private:
	//Stuktura do informacji MIDI
	struct TickData {
		stk::WvOut **wvout;
		stk::Instrmnt **instrument;	//w oryginale by³y tu dwie gwiazdki
		stk::Voicer *voicer;
		stk::JCRev reverb;
		stk::Chorus chorus;
		stk::Echo echo;
		stk::Delay delay;
		stk::BiQuad resonance;
		stk::ADSR envelope;
		stk::Messager messager;
		stk::Skini::Message message;
		stk::StkFloat volume;
		stk::StkFloat t60;
		unsigned int nWvOuts;
		int nVoices;
		int currentVoice;
		int channels;
		int counter;
		bool realtime;
		bool settling;
		bool haveMessage;
		int frequency;

		// Default constructor.
		TickData()
			: wvout(0), instrument(0), voicer(0), volume(1.0), t60(0.01),
			nWvOuts(0), nVoices(1), currentVoice(0), channels(2), counter(0),
			realtime(true), settling(false), haveMessage(false) {}
	};

	//publiczny interfejs klasy
public:

	/// Konstruktor domyœlny ma za zadanie przygotowaæ u³o¿enie widgetów
	Synth();

	/// G³ówna metoda, która uruchamia syntezator
	void Run();

	// Metody pomocnicze
private:
	static int tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer);
	static void processMessage(TickData* data);
	void ToneSelect();

private:

	/// Obiekt SFGUI, potrzebny do inicjalizacji biblioteki
	sfg::SFGUI m_sfgui;

	/// G³ówne okno aplikacji
	std::unique_ptr<sf::RenderWindow> m_render_window;

	/// Pulpit naszej aplikacji
	sfg::Desktop m_desktop;

	//Okno aplikacji
	sfg::Window::Ptr window;

	//Slidery
	//Slidery Wave Envelope:
	sfg::Scale::Ptr attack_slider;
	sfg::Scale::Ptr decay_slider;
	sfg::Scale::Ptr sustain_slider;
	sfg::Scale::Ptr release_slider;
	
	//Slidery Filter:
	sfg::Scale::Ptr cutoff_slider;
	sfg::Scale::Ptr resonance_slider;
	sfg::Scale::Ptr delay_slider;

	//Slidery Effects:
	sfg::Scale::Ptr echo_slider;
	sfg::Scale::Ptr chorus_slider;
	sfg::Scale::Ptr reverb_slider;

	//Slider Volume:
	sfg::Scale::Ptr volume_slider;

	//Radio Buttony:
	sfg::RadioButton::Ptr sine_radio_button;
	sfg::RadioButton::Ptr square_radio_button;
	sfg::RadioButton::Ptr sawtooth_radio_button;
	
	//Labele:
	sfg::Label::Ptr wave_label;
	sfg::Label::Ptr volume_label;

	//Labele zwi¹zane z przebiegiem
	sfg::Label::Ptr envelope_label;
	sfg::Label::Ptr attack_label;
	sfg::Label::Ptr decay_label;
	sfg::Label::Ptr sustain_label;
	sfg::Label::Ptr release_label;

	//Labele zwi¹zane z filtrami
	sfg::Label::Ptr filter_label;
	sfg::Label::Ptr cutoff_label;
	sfg::Label::Ptr resonance_label;
	sfg::Label::Ptr delay_label;

	//Labele zwi¹zane z efektami
	sfg::Label::Ptr effects_label;
	sfg::Label::Ptr echo_label;
	sfg::Label::Ptr chorus_label;
	sfg::Label::Ptr reverb_label;
	
	//G³ówny plik do obs³ugi dŸwiêku
	TickData data;

	//Tworzymy wskaŸniki na trzy typy "instrumentów" czyli fal
	Sinus * sine;
	Square * square;
	Sawtooth * saw;

};

#endif SYNTH_APP_H