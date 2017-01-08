#include "synth.h"

bool done;
static void finish(int ignore){ done = true; }
#define DELTA_CONTROL_TICKS 64 // default sample frames between control input checks



Synth::Synth() : sine{ new Sinus(10.0) }, square{ new Square(10.0) }, saw{ new Sawtooth(10.0) }
{
	// Inicjalizacja g³ównego 	m_render_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Simple and Fast Sound Synthesizer");

	//Tworzymy okno aplikacji
	window = sfg::Window::Create();
	window->SetTitle("Synthesizer");

	//Tworzymy slidery
	//Slidery Wave Envelope:

	attack_slider = sfg::Scale::Create(0,100,2,sfg::Scale::Orientation::HORIZONTAL);
	attack_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	attack_slider->GetAdjustment()->SetValue(20);

	decay_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	decay_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	decay_slider->GetAdjustment()->SetValue(30);

	sustain_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	sustain_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	sustain_slider->GetAdjustment()->SetValue(50);

	release_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	release_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	release_slider->GetAdjustment()->SetValue(25);

	//Slidery Filter:
	cutoff_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	cutoff_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	cutoff_slider->GetAdjustment()->SetValue(50);

	resonance_slider = sfg::Scale::Create(0, 99, 2, sfg::Scale::Orientation::HORIZONTAL);
	resonance_slider->SetRequisition(sf::Vector2f(99.0f, 0.0f));
	resonance_slider->GetAdjustment()->SetValue(0);

	delay_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	delay_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	delay_slider->GetAdjustment()->SetValue(50);


	//Slidery Effects:
	echo_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	echo_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	echo_slider->GetAdjustment()->SetValue(0);

	chorus_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	chorus_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	chorus_slider->GetAdjustment()->SetValue(0);

	reverb_slider = sfg::Scale::Create(0.01, 100, 2, sfg::Scale::Orientation::HORIZONTAL);
	reverb_slider->SetRequisition(sf::Vector2f(100.0f, 0.01f));
	reverb_slider->GetAdjustment()->SetValue(0.01f);

	//Slider Volume:
	volume_slider = sfg::Scale::Create(0, 100, 2, sfg::Scale::Orientation::VERTICAL);
	volume_slider->SetRequisition(sf::Vector2f(100.0f, 0.0f));
	volume_slider->GetAdjustment()->SetValue(50);


	//Tworzymy radio buttonsy:
	sine_radio_button =	sfg::RadioButton::Create("Sine");
	square_radio_button = sfg::RadioButton::Create("Square", sine_radio_button->GetGroup());
	sawtooth_radio_button = sfg::RadioButton::Create("Sawtooth", sine_radio_button->GetGroup());
	sine_radio_button->SetActive(true);

	sine_radio_button->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&Synth::ToneSelect, this));
	square_radio_button->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&Synth::ToneSelect, this));
	sawtooth_radio_button->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&Synth::ToneSelect, this));
	
	//Tworzymy labele:
	wave_label = sfg::Label::Create("Wave type");
	volume_label = sfg::Label::Create("Volume");

	//Labele zwi¹zane z przebiegiem
	envelope_label = sfg::Label::Create("Wave envelope");
	attack_label = sfg::Label::Create("Attack");
	decay_label = sfg::Label::Create("Decay");
	sustain_label = sfg::Label::Create("Sustain");
	release_label = sfg::Label::Create("Release");

	//Labele zwi¹zane z filtrami
	filter_label = sfg::Label::Create("Filter");
	cutoff_label = sfg::Label::Create("Cutoff");
	resonance_label = sfg::Label::Create("Resonance");
	delay_label = sfg::Label::Create("Delay");

	//Labele zwi¹zane z efektami
	effects_label = sfg::Label::Create("Effects");
	echo_label = sfg::Label::Create("Echo");
	chorus_label = sfg::Label::Create("Chorus");
	reverb_label = sfg::Label::Create("Reverb");

	//Tworzymy boxy
	auto main_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto column1_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto column2_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	
	//Box zwi¹zany z wyborem typu fali
	auto wave_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	//wave_box->Pack(wave_label);
	wave_box->Pack(wave_label);
	wave_box->Pack(sine_radio_button);
	wave_box->Pack(square_radio_button);
	wave_box->Pack(sawtooth_radio_button);
	
	//Boxy zwi¹zane z przebiegiem
	auto envelope_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto attack_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	attack_box->Pack(attack_label);
	attack_box->Pack(attack_slider);

	auto decay_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	decay_box->Pack(decay_label);
	decay_box->Pack(decay_slider);
	auto sustain_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	sustain_box->Pack(sustain_label);
	sustain_box->Pack(sustain_slider);
	auto release_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	release_box->Pack(release_label);
	release_box->Pack(release_slider);

	envelope_box->Pack(envelope_label);
	envelope_box->Pack(attack_box);
	envelope_box->Pack(decay_box);
	envelope_box->Pack(sustain_box);
	envelope_box->Pack(release_box);

	//Boxy zwi¹zane z filtrem
	auto filter_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto cutoff_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	cutoff_box->Pack(cutoff_label);
	cutoff_box->Pack(cutoff_slider);
	auto resonance_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	resonance_box->Pack(resonance_label);
	resonance_box->Pack(resonance_slider);
	auto delay_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	delay_box->Pack(delay_label);
	delay_box->Pack(delay_slider);

	filter_box->Pack(filter_label);
	filter_box->Pack(cutoff_box);
	filter_box->Pack(resonance_box);
	filter_box->Pack(delay_box);

	//Boxy zwi¹zane z efektami
	auto effects_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto echo_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	echo_box->Pack(echo_label);
	echo_box->Pack(echo_slider);
	auto chorus_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	chorus_box->Pack(chorus_label);
	chorus_box->Pack(chorus_slider);

	auto reverb_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	reverb_box->Pack(reverb_label);
	reverb_box->Pack(reverb_slider);

	effects_box->Pack(effects_label);
	effects_box->Pack(echo_box);
	effects_box->Pack(chorus_box);
	effects_box->Pack(reverb_box);

	//Box z regulacj¹ g³oœnoœci
	auto volume_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto volume_slider_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	volume_slider_box->Pack(volume_slider);
	volume_box->Pack(volume_slider_box);
	volume_box->Pack(volume_label);

	//Pakujemy 5 segmentów razem
	column1_box->Pack(wave_box);
	column1_box->Pack(effects_box);

	column2_box->Pack(envelope_box);
	column2_box->Pack(filter_box);

	main_box->Pack(column1_box);
	main_box->Pack(column2_box);
	main_box->Pack(volume_box);

	window->Add(main_box);

	m_desktop.Add(window);

	// Inicjalizacja biblioteki rysuj¹cej
	m_render_window->resetGLStates();

}

