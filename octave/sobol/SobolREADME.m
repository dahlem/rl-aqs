% In the following we assume that you have the GNU Scientific Library (GSL)
% installed. You can download the GSL at 
% http://www.gnu.org/software/gsl/
% The program was written for the GSL version 1.6, but may also work with
% other versions of the GSL.
%
% To compile and link the source file, sobol.c on Linux,
% you must first copy the file(s) to a local directory, 
% and then change directory (cd) to that local directory.
% Compile the program by entering on the Matlab prompt

mex sobol.c -lgsl -lgslcblas -lm

% This uses the system compiler to create the MEX-file called sobol 
% with the appropriate extension for your system.
% You can invoke the program for example by
% >> sobol(d,n,init)
% where >> denotes the Matlab prompt and 
%   - d specifies the dimension of the point to be generated
%   - n specifies the number of points to be generated
%   - init specifies whether a new sobol sequence
%     should be started, if init is the string 'init' then the sequence will
%     initialized.
% You also skip the third argument, then the current Sobol sequence is
% continued.
% If the program is invoked for the first time, the Sobol sequence is
% initialized automatically.
% If the program is invoked with a different dimension, the Sobol sequence is
% initialized automatically.
% You may also skip the second and the third argument. Then one point of
% dimension d is returned without initializing the Sobol sequence (except
% if the dimension is different from the dimension used before

% e.g.: generate 5 points of dimension 3
% Notice: If we invoke sobol for the first time the sequence is
% initialized automatically
% sobol(3,5)
% s=sobol('state') returns a vector containing the current state of the sobol generator. 
% You cannot save the state if no points have been generated.
% To change the state of the generator: 
% sobol('state',s) Resets the state to s. 

% e.g.: generate 1 point of dimension 3 without intialization
% sobol(3)

% e.g.: generate 10 points of dimension 2 with intialization
% sobol(3,10,'init')

% Have fun!

% Example:
% Generate 10 points of dimension 1
sobol(1,10)
% Save the state of the generator
s=sobol('state');
% Generate 10 points of dimension 1
sobol(1,10)
% Generate 10 points of dimension 1, with prior initialization
sobol(1,10,'init')
% Restore the state of the generator
sobol('state',s);
% Generate 10 points of dimension 1
sobol(1,10)


