/*
filename ="sobol.c",
version  ="0.1",
date     ="2005/07/15",
author   ="Wolfgang Putschögl",
address  ="Johann Radon Institute for Computational and Applied Mathematics (RICAM)
				Austrian Academy of Sciences
				Altenbergerstraße 69
				A-4040 Linz, Austria",
telephone = "+43 (0)732 2468 5248",
fax       = "+43 (0)732 2468 5412",
email    ="Wolfgang.Putschoegl@oeaw.ac.at",
web      ="www.ricam.oeaw.ac.at"
codetable="Western European, iso 8859-15",
*/


/* In the following we assume that you have the GNU Scientific Library (GSL)
 installed. You can download the GSL at
 http://www.gnu.org/software/gsl/
 The program was written for the GSL version 1.6, but may also work with
 other versions of the GSL.
*/

#include "mex.h"

#include <stdio.h>
#include <gsl/gsl_qrng.h>

/* if defined the correctness of input parameters is checked */
/* Uncomment for performance reasons only if you are completely sure that your input arguments are always correct */
#define PARAM_CHECK 

/* -------- BEGIN definitions from /qrng/sobol.c------------*/
/* I had to include the following lines from /qrng/sobol.c */
/* Maximum allowed space dimension */
#define SOBOL_MAX_DIMENSION 40
/* Bit count; assumes sizeof(int) >= 32-bit */
#define SOBOL_BIT_COUNT 30

typedef struct
{
	unsigned int  sequence_count;
	double        last_denominator_inv;
	int           last_numerator_vec[SOBOL_MAX_DIMENSION];
	int           v_direction[SOBOL_BIT_COUNT][SOBOL_MAX_DIMENSION];
} sobol_state_t;
/* -------- END definitions from /qrng/sobol.c------------*/

static int STATEinit=0;
static int STATEdim=0;

/* Variables to store the state of the generator */
static unsigned int  STATEsequence_count;
static double        STATElast_denominator_inv;
static int           STATElast_numerator_vec[SOBOL_MAX_DIMENSION];
static int           STATEv_direction[SOBOL_BIT_COUNT][SOBOL_MAX_DIMENSION];

/* Just compares two strings (strcmp didn't work) */
int stringcmp(char * const a, char * const b);
/* Save state of generator */
void saveState(const sobol_state_t * const s_state, const int d);
/* Restore state of generator */
void restoreState(sobol_state_t * const s_state);
/* Save state of generator to output */
void saveStateToOutput(double * const out);
/* Restore state of generator from input */
void restoreStateFromInput(const double * const in);

/* The gateway routine */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	char *initFlag,*statusFlag;
	int buflen,status;

	double *out,*in;
	int i,j;
	double d,n;
	gsl_qrng * q;

	const int dim=2+SOBOL_MAX_DIMENSION+SOBOL_MAX_DIMENSION*SOBOL_BIT_COUNT;
	
	/*  Check for proper number of arguments */
#ifdef PARAM_CHECK
	if (nrhs<1 || nrhs>3)
		mexErrMsgTxt("One, two or three inputs required.");
#endif

	/* Check whether the state has to be saved or restored */
	/* If the first argument is a string, then it is a command
	referring to the state of the generator (either save or load) */
	if (nrhs<=2 && mxIsChar(prhs[0]) && mxGetM(prhs[0]) == 1)
	{
		/* Get the length of the input string. */
		buflen=(mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;
		/* Allocate memory for input and output strings */
		statusFlag=mxCalloc(buflen, sizeof(char));
		/* Copy the string data from prhs[0] into a C string */
		status=mxGetString(prhs[0], statusFlag, buflen);
#ifdef PARAM_CHECK
		if (status!=0)
			mexWarnMsgTxt("Not enough space. String is truncated.");

		if (!stringcmp(statusFlag,"state"))
			mexErrMsgTxt("Wrong input. The first input argument should be either a scalar or the string 'state'");
#endif
		if(nrhs==1)
		{
			/* State should be output */
			if (STATEinit==0)
				mexErrMsgTxt("Generator not yet initialized.");
			/* Create an array of doubles */
			plhs[0]=mxCreateDoubleMatrix(1,dim,mxREAL);
			/* Create a C pointer to a copy of the output matrix */
			out=mxGetPr(plhs[0]);
			/* Save state of generator to output */
			saveStateToOutput(out);
		}
		else
		{
			/* Set state to input */
			/* Create a pointer to the input matrix in */
			in=mxGetPr(prhs[1]);
#ifdef PARAM_CHECK
			/* Get the dimensions of the matrix input in */
			if(mxGetM(prhs[1])!=1 || mxGetN(prhs[1])!=dim)
				mexErrMsgTxt("Wrong input. Wrong dimension for the state.");
#endif			
			/* Restore state of generator from input */
			restoreStateFromInput(in);
		}
		return;
	}

	/* n denotes the number of points to generate.
	If no argument is given it is set to 1 */
	if (nrhs>=2)
	{
#ifdef PARAM_CHECK
		/* Check to make sure the second input argument is a scalar */
		if (!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]) ||
						mxGetN(prhs[1])*mxGetM(prhs[1])!=1)
			mexErrMsgTxt("Input n must be a scalar.");
