#ifndef PLAY
#define PLAY

#include "constantes.h"

class player{
    protected:
        PaStreamParameters inputParameters;
        PaStreamParameters outputParameters;
        double sampleRate;
        unsigned long framesPerBuffer;
        PaStreamFlags streamFlags;

    public:
        PaStream *stream;

        player();
        
        player(double SampleRate, unsigned long FramesPerBuffer, PaStreamFlags StreamFlags){
            sampleRate = SampleRate;
            framesPerBuffer = FramesPerBuffer;
            streamFlags = StreamFlags;
        }

        void setCommonParameters(double SampleRate, unsigned long FramesPerBuffer, PaStreamFlags StreamFlags){
            sampleRate = SampleRate;
            framesPerBuffer = FramesPerBuffer;
            streamFlags = StreamFlags;
        }

        void setInputParameters(PaDeviceIndex device, int channelCount, PaSampleFormat sampleFormat){
            inputParameters.device = device;
            inputParameters.channelCount = channelCount;
            inputParameters.sampleFormat = sampleFormat;
            inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = NULL;
        }

        void setOutputParameters(PaDeviceIndex device, int channelCount, PaSampleFormat sampleFormat){
            outputParameters.device = device;
            outputParameters.channelCount = channelCount;
            outputParameters.sampleFormat = sampleFormat;
            outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowInputLatency;
            outputParameters.hostApiSpecificStreamInfo = NULL;
        }

        PaError openStreamInput(PaStreamCallback *streamCallback, void *userData){
            return Pa_OpenStream(
                        &stream,
                        &inputParameters,
                        NULL,
                        sampleRate,
                        framesPerBuffer,
                        streamFlags, 
                        streamCallback,
                        userData);
        }

        PaError openStreamOutput(PaStreamCallback *streamCallback, void *userData){
            return Pa_OpenStream(
                        &stream,
                        NULL,
                        &outputParameters,
                        sampleRate,
                        framesPerBuffer,
                        streamFlags, 
                        streamCallback,
                        userData);
        }

        PaError openStreamInputOutput(PaStreamCallback *streamCallback, void *userData){
            return Pa_OpenStream(
                        &stream,
                        &inputParameters,
                        &outputParameters,
                        sampleRate,
                        framesPerBuffer,
                        streamFlags, 
                        streamCallback,
                        userData);
        }

        PaError startStream(){
            return Pa_StartStream(stream);
        }

        PaError closeStream(){
            return Pa_CloseStream(stream);
        }
};

void playData(paTestData data){
    player play(SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff);
    PaError err;

    data.frameIndex = 0;

    Pa_Initialize();

    play.setOutputParameters(Pa_GetDefaultOutputDevice(), 1, PA_SAMPLE_TYPE);

    cout << endl << "=== Ecoute ===" << endl << flush;
    play.openStreamOutput(playCallback, &data);
    play.startStream();
    cout << "Attente de la fin de l'audio." << endl << flush;

    while( ( err = Pa_IsStreamActive( play.stream ) ) == 1 ) 
        Pa_Sleep(100);

    play.closeStream();
    cout << "Fini." << endl << flush;

    Pa_Terminate();
}

#endif