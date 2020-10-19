#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * DONE: Delete and use your own features!
 */

Features compute_features(const float *x, int N, float rate) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  Features feat;
  feat.p = compute_power(x,N);
  feat.am = compute_am(x,N);
  feat.zcr = compute_zcr(x,N, rate); /* Implementar després si cal */
  return feat;
}

/* 
 * DONE: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->lastState = ST_INIT;
  vad_data->alfa0 = 3.55;
  vad_data->alfa1 = 6.53;
  vad_data->tS = 5;
  vad_data->tV = 16;
  vad_data->timer = 0;
  vad_data->k0 = 0;
  vad_data->nInit = 11;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;

  state = ST_SILENCE;
  /* Això no millora el resultat
  if (vad_data->lastState == ST_SILENCE){
    state = ST_SILENCE;
  }
  if (vad_data->lastState == ST_VOICE){
    state = ST_VOICE;
  }
  */

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * DONE: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * DONE: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length, vad_data->sampling_rate);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

  switch (vad_data->state) {
  case ST_INIT:
    vad_data->k0 = vad_data->k0 + pow(10,compute_power(x,vad_data->frame_length)/10);
    vad_data->timer++;
    if(vad_data->timer == vad_data->nInit -1){
      vad_data->k0 = 10*log10((vad_data->k0)/(vad_data->nInit));
      vad_data->timer = 0;
      vad_data->state = ST_SILENCE;
    }
        
    break;

  case ST_SILENCE:
    vad_data->lastState = ST_SILENCE;
    if (f.p > (vad_data->k0 + vad_data->alfa0)){
      vad_data->state = ST_MAYBE_VOI;
      vad_data->timer = 0;
    }
    break;

  case ST_VOICE:
    vad_data->lastState = ST_VOICE;
    if (f.p < (vad_data->k0 + vad_data->alfa0 )){
      vad_data->state = ST_MAYBE_SIL;
      vad_data->timer = 0;
    }
    break;
  
  case ST_MAYBE_VOI:
    vad_data->timer++;
    /*if (f.p < (vad_data->k0 + vad_data->alfa0 ))
      vad_data->state = ST_SILENCE; */
    if ((f.p > (vad_data->k0 + vad_data->alfa0 + vad_data->alfa1)) && (vad_data->timer <= vad_data->tV))
      vad_data->state = ST_VOICE;
    if (vad_data->timer > vad_data->tV)
      vad_data->state = ST_SILENCE;
  break;

  case ST_MAYBE_SIL:
  /*
  vad_data->timer++;
    if ((f.p < (vad_data->k0 + vad_data->alfa0)) && (vad_data->timer <= vad_data->tS))
      vad_data->state = ST_SILENCE;
    if (vad_data->timer > vad_data->tS)
      vad_data->state = ST_VOICE; */
  
    vad_data->timer++;
    if (f.p > (vad_data->k0 + vad_data->alfa0))
      vad_data->state = ST_VOICE;
    if ((f.p < (vad_data->k0 + vad_data->alfa0)) && (vad_data->timer >= vad_data->tS))
      vad_data->state = ST_SILENCE;
  
  break;

  }
  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE)
    return vad_data->state;
  else
    return ST_MAYBE_VOI;
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
