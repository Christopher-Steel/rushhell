#ifndef	MACHINE_H
# define MACHINE_H

# ifndef STATE_MAX
#  define STATE_MAX 8
# endif
# ifndef EDGE_MAX
#  define EDGE_MAX 8
# endif

enum	eState
  {
    S0,
    s0 = S0,
    S1,
    s1 = S1,
    S2,
    s2 = S2,
    S3,
    s3 = S3,
    S4,
    s4 = S4,
    S5,
    s5 = S5,
    S6,
    s6 = S6,
    S7,
    s7 = S7,
    STATE_ERROR
  };

enum	eAction
  {
    MA,
    HR,
    ACTION_ERROR
  };

extern eState	gStateTable[STATE_MAX][EDGE_MAX];
extern eAction	gActionTable[STATE_MAX][EDGE_MAX];

#endif /* MACHINE_H */
