#include<stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#define SAMPLE_RATE 58000
#define RECORD_SECONDS 5
#define NUM_CHANNELS 1
#define SAMPLE_FORMAT paFloat32


recordSound(float *samples,int numFrames){
    PaStream *stream;
    PaError err;
    err=Pa_Initialize();
    if(err!=paNoError){
        fprintf(stderr,"Port init failed %s\n",Pa_GetErrorText(err));
        exit(1);
    }
    
    err = Pa_OpenDefaultStream(&stream,1,0,SAMPLE_FORMAT,SAMPLE_RATE,numFrames,NULL,NULL);
    if(err != paNoError){
        fprintf(stderr,"Port audio stream open failed%s\n",Pa_GetErrorText(err));
        Pa_Terminate();
        exit(1);
    }
    
    err=Pa_StartStream(stream);
    if(err!=paNoError){
                fprintf(stderr,"Port audio stream start failed%s\n",Pa_GetErrorText(err));
                Pa_closeStream(stream);
        Pa_Terminate();
        exit(1);
    }
    
    printf("recording\n");
    
    err=Pa_ReadStream(stream,samples,numFrames);
        if(err!=paNoError){
                fprintf(stderr,"Port audio read stream failed%s\n",Pa_GetErrorText(err));
    }
    
        err=Pa_StopStream(stream);
        if(err!=paNoError){
                fprintf(stderr,"Port audio stop stream failed%s\n",Pa_GetErrorText(err));
    }
    
            err=Pa_closeStream(stream);
        if(err!=paNoError){
                fprintf(stderr,"Port audio close stream failed%s\n",Pa_GetErrorText(err));
    }
    Pa_Terminate();
}

void plotGraph(float *samples,int numFrames){
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if(gnuplotPipe == NULL){
        fprintf(stderr,"Error opening gunplot pipe \n");
        exit(1);
    }
    fprintf(gnuplotPipe,"plot '-' with lines\n");
    
    for(int i=0;i<numFrames;i++){
        fprintf(gnuplotPipe,"%f/n",samples[i]);
    }
    fprintf(gnuplotPipe,"e\n");
    fclose(gnuplotPipe);
}

int main(){
    int numFrames = SAMPLE_RATE * RECORD_SECONDS;
    float *samples = (float *)malloc (numFrames * sizeof(float));
    
    if(samples == NULL){
        fprintf(stderr,"MEM allocation failed \n");
        return 1;
    }
    
    recordSound(samples,numFrames);
    
    plotGraph(samples,numFrames);
    free(samples);
    return 0;
}
