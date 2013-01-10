#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


// Positional command-line parameters
typedef enum {
	ARG_NAME,
	ARG_PLAYBACK_FILE,
	ARG_RECORD_FILE,
	ARG_RECORD_SAMPLE_RATE,
	ARG_RECORD_SAMPLE_COUNT
} ARG_ENUM;


int MCICommand(char *command)
{
	char buffer[32];
	MCIERROR returnValue;
	
	//printf("mciSendString: \n\t\"%s\"\n", command);
	returnValue = mciSendString(command, buffer, sizeof(buffer), 0);
	//printf("\tReturn Code = %d\n", returnValue);
	//printf("\tReturn String = %s\n", buffer);

	return atoi(buffer);
}


int main(int argc, char* argv[])
{
	char command[128];
	char i;
	int rxSampleRate;
	int length;

	// Only preserve name of executable (not path)
	i = strlen(argv[ARG_NAME]);
	while(--i >= 0) {
		if(argv[ARG_NAME][i] == '\\' || argv[ARG_NAME][i] == '/') {
			++i;
			break;
		}
	}
	
	if(argc < 4) {
		printf("Usage: %s [transmit file] [receive file] [receive sample rate]\n", argv[ARG_NAME] + i);
		return 0;
	}

	rxSampleRate = atoi(argv[ARG_RECORD_SAMPLE_RATE]);

	// Setup playback
	sprintf(command, "open %s type waveaudio alias waveOut", argv[ARG_PLAYBACK_FILE]);
	MCICommand(command);
	MCICommand("set waveOut time format samples");
	length = MCICommand("status waveOut length");

	// Setup record
	MCICommand("open new type waveaudio alias waveIn wait");
	sprintf(command, "set waveIn samplespersec %d wait", rxSampleRate);
	MCICommand(command);
	MCICommand("set waveIn time format samples");
	MCICommand("set waveIn samplespersec 22050 wait");
	MCICommand("set waveIn alignment 2 wait");
	MCICommand("set waveIn bitspersample 16 wait");
	MCICommand("set waveIn channels 1 wait");
	MCICommand("set waveIn format tag pcm wait");
		
	// Begin recording and playback
	MCICommand("cue waveIn input wait");
	MCICommand("cue waveOut output wait");
	MCICommand("record waveIn");
	MCICommand("play waveOut");
		
	// Don't know why we need the magic factor of 4 here (but it does work)
	sprintf(command, "record waveIn to %d wait", 4 * length);
	MCICommand(command);
	MCICommand("stop waveOut");

	// Save recorded samples
	sprintf(command, "save waveIn %s wait", argv[ARG_RECORD_FILE]);
	MCICommand(command);
	MCICommand("close waveIn wait");
	MCICommand("close waveOut wait");

	return 0;
}
