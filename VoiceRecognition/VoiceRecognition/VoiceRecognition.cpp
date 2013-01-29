// 
// Sample voice recognition code for PCSDK 
//
// by Ratko Jagodic
// 

#include <windows.h>
#include <iostream>
#include "conio.h"
#include "pxcvoice.h"
#include "util_pipeline.h"


// ---------- comment this out to command and control  ---------//
// you cannot use both at the same time unfortunately...
#define DICTATION 1;



using namespace std;

class CommandAndControl: public UtilPipeline {

private:
	std::vector<pxcCHAR *> commands;

public:

	CommandAndControl():UtilPipeline() 	{
		EnableVoiceRecognition();
	}

	void AddCommand(pxcCHAR* cmd)  {
		commands.push_back(cmd);
	}

	void SetGrammar()	{
		this->SetVoiceCommands(commands.size(), &commands[0]);
	}
	
	void PXCAPI OnRecognized(PXCVoiceRecognition::Recognition *data) {
		std::wcout << "\n<COMMAND RECOGNIZED> " << commands[data->label] << flush;
	}
};



class Dictation: public UtilPipeline {

public:

	Dictation():UtilPipeline() 	{
		EnableVoiceRecognition();
	}

	void PXCAPI OnRecognized(PXCVoiceRecognition::Recognition *data) {
		std::wcout << "\n<DICTATION RECOGNIZED> " << data->dictation << flush;
	}
};



int wmain(int argc, wchar_t* argv[]) {

#ifdef DICTATION
		Dictation reco;
#else
		CommandAndControl reco;
		reco.AddCommand(L"good morning"); 
		reco.AddCommand(L"show me the weather");
		reco.SetGrammar();
#endif

	reco.Init();
	std::cout << "\nInitialized. Speak....\n" << flush;
	
	while(!_kbhit())
	{
		reco.AcquireFrame(true);  // must have this for processing the audio
		reco.ReleaseFrame();	
		cout << ".";
	}
}