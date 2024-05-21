#ifndef FSM_H
#define FSM_H

enum Stato{ EMPTY, SX, DX, OCCUPIED};

struct Fsm{
  Stato stato;

};



static int actionVector[5];
static int saveIndex = 1;
static int ripetizione = 0;
static int numOfElementsInActionVector = sizeof(actionVector)/sizeof(int);
static bool giaCalcolato = true;



static bool notified = false;

static int a, b;



extern void resetActionVector();
extern void finishAction(int soglia);
//extern Stato getFsmStato(Fsm* fsm);
extern void updateLettura();
extern void resetActionVector();
extern void finishAction(int soglia, Stato tmp);
extern Stato getFsmStato(Fsm* fsm);









#endif