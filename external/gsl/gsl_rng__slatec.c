/* rng/slatec.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2007 James Theiler, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**

* ======================================================================
* NIST Guide to Available Math Software.
* Source for module RAND from package CMLIB.
* Retrieved from TIBER on Fri Oct 11 11:43:42 1996.
* ======================================================================
      FUNCTION RAND(R)
C***BEGIN PROLOGUE  RAND
C***DATE WRITTEN   770401   (YYMMDD)
C***REVISION DATE  820801   (YYMMDD)
C***CATEGORY NO.  L6A21
C***KEYWORDS  RANDOM NUMBER,SPECIAL FUNCTION,UNIFORM
C***AUTHOR  FULLERTON, W., (LANL)
C***PURPOSE  Generates a uniformly distributed random number.
C***DESCRIPTION
C
C      This pseudo-random number generator is portable among a wide
C variety of computers.  RAND(R) undoubtedly is not as good as many
C readily available installation dependent versions, and so this
C routine is not recommended for widespread usage.  Its redeeming
C feature is that the exact same random numbers (to within final round-
C off error) can be generated from machine to machine.  Thus, programs
C that make use of random numbers can be easily transported to and
C checked in a new environment.
C      The random numbers are generated by the linear congruential
C method described, e.g., by Knuth in Seminumerical Methods (p.9),
C Addison-Wesley, 1969.  Given the I-th number of a pseudo-random
C sequence, the I+1 -st number is generated from
C             X(I+1) = (A*X(I) + C) MOD M,
C where here M = 2**22 = 4194304, C = 1731 and several suitable values
C of the multiplier A are discussed below.  Both the multiplier A and
C random number X are represented in double precision as two 11-bit
C words.  The constants are chosen so that the period is the maximum
C possible, 4194304.
C      In order that the same numbers be generated from machine to
C machine, it is necessary that 23-bit integers be reducible modulo
C 2**11 exactly, that 23-bit integers be added exactly, and that 11-bit
C integers be multiplied exactly.  Furthermore, if the restart option
C is used (where R is between 0 and 1), then the product R*2**22 =
C R*4194304 should be correct to the nearest integer.
C      The first four random numbers should be .0004127026,
C .6750836372, .1614754200, and .9086198807.  The tenth random number
C is .5527787209, and the hundredth is .3600893021 .  The thousandth
C number should be .2176990509 .
C      In order to generate several effectively independent sequences
C with the same generator, it is necessary to know the random number
C for several widely spaced calls.  The I-th random number times 2**22,
C where I=K*P/8 and P is the period of the sequence (P = 2**22), is
C still of the form L*P/8.  In particular we find the I-th random
C number multiplied by 2**22 is given by
C I   =  0  1*P/8  2*P/8  3*P/8  4*P/8  5*P/8  6*P/8  7*P/8  8*P/8
C RAND=  0  5*P/8  2*P/8  7*P/8  4*P/8  1*P/8  6*P/8  3*P/8  0
C Thus the 4*P/8 = 2097152 random number is 2097152/2**22.
C      Several multipliers have been subjected to the spectral test
C (see Knuth, p. 82).  Four suitable multipliers roughly in order of
C goodness according to the spectral test are
C    3146757 = 1536*2048 + 1029 = 2**21 + 2**20 + 2**10 + 5
C    2098181 = 1024*2048 + 1029 = 2**21 + 2**10 + 5
C    3146245 = 1536*2048 +  517 = 2**21 + 2**20 + 2**9 + 5
C    2776669 = 1355*2048 + 1629 = 5**9 + 7**7 + 1
C
C      In the table below LOG10(NU(I)) gives roughly the number of
C random decimal digits in the random numbers considered I at a time.
C C is the primary measure of goodness.  In both cases bigger is better.
C
C                   LOG10 NU(I)              C(I)
C       A       I=2  I=3  I=4  I=5    I=2  I=3  I=4  I=5
C
C    3146757    3.3  2.0  1.6  1.3    3.1  1.3  4.6  2.6
C    2098181    3.3  2.0  1.6  1.2    3.2  1.3  4.6  1.7
C    3146245    3.3  2.2  1.5  1.1    3.2  4.2  1.1  0.4
C    2776669    3.3  2.1  1.6  1.3    2.5  2.0  1.9  2.6
C   Best
C    Possible   3.3  2.3  1.7  1.4    3.6  5.9  9.7  14.9
C
C             Input Argument --
C R      If R=0., the next random number of the sequence is generated.
C        If R .LT. 0., the last generated number will be returned for
C          possible use in a restart procedure.
C        If R .GT. 0., the sequence of random numbers will start with
C          the seed R mod 1.  This seed is also returned as the value of
C          RAND provided the arithmetic is done exactly.
C
C             Output Value --
C RAND   a pseudo-random number between 0. and 1.
C***REFERENCES  (NONE)
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  RAND
      DATA IA1, IA0, IA1MA0 /1536, 1029, 507/
      DATA IC /1731/
      DATA IX1, IX0 /0, 0/
C***FIRST EXECUTABLE STATEMENT  RAND
      IF (R.LT.0.) GO TO 10
      IF (R.GT.0.) GO TO 20
C
C           A*X = 2**22*IA1*IX1 + 2**11*(IA1*IX1 + (IA1-IA0)*(IX0-IX1)
C                   + IA0*IX0) + IA0*IX0
C
      IY0 = IA0*IX0
      IY1 = IA1*IX1 + IA1MA0*(IX0-IX1) + IY0
      IY0 = IY0 + IC
      IX0 = MOD (IY0, 2048)
      IY1 = IY1 + (IY0-IX0)/2048
      IX1 = MOD (IY1, 2048)
C
 10   RAND = IX1*2048 + IX0
      RAND = RAND / 4194304.
      RETURN
C
 20   IX1 = AMOD(R,1.)*4194304. + 0.5
      IX0 = MOD (IX1, 2048)
      IX1 = (IX1-IX0)/2048
      GO TO 10
C
      END

  **/