void Synth::ToneSelect() {
	//brzmienie
	if (sine_radio_button->IsActive()) {
		data.voicer->removeInstrument(data.instrument[0]);
		data.instrument[0] = sine;
		data.voicer->addInstrument(data.instrument[0]);
	}
	else if (square_radio_button->IsActive()) {
		data.voicer->removeInstrument(data.instrument[0]);
		data.instrument[0] = square;
		data.voicer->addInstrument(data.instrument[0]);
	}
	else if (sawtooth_radio_button->IsActive()) {
		data.voicer->removeInstrument(data.instrument[0]);
		data.instrument[0] = saw;
		data.voicer->addInstrument(data.instrument[0]);
	}

}

void Synth::processMessage(TickData* data)
{
	register stk::StkFloat value1 = data->message.floatValues[0];
	register stk::StkFloat value2 = data->message.floatValues[1];

	// If only one instrument, allow messages from all channels to control it.
	//int group = 1;
	//  if ( data->nVoices > 1 ) group = data->message.channel;

	switch (data->message.type) {

	case __SK_Exit_:
		if (data->settling == false) goto settle;
		done = true;
		return;

	case __SK_NoteOn_:
		if (value2 > 0.0) { // velocity > 0
			data->voicer->noteOn(value1, value2);
			break;
		}
		// else a note off, so continue to next case

	case __SK_NoteOff_:
		data->voicer->noteOff(value1, value2);
		break;

	case __SK_ControlChange_:
		if (value1 == 44.0)
			data->reverb.setEffectMix(value2  * stk::ONE_OVER_128);
		else if (value1 == 7.0)
			data->volume = value2 * stk::ONE_OVER_128;
		else if (value1 == 49.0)
			data->voicer->setFrequency(value2);
		else if (value1 == 50.0)
			data->voicer->controlChange(128, value2);
		else if (value1 == 51.0)
			data->frequency = data->message.intValues[1];
		else if (value1 == 52.0) {
			data->frequency += (data->message.intValues[1] << 7);
			// Convert to a fractional MIDI note value
			stk::StkFloat note = 12.0 * log(data->frequency / 220.0) / log(2.0) + 57.0;
			data->voicer->setFrequency(note);
		}
		else
			data->voicer->controlChange((int)value1, value2);
		break;

	case __SK_AfterTouch_:
		data->voicer->controlChange(128, value1);
		break;

	case __SK_PitchChange_:
		data->voicer->setFrequency(value1);
		break;

	case __SK_PitchBend_:
		data->voicer->pitchBend(value1);
		break;

	case __SK_Volume_:
		data->volume = value1 * stk::ONE_OVER_128;
		break;

	case __SK_ProgramChange_:
		if (data->currentVoice == (int)value1) break;

		// Two-stage program change process.
		if (data->settling == false) goto settle;

	} // end of switch

	data->haveMessage = false;
	return;

settle:
	// Exit and program change messages are preceeded with a short settling period.
	data->voicer->silence();
	data->counter = (int)(0.3 * data->t60 * stk::Stk::sampleRate());
	data->settling = true;
}

