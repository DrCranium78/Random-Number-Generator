- The program runs_obs.c prepares a datafile consisting of 65536 datapoints, each representing
  the number of binary runs in sequences of 16 numbers. The 16 bit sequences are produces by 
  generating a number using the random number generator. If the number is larger than the mean, 
  a 1 is inserted, if less than the mean, a 0 i inserted.
  
- The program runs_exp.c prepares a data file consisiting of 655536 datapoints, each representing
  the number of binary runs in 16 bitt numbers representing the numbers 0 - 65535. When aggregated
  we get the expected number of observations in 16 "bins" each representinf the total number of times
  we observe number of runs from 1 to 16. Of course we could produce the same numbers by answering 
  question such as how many ways are there to combine 3 elements drawn from a collection of 16, but 
  that wouldn't be nearly as much fun, and besides, we couldn't resue the same MatLab program to
  present the data.
  
- The program runs_dist.m is a MatLab program that reads and plots a histogram of the data prepared
  by the previous programs.
  
- The program wwruns.c tests for randomness of the random numebr generator using the Wald-Wolfowitz 
  test described in the document rng.pdf.
  
- The program chiruns.c tests for randomness using a chi square goodness of fit test described in 
  section 2.1 in the accompanying document.
  