#endif
		n=mxGetScalar(prhs[1]);
#ifdef PARAM_CHECK
		/* Check to make sure the number of points to generate is greater than zero */
		if (n<1)
			mexErrMsgTxt("Number of points to generate must be greater than zero.");
#endif
	}
	else
	{
		n=1;
	}
	
	if (nrhs==3)
	{
#ifdef PARAM_CHECK
		/* Input must be a string */
		if (mxIsChar(prhs[2])!=1)
			mexErrMsgTxt("Input must be a string.");
		/* Input must be a row vector */
		if (mxGetM(prhs[2])!=1)
			mexErrMsgTxt("Input must be a row vector.");
#endif
		/* Get the length of the input string */
		buflen=(mxGetM(prhs[2]) * mxGetN(prhs[2])) + 1;
		/* Allocate memory for input and output strings */
		initFlag=mxCalloc(buflen, sizeof(char));
		/* Copy the string data from prhs[2] into a C string */
		status=mxGetString(prhs[2], initFlag, buflen);
#ifdef PARAM_CHECK
		if (status!=0)
			mexWarnMsgTxt("Not enough space. String is truncated.");
#endif
		if(stringcmp(initFlag,"init"))
			STATEinit=0; /* Sequence should be initialized */
	}

#ifdef PARAM_CHECK
	/* Check to make sure the first input argument is a scalar */
	if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) ||
			 mxGetN(prhs[0])*mxGetM(prhs[0])!=1)
		mexErrMsgTxt("Input d must be a scalar.");
#endif
	/* Get the scalar input d */
	d=mxGetScalar(prhs[0]);
	/* Check to make sure the dimension is greater than zero */
#ifdef PARAM_CHECK
	if (d<1)
		mexErrMsgTxt("Dimension must be greater than zero.");
#endif

	q=gsl_qrng_alloc (gsl_qrng_sobol, d);

	sobol_state_t * s_state=(sobol_state_t *) q->state;
	/* Restore state of generator only if a point of the same dimension is to be generated */
	if (STATEinit && STATEdim == d) /* Restore state of generator */
		restoreState(s_state);
	else
		STATEinit++;

	double v[SOBOL_MAX_DIMENSION];
	/* Set the output pointer to the output matrix */
	plhs[0]=mxCreateDoubleMatrix(d,n,mxREAL);
	/* Create a C pointer to a copy of the output matrix */
	out=mxGetPr(plhs[0]);
	
	for (j=0;j<n;j++)
	{
		/* Generate one point... */
		gsl_qrng_get (q, v); /* Call the C subroutine */
		/* ...save this point */
		for (i=0;i<d;i++)
			out[i+j*(int)d]=v[i];
	}
	saveState(s_state,d);
}

int stringcmp(char * const a, char * const b)
{
	int i=0;
	while (b[i]!='\0')
	{
		if (a[i]!=b[i])
			return 0;
		i++;
	}
	return 1;
}

/* Save state of generator */
void saveState(const sobol_state_t *const s_state, int const d)
{
	int i,j;
	STATEsequence_count=s_state->sequence_count;
	STATElast_denominator_inv=s_state->last_denominator_inv;
	for (i=0; i<SOBOL_MAX_DIMENSION;i++)
		STATElast_numerator_vec[i]=s_state->last_numerator_vec[i];
	for (i=0; i<SOBOL_BIT_COUNT;i++)
		for (j=0; j<SOBOL_MAX_DIMENSION;j++)
			STATEv_direction[i][j]=s_state->v_direction[i][j];
	STATEdim=d;
}

/* Restore state of generator */
void restoreState(sobol_state_t * const s_state)
{
	int i,j;
	s_state->sequence_count=STATEsequence_count;
	s_state->last_denominator_inv=STATElast_denominator_inv;
	for (i=0; i<SOBOL_MAX_DIMENSION;i++)
		s_state->last_numerator_vec[i]=STATElast_numerator_vec[i];
	for (i=0; i<SOBOL_BIT_COUNT;i++)
		for (j=0; j<SOBOL_MAX_DIMENSION;j++)
			s_state->v_direction[i][j]=STATEv_direction[i][j];
}

/* Save state of generator to output */
void saveStateToOutput(double * const out)
{
	int i,j;
	out[0]=STATEsequence_count;
	out[1]=STATElast_denominator_inv;
	for (i=0; i<SOBOL_MAX_DIMENSION; i++)
		out[i+2]=STATElast_numerator_vec[i];
	for (i=0; i<SOBOL_BIT_COUNT; i++)
		for (j=0; j<SOBOL_MAX_DIMENSION; j++)
			out[i+j+2+SOBOL_MAX_DIMENSION]=STATEv_direction[i][j];
}

/* Restore state of generator from input */
void restoreStateFromInput(const double * const in)
{
	int i,j;
	STATEsequence_count=in[0];
	STATElast_denominator_inv=in[1];
	for (i=0; i<SOBOL_MAX_DIMENSION; i++)
		STATElast_numerator_vec[i]=in[i+2];
	for (i=0; i<SOBOL_BIT_COUNT; i++)
		for (j=0; j<SOBOL_MAX_DIMENSION; j++)
			STATEv_direction[i][j]=in[i+j+2+SOBOL_MAX_DIMENSION];
}
