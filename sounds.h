#ifndef SOUNDS_H
#define SOUNDS_H
#include "raylib.h"
#include <cmath>

// Generate a sine wave tone as a Raylib Wave
Wave GenTone(float freq, float duration, int sampleRate=44100) {
    int samples = (int)(sampleRate * duration);
    Wave w = {0};
    w.frameCount = samples;
    w.sampleRate = sampleRate;
    w.sampleSize = 16;
    w.channels = 1;
    w.data = RL_MALLOC(samples * 2);
    short *d = (short*)w.data;
    for (int i = 0; i < samples; i++) {
        float t = (float)i / sampleRate;
        float env = 1.0f;
        if (i < 2000) env = (float)i/2000.0f;
        if (i > samples-2000) env = (float)(samples-i)/2000.0f;
        d[i] = (short)(sinf(2.0f*PI*freq*t)*32000.0f*env);
    }
    return w;
}

// Key frequencies for digits 0-9
const float KFREQ[10]={220,262,294,330,370,415,466,523,587,659};

// Generate background music from keys 8,1,5,7 pattern
Wave GenBGMusic(float noteDur=0.5f, int repeats=30) {
    int keys[]={8,1,5,7};
    int sampleRate=44100;
    int noteLen=(int)(sampleRate*noteDur);
    int total=noteLen*4*repeats;
    Wave w={0};
    w.frameCount=total; w.sampleRate=sampleRate;
    w.sampleSize=16; w.channels=1;
    w.data=RL_MALLOC(total*2);
    short *d=(short*)w.data;
    for(int r=0;r<repeats;r++){
        for(int k=0;k<4;k++){
            float freq=KFREQ[keys[k]];
            int off=(r*4+k)*noteLen;
            for(int i=0;i<noteLen&&(off+i)<total;i++){
                float t=(float)i/sampleRate;
                float env=1.0f;
                if(i<1000) env=(float)i/1000.0f;
                if(i>noteLen-1000) env=(float)(noteLen-i)/1000.0f;
                d[off+i]=(short)(sinf(2*PI*freq*t)*20000*env);
            }
        }
    }
    return w;
}

// Generate glass shattering noise
Wave GenGlassSound(float dur=2.0f) {
    int sr=44100; int n=(int)(sr*dur);
    Wave w={0}; w.frameCount=n; w.sampleRate=sr;
    w.sampleSize=16; w.channels=1;
    w.data=RL_MALLOC(n*2);
    short *d=(short*)w.data;
    for(int i=0;i<n;i++){
        float env=1.0f-(float)i/n;
        d[i]=(short)((rand()%32000-16000)*env*0.7f);
    }
    return w;
}
#endif