int Synth::tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,	double streamTime, RtAudioStreamStatus status, void *dataPointer)
{
	TickData *data = (TickData *)dataPointer;
	register stk::StkFloat sample, *samples = (stk::StkFloat *)outputBuffer;
	int counter, nTicks = (int)nBufferFrames;

	while (nTicks > 0 && !done) {

		if (!data->haveMessage) {
			data->messager.popMessage(data->message);
			if (data->message.type > 0) {
				data->counter = (long)(data->message.time * stk::Stk::sampleRate());
				data->haveMessage = true;
			}
			else
				data->counter = DELTA_CONTROL_TICKS;
		}

		counter = std::min(nTicks, data->counter);
		stk::StkFrames samples_wek(counter);
		data->counter -= counter;
		for (int i = 0; i<counter; i++) {
			sample = data->voicer->tick();
			//sample = data->echo.tick(sample);	//dodajemy echo - dzia³a, ale nie powala
			sample = data->reverb.tick(data->chorus.tick(sample));	//dodajemy efekty
			//sample = data->delay.tick(sample);	//dodajemy delay - jeœli nie s³ychaæ ró¿nicy, to po co przep³acaæ
			//sample = data->resonance.tick(sample);		// dodajemy rezonans - dzia³a, ale œcisza
			sample *= data->volume;			//zg³aœniamy
			//samples_wek = data->voicer->lastFrame();
			//samples_wek = data->envelope.tick(samples_wek);		//error, ADSR dzia³a na wektorach, ale nie umiem ich przekazaæ "do grania"
			//samples_wek = data->reverb.tick(data->chorus.tick(samples_wek));
			for (unsigned int j = 0; j<data->nWvOuts; j++) data->wvout[j]->tick(sample);
			if (data->realtime)
			for (int k = 0; k<data->channels; k++) *samples++ = sample;
			nTicks--;
		}
		if (nTicks == 0) break;

		// Process control messages.
		if (data->haveMessage) processMessage(data);
	}

	return 0;
}

