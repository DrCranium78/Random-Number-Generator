%
%  ac_dist.m
% 
%  Created: 13. Jun. 2022
%   Author: Frank Bjørnø¸
% 
%  Purpose: To read and present data from a file prepared by running the program acdist.c
%           The objective is to demonstrate that (unbiased) autocorrelations are normally distributed
%           around a mean of 0.0 with standard deviation (standard error of the autocorrelation) 1/sqrt(N)
%           where N is the samplesize.
% 
%  License:
%  
%           Copyright (C) 2022 Frank Bjørnø
% 
%           1. Permission is hereby granted, free of charge, to any person obtaining a copy 
%           of this software and associated documentation files (the "Software"), to deal 
%           in the Software without restriction, including without limitation the rights 
%           to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
%           of the Software, and to permit persons to whom the Software is furnished to do 
%           so, subject to the following conditions:
%         
%           2. The above copyright notice and this permission notice shall be included in all 
%           copies or substantial portions of the Software.
% 
%           3. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
%           INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
%           PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
%           HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
%           CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
%           OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
% 



fmt = '%f';
DATA = [];
sizeDATA = [1, inf];

%  --------------------------    Read Data    --------------------------

	%  'data_file.dat' should be in the same directory as this MatLab file
fileID = fopen('datafile.dat', 'r');

if (fileID == -1)
    disp('Error: Could not open file.')
else
    while ~feof(fileID)
        DATA = fscanf(fileID, fmt, sizeDATA);        
    end
    
    closeresult = fclose(fileID);
    if closeresult ~= 0
        disp('Error: Could not close file.')
    end
end

	% clean up
clear closeresult fileID fmt sizeDATA


%  --------     prepare data for the overlaying normal curve    --------
    
	%   standard deviation is 1 / sqrt(SAMPLESIZE), that is the square root
	%   of the size of the sample from which the correlation coefficient is 
	%   calculated. This is not the same number as the size of data points
	%   in the data file. This value is hardcoded for simplicity

x  = -0.2:0.00001:0.2;                 %  prepare data points from -0.2 to 0.2 on the x axis
y  = normpdf(x, 0.0, 0.05);            %  unbiased autocorrelation has mean 0.0
SI = size(DATA);

%  ----------------------     plot the graphs    -----------------------

	%  plot the histogram of the autocorrelations
f1 = figure(1);
set(gcf, 'position', [400 500 800 380]);
H = histogram(DATA);
H.FaceColor = [.8 .8 1];
H.EdgeColor = [.8 .8 1];
H;

hold on

	%  plot the overlaying normal curve N(0.0, 0.05)   
y = y .* SI(2) * H.BinWidth;          %  scale the y coordinates to fit the histogram
plot(x, y, 'red')
