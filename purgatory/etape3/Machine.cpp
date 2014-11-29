#include "Machine.hpp"

eState gStateTable[STATE_MAX][EDGE_MAX] =
  {
    /*         M            E            C            H            A            N            T        OTHER */
    {         S1, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR,          S2, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR,          S2,          S3, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR,          S4, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR,          S5, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR,          S6, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR,          S7, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
  };

eAction gActionTable[STATE_MAX][EDGE_MAX] =
  {
    /*          M             E             C             H             A             N             T         OTHER */
    {          MA, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR,           MA, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR,           MA,           MA, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR, ACTION_ERROR, ACTION_ERROR,           MA, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR,           MA, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR,           MA, ACTION_ERROR, ACTION_ERROR},
    {ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR, ACTION_ERROR,           MA, ACTION_ERROR},
    {          HR,           HR,           HR,           HR,           HR,           HR,           HR,           HR}
  };