#include "gsl__config.h"
#include <stdlib.h>
#include "gsl_rng.h"

static inline unsigned long int slatec_get (void *vstate);
static double slatec_get_double (void *vstate);
static void slatec_set (void *state, unsigned long int s);

typedef struct
  {
    long int x0, x1;
  }
slatec_state_t;

static const long P = 4194304;
static const long a1 = 1536;
static const long a0 = 1029;
static const long a1ma0 = 507;
static const long c = 1731;

static inline unsigned long int
slatec_get (void *vstate)
{
  long y0, y1;
  slatec_state_t *state = (slatec_state_t *) vstate;

  y0 = a0 * state->x0;
  y1 = a1 * state->x1 + a1ma0 * (state->x0 - state->x1) + y0;
  y0 = y0 + c;
  state->x0 = y0 % 2048;
  y1 = y1 + (y0 - state->x0) / 2048;
  state->x1 = y1 % 2048;

  return state->x1 * 2048 + state->x0;
}

static double 
slatec_get_double (void *vstate)
{
  return slatec_get (vstate) / 4194304.0 ;
}

static void
slatec_set (void *vstate, unsigned long int s)
{
  slatec_state_t *state = (slatec_state_t *) vstate;

  /* Only eight seeds are permitted.  This is pretty limiting, but
     at least we are guaranteed that the eight sequences are different */

  s = s % 8;
  s *= P / 8;

  state->x0 = s % 2048;
  state->x1 = (s - state->x0) / 2048;
}

static const gsl_rng_type slatec_type =
{"slatec",                      /* name */
 4194303,                       /* RAND_MAX */
 0,                             /* RAND_MIN */
 sizeof (slatec_state_t),
 &slatec_set,
 &slatec_get,
 &slatec_get_double};

const gsl_rng_type *gsl_rng_slatec = &slatec_type;