void Synth::Run() {

	sf::Event event;
	sf::Clock clock;

	int i;

#if defined(__STK_REALTIME__)
	RtAudio dac;
#endif

	stk::Stk::setSampleRate(44100.0);
	stk::Stk::showWarnings(true);

	
	data.nWvOuts = 0;
	data.wvout = (stk::WvOut **)calloc(data.nWvOuts, sizeof(stk::WvOut *));


	////////////// BRZMIENIE STARTOWE ///////////////////////////////
	
	data.nVoices = 1;
	data.instrument = (stk::Instrmnt **)calloc(data.nVoices, sizeof(stk::Instrmnt *));
	data.currentVoice = 0;
	data.instrument[0] = sine;
	data.voicer = (stk::Voicer *) new stk::Voicer(0.0);
	data.voicer->addInstrument(data.instrument[0]);


	///////////////////////////////////////////////////////

	try {
		if (!data.messager.startMidiInput()) exit(0);
		data.realtime = true;
	}
	catch (stk::StkError &) {
		goto cleanup;
	}

	//dodatkowe parametry
	data.delay.setMaximumDelay(4000);
	data.delay.setGain(1.0);

	data.echo.setEffectMix(0.5);
	data.echo.setMaximumDelay(100000);
	data.chorus.setEffectMix(0.5);
	data.chorus.setModFrequency(5);
	data.reverb.setEffectMix(1.0);

#if defined(__STK_REALTIME__)
	if (data.realtime) {
		RtAudioFormat format = (sizeof(stk::StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
		RtAudio::StreamParameters parameters;
		parameters.deviceId = dac.getDefaultOutputDevice();
		parameters.nChannels = data.channels;
		unsigned int bufferFrames = stk::RT_BUFFER_SIZE;
		try {
			dac.openStream(&parameters, NULL, format, (unsigned int)stk::Stk::sampleRate(), &bufferFrames, &tick, (void *)&data);
		}
		catch (RtAudioError& error) {
			error.printMessage();
			goto cleanup;
		}
	}
#endif
	
	(void)signal(SIGINT, finish);	///funkcja, ktora mo¿e, ale nie musi siê przydaæ

#if defined(__STK_REALTIME__)
	if (data.realtime) {
		try {
			dac.startStream();
		}
		catch (RtAudioError &error) {
			error.printMessage();
			goto cleanup;
		}
	}
#endif

		while (m_render_window->isOpen()) {
			// Sprawdzamy czy nast¹pi³o zdarzenie, jeœli tak, to je obs³ugujemy
			while (m_render_window->pollEvent(event)) {
				m_desktop.HandleEvent(event);
				
				// Jeœli zakmniêto okno, to zakañczamy program
				if (event.type == sf::Event::Closed) {
					m_render_window->close();
				}
			}

			
			// Funkcja odœwie¿aj¹ca widgety na pulpicie
			m_desktop.Update(clock.restart().asSeconds());

			// najpierw czyœcimy okno
			m_render_window->clear();

			m_sfgui.Display(*m_render_window);
			m_render_window->display();

			data.volume = volume_slider->GetValue() / 100;	//g³oœnoœæ

			data.envelope.setAttackTime(attack_slider->GetValue()+1);		//ADSR
			data.envelope.setDecayTime(decay_slider->GetValue()+1);
			data.envelope.setSustainLevel(sustain_slider->GetValue());
			data.envelope.setReleaseTime(release_slider->GetValue());

			data.delay.setDelay(delay_slider->GetValue() * 40);	//filtry
			data.resonance.setResonance(440.0, resonance_slider->GetValue() / 100, true);


			data.echo.setDelay(echo_slider->GetValue()*100);		//efekty
			data.chorus.setModDepth(chorus_slider->GetValue()/1000);
			data.t60 = reverb_slider->GetValue() / 100;
			data.reverb.setT60(data.t60);

			#if defined(__STK_REALTIME__)
					if (data.realtime)
						// Periodically check "done" status.
						stk::Stk::sleep(10);
					else
			#endif
						// Call the "tick" function to process data.
						tick(NULL, NULL, 256, 0, 0, (void *)&data);

	}

#if defined(__STK_REALTIME__)
		if (data.realtime) {
			try {
				dac.closeStream();
			}
			catch (RtAudioError& error) {
				error.printMessage();
			}
		}
#endif

	// Zwalniamy pamiêæ
	cleanup:
		delete data.voicer;

		for (i = 0; i<data.nVoices; i++) delete data.instrument[i];
		free(data.instrument);

}