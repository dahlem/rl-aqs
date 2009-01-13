function [value,dvalue,ddvalue,tauint,dtauint,wopt,Qval] = UWerr(Data,Stau,Nrep,Name,Quantity,varargin)
#
# call:
# [value,dvalue,ddvalue,tauint,dtauint,Qval] = ...
# UWerr(Data,Stau,Nrep,Name,Quantity,P1,P2,....)
#
# autocorrelation-analysis of MC time-series
# following the Gamma-method in
# ``Monte Carlo errors with less errors''
# by Ulli Wolff, hep-lat/0306017
# 
#----------------------------------------------------------------------------
#  Ulli Wolff,   Nov. 2006 Version V6
#  new: errobars on normalized autocorrelation (Luescher formula, see below)
#       subleading terms in error of tauint changed again (see v4 of paper)
#       protection against error^2 < 0 estimates 
#----------------------------------------------------------------------------
# input arguments beyond Data maybe omitted or set to []
# in the input list; they then get default value indicated in [D=..]
#
#
# Data     -- (N x Nalpha) matrix of measured (equilibrium!) data 
#              N = total number of measurements
#              Nalpha = number of (primary) observables
#              if your data are in a different format, consider 
#              the MATLAB commands `reshape' and `permute'
#
# Stau     -- guess for the ratio S of tau/tauint [D=1.5]
#             if set = 0, absence of autocorrelations is *assumed*
#
# Nrep     -- vector [N1 N2 N3 ...] specifying a breakup of the N rows
#             of Data into replica of length N1,N2,.. (N=sum(Nrep)!)  [D=[N]]
#             The replica distribution is histogrammed and a Q-value 
#             (=probability to find this much or more scatter) is given 
#             for R >= 2 replica; if you have one history you may set Nrep to 
#             artificially split it into >=2 replica to see their distribution                
#
# Name     -- if string: name of observable for titles of generated plots
#             if not string: all plots are supressed [D='NoName']
#
# Quantity -- either:
#             scalar function handle (@functionname) for the derived 
#             observable F; it has to operate on a row-vector of length 
#             Nalpha as first argument; optional parameters P1,P2,... are 
#             passed on to this function as 2nd, 3rd, .... argument
#          -- or:
#             integer between 1 and Nalpha to analyze primary observable [D=1]
#
#----------------------------------------------------------------------------
# output::
#  value   -- estimate for F
#  dvalue  -- its error (incl. autocorrelation effects)
#  ddvalue -- statistical error of the error
#  tauint  -- integrated autocorrelation time
#  dtauint -- statistical error of tauint
#  wopt    -- optimal w value
#  Qval    -- Q-value of the replica distribution if R >= 2
#             (goodness of fit to a constant)
#----------------------------------------------------------------------------
#

## mlock  # this locks the function in memory and preserves persistent variables 
       # between calls; munlock('UWerr') required before a clear command 
## persistent f1 f2 f3  # figure handles for plot windows

# analyze input arguments:

  [N,Nalpha]=size(Data); # number of measurements and observables
  results_outdir="./results/";
  
  if nargin < 5 | isempty(Quantity)
    Quantity=1;
  endif
  if nargin < 4 | isempty(Name)
    Name="NoName";
  endif
  if nargin < 3 | isempty(Nrep)
    Nrep=[N];
  endif
  if nargin < 2 | isempty(Stau)
    Stau=1.5;
  endif

  if any(Nrep ~= round(Nrep)) | any(Nrep < 1) | sum(Nrep) ~= N
    error("inconsistent N,Nrep")
  endif

  if isnumeric(Quantity)
    if round(Quantity) ~= Quantity | Quantity < 1 | Quantity > Nalpha
      error("illegal numeric value of Quantity")
    endif
    primary = 1; # logical variable
    primaryindex=Quantity;
  else
    primary = 0;
  endif

  Nrep=Nrep(:);   # make column
  R=length(Nrep); # number of Replica

#----------------------------------------------------------------------------

# means of primaries:

  abb=mean(Data,1);    #    total mean, primary obs.
  abr=zeros(R,Nalpha); # replicum-mean, primary obs.
  i0=1;
  for r=1:R
    i1=i0-1+Nrep(r);
    abr(r,:)=mean(Data(i0:i1,:),1);
    i0=i0+Nrep(r);
  endfor

# means of derived (or primary, depending on Quantity):

  if primary
    Fbb=abb(primaryindex);
    Fbr=abr(:,primaryindex);	
  else
    Fbb=feval(Quantity,abb,varargin{:}); # total mean, derived obs.
    Fbr=zeros(R,1);                      # replica-means, derived obs.
    for i=1:R
      Fbr(i)=feval(Quantity,abr(i,:),varargin{:});
    endfor
  endif
  Fb=sum(Fbr.*Nrep)/N;  # weighed mean of replica means

#----------------------------------------------------------------------------

# form the gradient of f and project fluctuations:

  if primary
    delpro = Data(:,primaryindex)-abb(primaryindex);
  else
    fgrad=zeros(Nalpha,1);
    h=std(Data,1)/sqrt(N); # spread for num. derivative
    ainc=abb;
    for alpha=1:Nalpha
      if h(alpha) == 0     # Data for this observable do not fluctuate
	fgrad(alpha)=0;
      else
	ainc(alpha) =abb(alpha)+h(alpha);
	fgrad(alpha)=feval(Quantity,ainc,varargin{:});
	ainc(alpha) =abb(alpha)-h(alpha);
	fgrad(alpha)=fgrad(alpha)-feval(Quantity,ainc,varargin{:});
	ainc(alpha) =abb(alpha);
	fgrad(alpha)=fgrad(alpha)/(2*h(alpha));
      endif
    endfor

# projected deviations: 
    delpro = Data*fgrad-abb*fgrad;
  endif

#----------------------------------------------------------------------------
# This is returned if the error analysis fails in the following:
  value   = Fbb;
  dvalue  = 0;
  ddvalue = 0;
  tauint  = 0.5;
  dtauint = 0;
  Qval=[];
  
#----------------------------------------------------------------------------  
# Computation of Gamma, automatic windowing
# note: W=1,2,3,.. in Matlab-vectorindex <-> W=0,1,2,.. in the paper!

# values for W=0:
  GammaFbb(1) = mean(delpro.^2);
# sick case:
  if GammaFbb(1) == 0
    beep
    fprintf("\n WARNING: no fluctuations \n")
    return
  endif

# compute Gamma(t), find the optimal W

  if Stau == 0                    # no autocorrelations assumed
    Wopt=0;
    tmax=0; flag=0;
  else
    tmax = floor(min(Nrep)/2);    # do not take t larger than this
    flag=1; Gint=0;
  endif
  t=1; 
  while t <= tmax,
    GammaFbb(t+1) = 0;
				# sum over replica:
    i0=1;
    for r=1:R
      i1=i0-1+Nrep(r);
      GammaFbb(t+1) = GammaFbb(t+1) + sum(delpro(i0:i1-t).*delpro(i0+t:i1));
      i0=i0+Nrep(r);
    endfor
    GammaFbb(t+1) = GammaFbb(t+1)/(N-R*t);
    if flag
      Gint=Gint+GammaFbb(t+1)/GammaFbb(1);
      if (Gint <= 0)
	tauW=eps;
      else
	tauW = Stau/(log((Gint+1)/Gint));
      endif
      gW = exp(-t/tauW)-tauW/sqrt(t*N);
      if gW < 0                # this W is taken as optimal
	Wopt=t; 
	tmax=min(tmax,2*t); flag = 0; # Gamma up to tmax
      endif
    endif
    t=t+1;
  endwhile   # while-loop 

  if flag,
    beep
    fprintf("\n WARNING: windowing condition failed up to W = %i \n",tmax);
    Wopt=tmax;
  endif

  CFbbopt=GammaFbb(1) + 2*sum(GammaFbb(2:Wopt+1));   # first estimate
  if CFbbopt <= 0
    beep
    fprintf("\n WARNING: Gamma pathological: estimated error^2 < 0 \n");
    return
  endif

  GammaFbb=GammaFbb+CFbbopt/N;                       # bias in Gamma corrected
  CFbbopt=GammaFbb(1) + 2*sum(GammaFbb(2:Wopt+1));   # refined estimate
  sigmaF=sqrt(CFbbopt/N);                            # error of F
  rho=GammaFbb/GammaFbb(1);                          # normalized autocorr.
  tauintFbb=cumsum(rho)-0.5;

#----------------------------------------------------------------------------

# bias cancellation for the mean value

  if R >= 2
    bF = (Fb-Fbb)/(R-1);
    Fbb=Fbb - bF;
    if abs(bF) > sigmaF/4
      beep
      fprintf("\n WARNING: a %.1f sigma bias of the mean has been cancelled \n",bF/sigmaF);
    endif
    Fbr = Fbr - bF*N./Nrep;
    Fb  = Fb  - bF*R;
  endif

#----------------------------------------------------------------------------

# answers to be returned:

  value   = Fbb;
  dvalue  = sigmaF;
  ddvalue = dvalue*sqrt((Wopt+0.5)/N);
  tauint  = tauintFbb(Wopt+1);
				# changed subleading terms (prev. versions!) here:
  dtauint = tauint*2*sqrt((Wopt-tauint+0.5)/N);
  wopt = Wopt;
				# Q-value for replica distribution if R >= 2:
  
  if R >= 2
    chisq=sum((Fbr-Fb).^2.*Nrep)/CFbbopt;
    Qval=1-gammainc(chisq/2,(R-1)/2);
  else
    Qval=[];
  endif

#----------------------------------------------------------------------------

# autocorrelation:
  if Stau ~= 0

# plot of GammaF(t)/Gamma(0)=rho(t)
# contruct errors acc. to hep-lat/0409106 eq. (E.11)
# pad zeros to simplify summation:
    rho(tmax+2:2*tmax+Wopt+1)=0;

    for t=0:tmax
      k=[max(1,t-Wopt):t+Wopt];                          # summation range    
      drho(t+1)=sum((rho(k+t+1)+rho(abs(k-t)+1)-2*rho(t+1)*rho(k+1)).^2);
      drho(t+1)=sqrt(drho(t+1)/N);
    endfor
    
    x = 0:tmax;
    y = rho(1:tmax+1);
    z = drho;
    outfile = [results_outdir "normalised-autocorr-mass-" Name ".dat"];
    fd = fopen(outfile, "wt");

    fprintf(fd, "tauint,rho,drho\n");
    for i = 1:columns(x)
      fprintf(fd, "%.6f,%.6f,%.6f\n", x(i), y(i), z(i));
    endfor
    fclose(fd);
    
# tauint vs. W:
    x = 0:tmax;
    y = tauintFbb;
    z = tauintFbb.*sqrt(([0:tmax])/N)*2;
    outfile = [results_outdir "tauint_vs_w-" Name ".dat"];
    fd = fopen(outfile, "wt");

    fprintf(fd, "W,tauint,error\n");
    for i = 1:columns(x)
      fprintf(fd, "%.6f,%.6f,%.6f\n", x(i), y(i), z(i));
    endfor
    fclose(fd);
  endif

  if R >= 2
    p=(Fbr-Fb)./(sigmaF*sqrt(N./Nrep-1));  
    outfile = [results_outdir "replica_dist-" Name ".dat"];
    fd = fopen(outfile, "wt");
    fprintf(fd, "%.6f\n", p);
    fclose(fd);
  endif

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
endfunction
